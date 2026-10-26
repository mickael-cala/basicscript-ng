#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
build.py - compile ScriptBasic avec le compilateur C de Zig.

Aucun autre outil n'est necessaire : Zig embarque l'editeur de liens, les
en-tetes et les bibliotheques de chaque plateforme. Une seule commande
construit l'interpretteur pour les six cibles :

    python build.py --list
    python build.py --target windows-x64
    python build.py --target all
    python build.py --target windows-x64 --httpd    (le serveur sbhttpd)
    python build.py --target windows-x64 --modules  (les modules, voir MODULES)
    python build.py --target windows-x64 --standalone [SCRIPT]
                                                    (executable autonome ;
                                                     SCRIPT vaut tests/hello.bas
                                                     par defaut)
    python build.py --target windows-x64 --stndlone [SCRIPT]
                                                    (runtime -C + stndlone ;
                                                     SCRIPT vaut tests/hello.bas)
    python build.py --target windows-x64 --libscriba
                                                    (archive statique et ses
                                                     en-tetes)
    python build.py --target windows-x64 --re-engine
                                                    (essai C direct du moteur
                                                     d'expressions regulieres)

Arborescence produite :

    obj/<cible>/*.o            objets intermediaires
    dist/<cible>/scriba*       l'interpreteur
    dist/<cible>/scriba.conf   sa configuration binaire
    dist/<cible>/<script>_standalone*   executable autonome (--standalone)
    dist/<cible>/<script>_stndlone*     executable lie a stndlone (--stndlone)
    dist/<cible>/libscriba.a           archive statique (--libscriba)
    dist/<cible>/include/              ses en-tetes publics
    dist/<cible>/re_engine[.exe]       essai C du moteur de re (--re-engine)
    dist/<cible>/modules/      hash.dll, re.dll, cio.dll... (ou .so, .dylib)
    dist/<cible>/modules/include/   leurs declarations .bas (import hash.bas...)

La configuration est un fichier binaire propre a l'ABI (elle encode
sizeof(long) et la taille de ses structures) : elle est produite en executant
un binaire de la meme famille d'ABI. Les cibles non executables sur la
machine hote (Linux, macOS) recoivent la configuration de la cible hote de
meme ABI (cf. CONF_SOURCE).

Options utiles :

    --httpd          construit le serveur HTTP (sbhttpd) au lieu de
                     l'interpreteur ; scriba doit deja etre construit pour la
                     meme cible, car sbhttpd lit sa configuration binaire
    --modules        construit les modules au lieu de l'interpreteur ;
                     scriba doit deja etre construit, c'est lui qui les charge
    --standalone     construit un executable autonome a partir d'un script
                     (tests/hello.bas par defaut) ; scriba est construit
                     d'abord s'il manque ; passe par `scriba -E`
    --stndlone       construit le second chemin : `scriba -C`, puis liaison
                     du C genere avec stndlone.o et le coeur
    --libscriba      archive statique (coeur + libscriba.c + lmt_none.c) et
                     copie des en-tetes dans dist/<cible>/include/
    --re-engine      essai C du moteur d'expressions regulieres de Henry
                     Spencer, hors module : mode REG_BASIC, REG_PEND,
                     references arriere et table cnames
    --release         avec --modules : exclut les modules de test (trial)
    --zig CHEMIN     zig par defaut : C:\\outils\\zig\\0.17.0\\zig.exe
    -O2 / -O0        niveau d'optimisation (defaut -O2)
    --clean          efface obj/ et dist/
    --verbose        affiche chaque ligne de commande
"""

import argparse
import os
import shlex
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SRC = ROOT / "src"
OBJ = ROOT / "obj"
DIST = ROOT / "dist"

DEFAULT_ZIG = r"C:\outils\zig\0.17.0\zig.exe"
WSL_DISTRO = os.environ.get("WSL_DISTRO", "Ubuntu-26.04")

# ---------------------------------------------------------------------------
# Les sources de l'interpretteur.
#
# Une seule liste, celle de la cible `scriba` historique : 26 fichiers du
# coeur, 19 commandes/utilitaires, le point d'entree de la variation standard
# et la gestion de licence. `confpile.c` et `lsp.c` sont requis par
# scribacmd.c (options -i/-o de la configuration texte).
# ---------------------------------------------------------------------------
SOURCES = [
    # coeur
    "confpile.c", "lsp.c", "builder.c", "conftree.c", "dynlolib.c",
    "execute.c", "expression.c", "filesys.c", "getopt.c", "lexer.c",
    "match.c", "memory.c", "myalloc.c", "options.c", "reader.c",
    "report.c", "sym.c", "syntax.c", "errcodes.c", "hookers.c",
    "modumana.c", "uniqfnam.c", "epreproc.c", "md5.c", "ipreproc.c",
    "scriba.c",
    # commandes natives et utilitaires
    "commands/environ.c", "commands/external.c", "commands/extops.c",
    "commands/file.c", "commands/function.c", "commands/goto.c",
    "commands/if.c", "commands/let.c", "commands/mathfunc.c",
    "commands/mathops.c", "commands/print.c", "commands/string.c",
    "commands/time.c", "commands/while.c",
    "logger.c", "thread.c", "hndlptr.c", "mygmtime.c", "basext.c",
    # point d'entree et licence
    "variations/standard/scribacmd.c", "lmt_none.c",
]

# ---------------------------------------------------------------------------
# Le serveur HTTP (variation httpd).
#
# Meme coeur que scriba, sans variations/standard/scribacmd.c (il porte son
# main) ni lmt_none.c, plus httpd.c, la variation httpd et les deux modules
# que lmt_httpd.c lie en statique. -DSTATIC_LINK=1 est indispensable : sans
# lui, cgi et mt exportent chacun versmodu, bootmodu, finimodu... et l'editeur
# de liens refuse les doublons (TCC les tolerait, lld non).
#
# Le serveur n'a pas de liste de modules a charger : cgi et mt sont dans le
# binaire, lmt_httpd.c reprend leurs tables CGI_SLFST et MT_SLFST.
# ---------------------------------------------------------------------------
HTTPD_EXTRA = [
    "httpd.c",
    "variations/httpd/service.c",
    "variations/httpd/websrv.c",
    "lmt_httpd.c",
    "extensions/cgi/cgi.c",
    "extensions/cgi/interface.c",
    "extensions/mt/interface.c",
]

HTTPD_STATIC = {
    "extensions/cgi/cgi.c",
    "extensions/cgi/interface.c",
    "extensions/mt/interface.c",
}

HTTPD_SOURCES = [s for s in SOURCES
                 if s not in ("variations/standard/scribacmd.c", "lmt_none.c")] + HTTPD_EXTRA

# Le moteur de re, sans l'interface du module. C'est le meme quatuor que
# MODULES["re"]["sources"], mais lie ici dans un programme ordinaire :
# l'interface impose REG_EXTENDED, alors que l'essai doit atteindre REG_BASIC,
# les references arriere et REG_PEND. Le source de l'essai est dans tests/,
# hors de src/, parce qu'il n'appartient a aucun produit.
RE_ENGINE_ENGINE = [
    "extensions/re/regcomp.c",
    "extensions/re/regexec.c",
    "extensions/re/regerror.c",
    "extensions/re/regfree.c",
]

# Le code est passe en C23 : plus de definitions K&R, plus de listes de
# parametres vides (en C23, `()` veut dire `(void)`), plus d'`int` implicite.
# Les deux -W- qui suivent ne sont pas decoratifs : ils signalent toute
# declaration historique qui reviendrait dans une unite compilee.
COMMON = [
    "-std=c23",
    "-Wstrict-prototypes",
    "-Wdeprecated-non-prototype",
    "-fno-strict-aliasing",
    "-Wno-comment",
    "-Wno-parentheses",
    "-Wno-microsoft-anon-tag",
    "-Wno-incompatible-function-pointer-types",
    "-Wno-int-conversion",
    "-Wno-format",
    "-Wno-error=date-time",
    # Les en-tetes de ce projet ne s'incluent pas eux-memes : ils sont nés
    # d'un generateur qui les copiait dans des .c deja pourvus de <stdio.h>.
    # On fournit donc les en-tetes usuels a toutes les unites.
    "-include", "stdio.h",
    "-include", "stdlib.h",
    "-include", "string.h",
    "-include", "ctype.h",
    "-include", "time.h",
]

# ---------------------------------------------------------------------------
# Les modules charges a l'execution.
#
# Un module est une bibliotheque partagee, pas un programme : elle n'a pas de
# main, elle exporte versmodu / bootmodu / finimodu et les fonctions que les
# programmes BASIC declarent (`declare sub m alias "md5fun" lib "t"`) ou
# importent (`import t.bas`). modumana.c la charge par son nom, en essayant
# chaque dossier de la cle `module` de la configuration, suffixe de la cle
# `dll`.
#
# `re` n'est pas un seul fichier : c'est l'interface plus le moteur de Henry
# Spencer (regcomp, regexec, regerror, regfree), tous dans src/extensions/re/.
# regex_test.c est volontairement exclu : il porte son propre main().
#
# `declare` est le fichier de declaration copie dans modules/include/ pour que
# `import <module>.bas` fonctionne. trial n'en a pas d'utilisable : son
# trial.bas porte des chemins absolus D:\MyProjects\... d'une autre machine.
#
# `os` restreint un module aux systemes ou il a un sens : `cio` et `win`
# appellent l'API Windows (le fichier notunix.txt de l'amont le disait deja),
# `unix` appelle fork et chmod. Sans cette cle, le module est construit pour
# les six cibles.
#
# `defines` ajoute des options de compilation propres au module. `unix` ne
# s'inclut pas unistd.h ni sys/stat.h : sans eux, clang refuse fork() et
# chmod() (ERRATA E12), et le module ne se contente pas d'un warning.
#
# `dbg` n'est pas ici, et ne peut pas y etre : ce n'est pas un module mais un
# preprocesseur externe (interface prepext.h, charge par `scriba -p`), sans
# versmodu ni bootmodu. Voir docs/MODULES.md § 6.
#
# Cles optionnelles de la fusion (docs/PLAN.md phase 2) : `include_dirs`
# ajoute des -I vers les en-tetes vendorises de src/vendor/ ; `libs` donne
# des bibliotheques de lien par famille d'OS ; `arch` restreint la largeur
# de mots (32 ou 64) quand une DLL d'execution n'existe qu'en une taille ;
# `import_def` fabrique la bibliotheque d'import Windows par `zig dlltool`
# depuis un .def vendorise.
# ---------------------------------------------------------------------------
MODULES = {
    "hash": {
        "sources": ["extensions/hash/interface.c"],
        "declare": "hash.bas",
    },
    "re": {
        "sources": ["extensions/re/interface.c", "extensions/re/regcomp.c",
                    "extensions/re/regexec.c", "extensions/re/regerror.c",
                    "extensions/re/regfree.c"],
        "declare": "re.bas",
    },
    "t": {
        "sources": ["extensions/t/interface.c"],
        "declare": "t.bas",
    },
    # cgi et mt etaient lies en statique dans sbhttpd ; ils sont aussi des
    # modules ordinaires. Compiles ici, ils se chargent par `import cgi.bas`
    # (y compris hors serveur) et donnent un test executable aux deux.
    "cgi": {
        "sources": ["extensions/cgi/interface.c", "extensions/cgi/cgi.c"],
        "declare": "cgi.bas",
    },
    "mt": {
        "sources": ["extensions/mt/interface.c"],
        "declare": "mt.bas",
        # macOS exclu (ERRATA E47) : le test meurt en SIGSEGV sur Apple
        # Silicon ; le module n'est pas declare utilisable la-bas, ni
        # construit - le jeu modules et la suite amont sautent alors
        # tout seuls avec leur raison.
        "os": ("windows", "linux"),
    },
    # trial est un module d'essai de l'ABI : il reste dans --modules pour
    # tests/trial.bas, mais --release l'exclut d'un artefact de production.
    "trial": {
        "sources": ["extensions/trial/interface.c"],
        "declare": None,
        "test_only": True,
    },
    "cio": {
        "sources": ["extensions/cio/interface.c"],
        "declare": "cio.bas",
        "os": ("windows",),
    },
    "win": {
        "sources": ["extensions/win/interface.c"],
        "declare": "win.bas",
        "os": ("windows",),
    },
    "unix": {
        "sources": ["extensions/unix/interface.c"],
        "declare": "unix.bas",
        "os": ("linux", "macos"),
        "defines": ["-include", "unistd.h", "-include", "sys/stat.h"],
    },
    # zlib : les sources 1.3.1 sont vendorisees dans src/vendor/zlib
    # (intouchables, voir le README du dossier). L'interface porte la
    # correction venue de ScriptBasic-master-ng : des donnees incompressibles
    # rendent un flux plus long que l'entree, le tampon de sortie double au
    # lieu de rendre ZLIB_ERROR_NOCOMPRESS (PATCHES B.10 de -ng).
    "zlib": {
        "sources": ["extensions/zlib/interface.c",
                    "vendor/zlib/adler32.c", "vendor/zlib/compress.c",
                    "vendor/zlib/crc32.c", "vendor/zlib/deflate.c",
                    "vendor/zlib/gzclose.c", "vendor/zlib/gzlib.c",
                    "vendor/zlib/gzread.c", "vendor/zlib/gzwrite.c",
                    "vendor/zlib/infback.c", "vendor/zlib/inffast.c",
                    "vendor/zlib/inflate.c", "vendor/zlib/inftrees.c",
                    "vendor/zlib/trees.c", "vendor/zlib/uncompr.c",
                    "vendor/zlib/zutil.c"],
        "include_dirs": ("vendor/zlib",),
        "declare": "zlib.bas",
    },
    # odbc : le gestionnaire de pilotes ODBC de Windows (odbc32.dll). Les
    # deux architectures sont construites et testees ; l'interface porte la
    # correction E11 : SQLGetData rend un SQLLEN*, qui fait 64 bits sous
    # Windows 64. Linux est pret mais non lie : les en-tetes unixODBC sont
    # vendorises (src/vendor/odbc), reste le libodbc.so du systeme hote,
    # invisible du sysroot zig en cross-compilation (ERRATA E42).
    "odbc": {
        "sources": ["extensions/odbc/interface.c"],
        "declare": "odbc.bas",
        "os": ("windows",),
        "libs": {"windows": ("-lodbc32",)},
    },
    # dyc : appel dynamique de fonctions de DLL par nom, interface Windows
    # (LoadLibrary/GetProcAddress). ABI 32 bits seulement (ERRATA E10) :
    # le DynaCall empile les arguments dans un tampon et passe un call nu ;
    # en 64 bits, l'appele lit ses registres et meurt en SIGSEGV. Une
    # ebauche x64 venait avec le noyau (commit 82f4b41) : elle ne compile
    # pas sous zig cc (code inacheve d'un autre atelier, ERRATA E41), la
    # version 32 bits de -ng la remplace.
    "dyc": {
        "sources": ["extensions/dyc/interface.c"],
        "declare": "dyc.bas",
        "os": ("windows",),
        "arch": (32,),
    },
    # curl : en-tetes 8.21.0 vendorises (src/vendor/curl), liaison vers la
    # libcurl du systeme. Windows : l'import est genere par `zig dlltool`
    # depuis le .def vendor, la DLL d'execution est celle de MinGW
    # (libcurl-4.dll, dans le PATH). 64 bits seulement ici : aucune DLL
    # 32 bits sur la machine, et pas de libcurl de developpement sous UNIX
    # (ERRATA E40).
    "curl": {
        "sources": ["extensions/curl/interface.c"],
        "declare": "curl.bas",
        "include_dirs": ("vendor/curl/include",),
        "os": ("windows",),
        "arch": (64,),
        "import_def": "vendor/curl/libcurl-4.def",
    },
}

# Suffixe d'une bibliotheque chargee a l'execution, par famille de cible.
# Il doit concorder avec la cle `dll` de la configuration binaire : c'est
# modumana.c qui l'ajoute au nom demande par le programme BASIC.
MODULE_EXT = {"windows": ".dll", "linux": ".so", "macos": ".dylib"}

# Chaque cible : triplets acceptes par `zig cc -target`, suffixe du binaire,
# macros, bibliotheques, et famille d'ABI pour la configuration binaire.
TARGETS = {
    "windows-x86": {
        "triples": ["x86-windows-gnu"],
        "exe": "scriba.exe",
        "httpd_exe": "sbhttpd.exe",
        "defines": ["-DWIN32"],
        "libs": ["-lkernel32", "-luser32", "-lws2_32", "-ladvapi32"],
        "abi": "win32",
    },
    "windows-x64": {
        "triples": ["x86_64-windows-gnu"],
        "exe": "scriba.exe",
        "httpd_exe": "sbhttpd.exe",
        "defines": ["-DWIN32"],
        "libs": ["-lkernel32", "-luser32", "-lws2_32", "-ladvapi32"],
        "abi": "win64",
    },
    "linux-x64": {
        "triples": ["x86_64-linux-gnu"],
        "exe": "scriba",
        "httpd_exe": "sbhttpd",
        # -std=c23 est un mode ISO strict : sans _DEFAULT_SOURCE, glibc cache
        # les declarations POSIX (fileno, ftruncate, kill, S_IFDIR, h_addr).
        "defines": ["-include", "unistd.h", "-D_DEFAULT_SOURCE"],
        "libs": ["-lm", "-ldl"],
        "abi": "lp64",
    },
    "linux-arm64": {
        "triples": ["aarch64-linux-gnu"],
        "exe": "scriba",
        "httpd_exe": "sbhttpd",
        "defines": ["-include", "unistd.h", "-D_DEFAULT_SOURCE"],
        "libs": ["-lm", "-ldl"],
        "abi": "lp64",
    },
    "macos-x64": {
        "triples": ["x86_64-macos"],
        "exe": "scriba",
        "httpd_exe": "sbhttpd",
        # Meme raison que _DEFAULT_SOURCE sous Linux : le mode ISO strict
        # cache les declarations POSIX de Darwin.
        "defines": ["-include", "unistd.h", "-D_DARWIN_C_SOURCE"],
        "libs": [],
        "abi": "lp64",
    },
    "macos-arm64": {
        "triples": ["aarch64-macos"],
        "exe": "scriba",
        "httpd_exe": "sbhttpd",
        "defines": ["-include", "unistd.h", "-D_DARWIN_C_SOURCE"],
        "libs": [],
        "abi": "lp64",
    },
}

# Cible hote qui fabrique la configuration binaire de chaque famille d'ABI.
# (Une configuration win64 ne convient pas a Linux : sizeof(long) y vaut 4
# contre 8.)
CONF_SOURCE = {"win32": "windows-x86", "win64": "windows-x64", "lp64": "linux-x64"}

# Le .lsp designe l'extension des modules charges dynamiquement. Elle depend
# de l'OS de la cible, pas de la taille d'un long : macOS charge des .dylib.
CONF_DLL = {"windows": ".dll", "linux": ".so", "macos": ".dylib"}


def zig_path(args):
    if args.zig:
        return args.zig
    env = os.environ.get("ZIG")
    if env:
        return env
    for cand in (DEFAULT_ZIG, shutil.which("zig")):
        # Un chemin propose mais inexistant n'est pas un compilateur : le
        # defaut Windows ne doit pas masquer le PATH des autres postes ni
        # des coureurs de CI (diagnostic de la passe CI, 2026-10-26).
        if cand and os.path.isfile(cand):
            return cand
    sys.exit("zig introuvable : passez --zig CHEMIN ou posez la variable ZIG.")


def run(cmd, verbose, cwd=None, env=None):
    if verbose:
        print("  $ " + " ".join(cmd))
    proc = subprocess.run(cmd, cwd=cwd, env=env, capture_output=True, text=True,
                          errors="replace")
    return proc


def object_path(name, rel):
    """Chemin de l'objet d'un fichier source pour une cible."""
    return OBJ / name / (rel.replace("/", "_").replace("\\", "_")[:-2] + ".o")


def newest_header_mtime():
    """Date du plus recent en-tete du coeur.

    Un objet est recompile quand son `.c` est plus recent que lui. Sans tenir
    compte des `.h`, un changement d'en-tete laissait des objets anciens et
    produisait un binaire incoherent : le programme plantait sur n'importe
    quelle ligne, sans message de compilation (ERRATA E38).
    """
    return max((p.stat().st_mtime for p in SRC.rglob("*.h")), default=0.0)


def program_spec(name, program):
    """Sources, nom du binaire et bibliotheques d'un programme d'une cible."""
    spec = TARGETS[name]
    if program == "httpd":
        libs = list(spec["libs"])
        if name.startswith(("linux", "macos")):
            libs.append("-lpthread")
        return HTTPD_SOURCES, spec["httpd_exe"], libs
    return SOURCES, spec["exe"], spec["libs"]


def build_target(name, zig, opt, verbose, program="scriba"):
    spec = TARGETS[name]
    triple = spec["triples"][0]
    outdir = DIST / name
    objdir = OBJ / name
    sources, exe_name, libs = program_spec(name, program)
    outdir.mkdir(parents=True, exist_ok=True)
    objdir.mkdir(parents=True, exist_ok=True)

    flags = [zig, "cc", "-target", triple] + COMMON + spec["defines"] + [
        opt, "-I" + str(SRC), "-c"]

    print(f"== {name}  ({triple})" + ("  [httpd]" if program == "httpd" else ""))
    headers = newest_header_mtime()
    objects, failed = [], []
    for rel in sources:
        src = SRC / rel
        # Les objets du coeur sont partages entre scriba et sbhttpd : seuls le
        # serveur et ses deux modules statiques ont leur propre sous-dossier.
        sub = objdir if rel in SOURCES else objdir / "httpd"
        sub.mkdir(parents=True, exist_ok=True)
        obj = sub / (rel.replace("/", "_").replace("\\", "_")[:-2] + ".o")
        objects.append(obj)
        if obj.exists() and obj.stat().st_mtime >= max(src.stat().st_mtime,
                                                      headers):
            continue
        extra = ["-DSTATIC_LINK=1"] if rel in HTTPD_STATIC else []
        proc = run(flags + extra + [str(src), "-o", str(obj)], verbose)
        if proc.returncode != 0:
            failed.append((rel, proc.stderr.strip()))
            print(f"  ECHEC  {rel}")
        elif proc.stderr.strip():
            print(f"  note   {rel}: {proc.stderr.strip().splitlines()[0]}")

    if failed:
        for rel, err in failed:
            print(f"\n--- {rel} ---\n{err}")
        print(f"\n{name} : {len(failed)} fichier(s) en echec, pas de lien.")
        return False

    exe = outdir / exe_name
    cmd = [zig, "cc", "-target", triple] + libs + \
          [str(o) for o in objects] + ["-o", str(exe)]
    proc = run(cmd, verbose)
    if proc.returncode != 0:
        print(proc.stderr)
        print(f"\n{name} : echec du lien.")
        return False
    print(f"  lie    {exe.relative_to(ROOT)}")
    return True


def build_re_engine(name, zig, opt, verbose):
    """Construit l'essai C direct du moteur Spencer.

    Le programme est tests/re_engine.c ; il est lie aux quatre fichiers du
    moteur, sans l'interface du module. Il rend 0 si tous les essais passent,
    ce qui en fait un controle des en-tetes reconstruits (regex.h, regex2.h,
    utils.h, cclass.h, cname.h) et non un simple exemple.
    """
    spec = TARGETS[name]
    triple = spec["triples"][0]
    outdir = DIST / name
    objdir = OBJ / name / "re_engine"
    outdir.mkdir(parents=True, exist_ok=True)
    objdir.mkdir(parents=True, exist_ok=True)

    flags = [zig, "cc", "-target", triple] + COMMON + spec["defines"] + [
        opt, "-I" + str(SRC), "-I" + str(SRC / "extensions" / "re"),
        "-c"]
    sources = [ROOT / "tests" / "re_engine.c"] + [SRC / rel
                                                  for rel in RE_ENGINE_ENGINE]
    objects = []
    headers = newest_header_mtime()
    print(f"== {name}  ({triple})  [re_engine]")
    for source in sources:
        obj = objdir / (source.stem + ".o")
        objects.append(obj)
        if obj.exists() and obj.stat().st_mtime >= max(source.stat().st_mtime,
                                                      headers):
            continue
        proc = run(flags + [str(source), "-o", str(obj)], verbose)
        if proc.returncode != 0:
            print(f"  ECHEC  {source.relative_to(ROOT)}")
            print(proc.stderr.strip())
            return False
        if proc.stderr.strip():
            print(f"  note   {source.name}: "
                  f"{proc.stderr.strip().splitlines()[0]}")

    exe = outdir / ("re_engine.exe" if target_os(name) == "windows"
                    else "re_engine")
    if exe.exists():
        exe.unlink()
    cmd = ([zig, "cc", "-target", triple] + list(spec["libs"]) +
           [str(obj) for obj in objects] + ["-o", str(exe)])
    proc = run(cmd, verbose)
    if proc.returncode != 0 or not exe.is_file() or exe.stat().st_size == 0:
        print(proc.stderr.strip())
        if exe.is_file():
            exe.unlink()
        print(f"  ECHEC  {name} : liaison de {exe.name}")
        return False
    print(f"  lie    {exe.relative_to(ROOT)}")
    return True


def build_modules(name, zig, opt, verbose, release=False):
    """Compile les modules d'une cible en bibliotheques partagees.

    Aucun lien dans un executable : chaque module sort pret a etre charge par
    modumana.c depuis dist/<cible>/modules/, et les declarations .bas que les
    programmes importent sont copiees dans modules/include/ (la cle `include`
    de la configuration pointe la).
    """
    spec = TARGETS[name]
    triple = spec["triples"][0]
    osfam = target_os(name)
    ext = MODULE_EXT[osfam]
    outdir = DIST / name / "modules"
    objdir = OBJ / name / "modules"
    incdir = outdir / "include"
    outdir.mkdir(parents=True, exist_ok=True)
    objdir.mkdir(parents=True, exist_ok=True)

    base = [zig, "cc", "-target", triple] + COMMON + spec["defines"] + [
        opt, "-I" + str(SRC), "-shared", "-fvisibility=hidden"]
    if osfam != "windows":
        base.append("-fPIC")

    headers = newest_header_mtime()
    print(f"== {name}  modules  ({triple})")
    ok = True
    built = []
    bits = 32 if name.endswith("-x86") else 64
    for mod, m in MODULES.items():
        if "os" in m and osfam not in m["os"]:
            continue
        if "arch" in m and bits not in m["arch"]:
            continue
        lib = outdir / (mod + ext)
        if release and m.get("test_only"):
            # Un dist/ deja construit peut contenir l'ancien module : le retirer
            # pour que l'artefact de release ne le transporte pas par accident.
            if lib.exists():
                lib.unlink()
            print(f"  saute  {mod} (module de test)")
            continue
        flags = base + m.get("defines", [])
        incs = m.get("include_dirs", ())
        if isinstance(incs, dict):
            incs = incs.get(osfam, ())
        flags += ["-I" + str(SRC / d) for d in incs]
        objects, failed = [], []
        for rel in m["sources"]:
            src = SRC / rel
            obj = objdir / (rel.replace("/", "_").replace("\\", "_")[:-2] + ".o")
            objects.append(obj)
            if obj.exists() and obj.stat().st_mtime >= max(src.stat().st_mtime,
                                                          headers):
                continue
            proc = run(flags + ["-c", str(src), "-o", str(obj)], verbose)
            if proc.returncode != 0:
                failed.append((rel, proc.stderr.strip()))

        if failed:
            for rel, err in failed:
                print(f"  ECHEC  {mod}/{rel}\n{err}")
            ok = False
            continue

        # Un .def vendorise devient une bibliotheque d'import par zig
        # dlltool : c'est le chemin de curl, dont le .def decrit la DLL de
        # MinGW, jamais versionnee ici.
        extra = list(m.get("libs", {}).get(osfam, ()))
        if "import_def" in m:
            implib = objdir / (mod + "-import.lib")
            deffile = SRC / m["import_def"]
            if (not implib.exists()
                    or implib.stat().st_mtime < deffile.stat().st_mtime):
                # zig dlltool : `i386` se declare, le 64 bits est sa machine
                # par defaut ("x86_64" n'est pas une valeur acceptee).
                cmd_dt = [zig, "dlltool", "-d", str(deffile), "-l", str(implib)]
                if bits == 32:
                    cmd_dt[2:2] = ["-m", "i386"]
                proc = run(cmd_dt, verbose)
                if proc.returncode != 0:
                    print(f"  ECHEC  {mod} (dlltool)\n{proc.stderr.strip()}")
                    ok = False
                    continue
            extra.append(str(implib))

        # Le lien se fait dans obj/ : sous Windows, l'editeur de liens ecrit
        # a cote de la sortie un fichier .pdb (et, avec --out-implib, un .lib)
        # qui n'ont rien a faire dans dist/. Le module termine est ensuite
        # deplace a sa place.
        tmp_lib = objdir / (mod + ext)
        cmd = (flags + spec["libs"] + extra + [str(o) for o in objects]
               + ["-o", str(tmp_lib)])
        if osfam == "windows":
            i = cmd.index("-o")
            cmd[i:i] = [f"-Wl,--out-implib,{objdir / (mod + '.lib')}"]
        proc = run(cmd, verbose)
        if proc.returncode != 0:
            print(f"  ECHEC  {mod} (lien)\n{proc.stderr.strip()}")
            ok = False
            continue
        shutil.move(str(tmp_lib), str(lib))
        print(f"  module {lib.relative_to(ROOT)}")
        built.append(mod)

    incdir.mkdir(parents=True, exist_ok=True)
    copied = []
    for mod in built:
        declare = MODULES[mod]["declare"]
        if not declare:
            continue
        shutil.copy2(SRC / "extensions" / mod / declare, incdir / declare)
        copied.append(declare)
    if copied:
        print(f"  include {incdir.relative_to(ROOT)}/  ({', '.join(copied)})")
    return ok


def make_config(name):
    """Ecrit dist/<cible>/scriba.conf a partir de scriba.conf.lsp.

    La configuration binaire est un fichier propre a l'ABI. On l'ecrit avec un
    binaire qui peut tourner ici (Windows 32/64, Linux x64 sous WSL) : la
    commande `scriba -f <sortie> -k <texte>` compile le texte LSP.
    """
    spec = TARGETS[name]
    abi = spec["abi"]
    # Le binaire de la cible elle-meme s'il tourne ici, sinon celui de la
    # cible hote de meme ABI (Windows 32/64, ou Linux x64 sous WSL).
    src_target = name if name == host_target() else CONF_SOURCE[abi]
    src_exe = DIST / src_target / TARGETS[src_target]["exe"]
    if not src_exe.exists():
        print(f"  (configuration : {src_exe.name} absent pour {abi}, ignoree)")
        return False

    text = (ROOT / "scriba.conf.lsp").read_text(encoding="utf-8")
    text = text.replace('dll ".dll"', f'dll "{CONF_DLL[target_os(name)]}"')
    tmp = OBJ / f"scriba-{name}.conf.lsp"
    tmp.parent.mkdir(parents=True, exist_ok=True)
    tmp.write_text(text, encoding="utf-8", newline="")

    conf = DIST / name / "scriba.conf"
    if src_target == "linux-x64" and os.name == "nt":
        # le binaire est lance depuis son dossier (il y cherche ses fichiers)
        inner = (f"cd '{to_wsl(src_exe.parent)}' && ./{src_exe.name} "
                 f"-f '{to_wsl(conf)}' -k '{to_wsl(tmp)}'")
        cmd = ["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner]
    else:
        cmd = [str(src_exe), "-f", str(conf), "-k", str(tmp)]
    proc = subprocess.run(cmd, capture_output=True, text=True, errors="replace")
    if proc.returncode != 0:
        print(f"  configuration {name} : echec ({proc.stdout.strip()} {proc.stderr.strip()})")
        return False
    print(f"  conf   {conf.relative_to(ROOT)}  (depuis {src_target})")
    return True


def to_wsl(path):
    p = Path(path).resolve().as_posix()
    if len(p) > 2 and p[1] == ":":
        p = "/mnt/" + p[0].lower() + p[2:]
    return p


def can_run_target(name):
    """Vrai si le binaire de cette cible peut tourner sur la machine hote."""
    host = host_target()
    if name == host:
        return True
    # Windows x64 execute aussi un binaire 32 bits (WOW64), pas l'inverse.
    if os.name == "nt" and name == "windows-x86" and host == "windows-x64":
        return True
    # WSL execute un binaire linux-x64 ; il n'execute ni linux-arm64 ni macOS.
    return bool(os.name == "nt" and name == "linux-x64" and
                shutil.which("wsl.exe"))


def build_standalone(name, zig, opt, verbose, script):
    """Construit un executable autonome avec `scriba -E`.

    `scriba -E` recopie l'interpreteur et y ajoute le bytecode du script.
    Une fois `scriba` construit, aucun compilateur n'est necessaire ; sinon
    `build_target` le construit d'abord. Le C produit par `-C` et le runtime
    `stndlone` sont l'autre chemin, volontairement non utilise ici.
    """
    if not can_run_target(name):
        print(f"  ECHEC  {name} : --standalone exige une cible executable ici")
        return False

    spec = TARGETS[name]
    interp = DIST / name / spec["exe"]
    if not interp.exists():
        print(f"  note   {name} : scriba absent, construction prealable")
        if not build_target(name, zig, opt, verbose):
            return False

    conf = DIST / name / "scriba.conf"
    if not conf.exists() and not make_config(name):
        print(f"  ECHEC  {name} : configuration binaire impossible")
        return False

    out = DIST / name / (script.stem + "_standalone" +
                         (".exe" if target_os(name) == "windows" else ""))
    # ScriptBasic rend 0 meme quand il echoue. Sans cette suppression, un
    # `-E` en erreur laisserait l'ancien binaire en place et le pilote
    # annoncerait un succes qui ne correspond a aucun fichier nouveau.
    if out.is_file() or out.is_symlink():
        out.unlink()
    try:
        if os.name == "nt" and name == "linux-x64":
            inner = (
                f"cd {shlex.quote(to_wsl(DIST / name))} && "
                f"SCRIBACONF={shlex.quote(to_wsl(conf))} "
                f"{shlex.quote(to_wsl(interp))} -E -o "
                f"{shlex.quote(to_wsl(out))} {shlex.quote(to_wsl(script))} && "
                f"chmod +x {shlex.quote(to_wsl(out))}")
            proc = run(["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner],
                       verbose)
        else:
            env = dict(os.environ)
            if target_os(name) != "windows":
                env["SCRIBACONF"] = str(conf)
            proc = run([str(interp), "-E", "-o", str(out), str(script)],
                       verbose, cwd=str(DIST / name), env=env)
    except OSError as exc:
        print(f"  ECHEC  {name} : {exc}")
        return False

    output = (proc.stdout or "") + (proc.stderr or "")
    if (proc.returncode != 0 or "error &H" in output or
            "error 0x" in output or not out.is_file() or
            out.stat().st_size == 0):
        if proc.stdout.strip():
            print(proc.stdout.strip())
        if proc.stderr.strip():
            print(proc.stderr.strip())
        # Un fichier partiel ne doit pas rester et passer pour un produit.
        if out.is_file() or out.is_symlink():
            out.unlink()
        print(f"  ECHEC  {name} : generation de {out.name}")
        return False
    if target_os(name) != "windows" and os.name != "nt":
        out.chmod(out.stat().st_mode | 0o111)
    print(f"  autonome {out.relative_to(ROOT)}")
    return True


def build_standalone_modules(name, zig, opt, verbose, script):
    """`--standalone --modules` : l'autonome colle aussi les modules.

    Une variante de `scriba` est liee avec les modules du jeu cible
    compiles en -DSTATIC_LINK=1 : leurs fonctions deviennent internes et
    la table generee lmt_standalone.c (StaticallyLinkedModules) donne a
    modumana la porte de chacun - la SLFST du module. `scriba -E` sur
    cette variante recopie donc un exécutable qui n'a besoin de rien a
    cote de lui : ni modules/, ni scriba.conf.
    """
    if not can_run_target(name):
        print(f"  ECHEC  {name} : --standalone exige une cible executable ici")
        return False

    spec = TARGETS[name]
    triple = spec["triples"][0]
    fam = target_os(name)
    bits = 32 if name.endswith("-x86") else 64

    # Le jeu de modules : les memes regles que build_modules, sans trial.
    choix = []
    for mod, m in MODULES.items():
        if m.get("test_only"):
            continue
        if "os" in m and fam not in m["os"]:
            continue
        if "arch" in m and bits not in m["arch"]:
            continue
        choix.append((mod, m))
    if not choix:
        print(f"  ECHEC  {name} : aucun module a coller")
        return False

    if not build_target(name, zig, opt, verbose):
        print(f"  ECHEC  {name} : coeur non construit, variante abandonnee")
        return False
    conf = DIST / name / "scriba.conf"
    if not conf.exists() and not make_config(name):
        print(f"  ECHEC  {name} : configuration binaire impossible")
        return False

    sdir = OBJ / name / "statique"
    sdir.mkdir(parents=True, exist_ok=True)
    base = [zig, "cc", "-target", triple] + COMMON + spec["defines"] + [
        opt, "-I" + str(SRC), "-DSTATIC_LINK=1"]
    headers = newest_header_mtime()
    objets = []
    extras = []
    for mod, m in choix:
        incs = m.get("include_dirs", ())
        if isinstance(incs, dict):
            incs = incs.get(fam, ())
        flags = base + list(m.get("defines", ()))
        flags += ["-I" + str(SRC / d) for d in incs]
        extras += list(m.get("libs", {}).get(fam, ()))
        if "import_def" in m:
            implib = sdir / (mod + "-import.lib")
            deffile = SRC / m["import_def"]
            if (not implib.exists()
                    or implib.stat().st_mtime < deffile.stat().st_mtime):
                cmd_dt = [zig, "dlltool", "-d", str(deffile),
                          "-l", str(implib)]
                if bits == 32:
                    cmd_dt[2:2] = ["-m", "i386"]
                proc = run(cmd_dt, verbose)
                if proc.returncode != 0:
                    print(f"  ECHEC  {mod} (dlltool)\n{proc.stderr.strip()}")
                    return False
            extras.append(str(implib))
        for rel in m["sources"]:
            src = SRC / rel
            obj = sdir / (mod + "_" + rel.replace("/", "_")[:-2] + ".o")
            objets.append(obj)
            if obj.exists() and obj.stat().st_mtime >= max(
                    src.stat().st_mtime, headers):
                continue
            proc = run(flags + ["-c", str(src), "-o", str(obj)], verbose)
            if proc.returncode != 0:
                print(f"  ECHEC  {mod}/{rel}\n{proc.stderr.strip()}")
                return False

    # La table : le nom de chaque module -> sa SLFST. cgi, mt, cio et dyc
    # la portaient deja ; les autres l'ont recue a la phase 6.
    lmt_c = OBJ / name / "lmt_standalone.c"
    lmt = ['#include <stdio.h>',
           '#include "basext.h"',
           '/* Genere par build.py pour --standalone --modules : ne pas',
           '   editer, changer le jeu de modules dans MODULES. */']
    for mod, _ in choix:
        lmt.append(f"extern SLFST {mod.upper()}_SLFST[];")
    lmt.append("MODLIST StaticallyLinkedModules[] ={")
    for mod, _ in choix:
        lmt.append('{"%s" , (void *)%s_SLFST },' % (mod, mod.upper()))
    lmt += ["{ NULL, NULL },", "};", ""]
    lmt_c.write_text("\n".join(lmt), encoding="utf-8", newline="")
    lmt_obj = OBJ / name / "lmt_standalone.o"
    proc = run(base + ["-c", str(lmt_c), "-o", str(lmt_obj)], verbose)
    if proc.returncode != 0:
        print(f"  ECHEC  lmt_standalone\n{proc.stderr.strip()}")
        return False

    # La variante : le coeur sans lmt_none (table vide) plus les modules.
    coeur = [object_path(name, s) for s in SOURCES if s != "lmt_none.c"]
    manquants = [p for p in coeur + [lmt_obj] + objets if not p.is_file()]
    if manquants:
        print(f"  ECHEC  {name} : objet(s) absent(s) : "
              f"{', '.join(p.name for p in manquants)}")
        return False
    variante = DIST / name / ("scriba_modules" +
                              (".exe" if fam == "windows" else ""))
    cmd = ([zig, "cc", "-target", triple] + list(spec["libs"]) + extras +
           [str(p) for p in coeur] + [str(lmt_obj)] +
           [str(p) for p in objets] + ["-o", str(variante)])
    # Pas de --out-implib ici : c'est un EXE, pas une DLL (l'implib fait
    # tirer le CRT de DLL et DllMain entre en collision).
    proc = run(cmd, verbose)
    if proc.returncode != 0 or not variante.is_file():
        print(proc.stderr.strip())
        print(f"  ECHEC  {name} : liaison de la variante a modules statiques")
        return False
    print(f"  variante {variante.relative_to(ROOT)} "
          f"({len(choix)} modules statiques)")

    # -E sur la variante : le produit embarque bytecode ET modules.
    out = DIST / name / (script.stem + "_standalone" +
                         (".exe" if fam == "windows" else ""))
    if out.is_file() or out.is_symlink():
        out.unlink()
    try:
        if os.name == "nt" and name == "linux-x64":
            inner = (
                f"cd {shlex.quote(to_wsl(DIST / name))} && "
                f"SCRIBACONF={shlex.quote(to_wsl(conf))} "
                f"{shlex.quote(to_wsl(variante))} -E -o "
                f"{shlex.quote(to_wsl(out))} {shlex.quote(to_wsl(script))} && "
                f"chmod +x {shlex.quote(to_wsl(out))}")
            proc = run(["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner],
                       verbose)
        else:
            env = dict(os.environ)
            if fam != "windows":
                env["SCRIBACONF"] = str(conf)
            proc = run([str(variante), "-E", "-o", str(out), str(script)],
                       verbose, cwd=str(DIST / name), env=env)
    except OSError as exc:
        print(f"  ECHEC  {name} : {exc}")
        return False

    output = (proc.stdout or "") + (proc.stderr or "")
    if (proc.returncode != 0 or "error &H" in output or
            "error 0x" in output or not out.is_file() or
            out.stat().st_size == 0):
        if proc.stdout.strip():
            print(proc.stdout.strip())
        if proc.stderr.strip():
            print(proc.stderr.strip())
        if out.is_file() or out.is_symlink():
            out.unlink()
        print(f"  ECHEC  {name} : generation de {out.name}")
        return False
    if fam != "windows" and os.name != "nt":
        out.chmod(out.stat().st_mode | 0o111)
    print(f"  autonome {out.relative_to(ROOT)} (modules colles)")
    return True


def build_libscriba(name, zig, opt, verbose):
    """Construit dist/<cible>/libscriba.a et copie les en-tetes.

    L'archive contient le coeur, les commandes et lmt_none.c, SANS
    variations/standalone/libscriba.c ni scribacmd.c : le premier porte un
    main() de demonstration, le second le main() du CLI. Un hote qui lie
    l'archive fournit les siens (main, GetC que le coeur reference,
    _environ sous UNIX) - voir examples/embed/ et libscriba.h. Amont, la
    collision etait evitee parce que l'objet de demonstration n'etait tire
    de l'archive qu'a la demande ; lld tire aussi des symboles dont
    l'hote a defini sondouble : sortir l'objet est plus simple et sans
    exception de plateforme.
    """
    if not build_target(name, zig, opt, verbose):
        print(f"  ECHEC  {name} : coeur non construit, archive abandonnee")
        return False

    spec = TARGETS[name]
    triple = spec["triples"][0]

    # scribacmd.c (main du CLI) et libscriba.c (main de demonstration)
    # restent dehors : l'hote apporte le sien.
    exclus = ("variations/standard/scribacmd.c",
              "variations/standalone/libscriba.c")
    sources = [s for s in SOURCES if s not in exclus]
    members = [object_path(name, s) for s in sources]
    missing = [p for p in members if not p.is_file()]
    if missing:
        print(f"  ECHEC  {name} : objet(s) absent(s) : "
              f"{', '.join(p.name for p in missing)}")
        return False

    out = DIST / name / "libscriba.a"
    if out.exists():
        out.unlink()
    proc = run([zig, "ar", "rcs", str(out)] + [str(p) for p in members],
               verbose)
    if proc.returncode != 0 or not out.is_file():
        print(proc.stderr.strip())
        print(f"  ECHEC  {name} : archive libscriba.a")
        return False

    incdir = DIST / name / "include"
    incdir.mkdir(parents=True, exist_ok=True)
    for header in sorted(SRC.glob("*.h")):
        shutil.copy2(header, incdir / header.name)
    # la notice de l'hôte, à côté des en-têtes qu'elle inclut
    shutil.copy2(SRC / "variations" / "standalone" / "libscriba.h",
                 incdir / "libscriba.h")

    # Les hôtes embed : la preuve vivante que l'archive se consomme
    # (PLAN phase 6). Chaque .c de examples/embed/ devient un exécutable
    # dans dist/<cible>/embed/, joué par `tests/run.py --embed`.
    hotes = sorted((ROOT / "examples" / "embed").glob("*.c"))
    if hotes:
        edir = DIST / name / "embed"
        edir.mkdir(parents=True, exist_ok=True)
        suffix = ".exe" if target_os(name) == "windows" else ""
        for hote in hotes:
            produit = edir / (hote.stem + suffix)
            cmd = ([zig, "cc", "-target", triple] + spec["defines"] +
                   [opt, "-I" + str(incdir), str(hote),
                    str(out), "-o", str(produit)] + list(spec["libs"]))
            proc = run(cmd, verbose)
            if proc.returncode != 0 or not produit.is_file():
                print(proc.stderr.strip())
                print(f"  ECHEC  {name} : hote embed {hote.name}")
                return False
            print(f"  hote   {produit.relative_to(ROOT)}")
    print(f"  archive {out.relative_to(ROOT)}  "
          f"({len(members)} objets) + {incdir.relative_to(ROOT)}/")
    return True


def build_stndlone(name, zig, opt, verbose, script):
    """Construit un executable par le chemin `-C` + `stndlone.o`.

    `scriba -C` ecrit le bytecode et un main() qui appelle stndlone(). On
    compile ce C, stndlone.c et le coeur, en excluant scribacmd.o (son main)
    et lmt_none.o (stndlone.c definit deja StaticallyLinkedModules).
    """
    if not can_run_target(name):
        print(f"  ECHEC  {name} : --stndlone exige une cible executable ici")
        return False

    spec = TARGETS[name]
    triple = spec["triples"][0]
    if not build_target(name, zig, opt, verbose):
        print(f"  ECHEC  {name} : coeur non construit")
        return False

    interp = DIST / name / spec["exe"]
    conf = DIST / name / "scriba.conf"
    if not conf.exists() and not make_config(name):
        print(f"  ECHEC  {name} : configuration binaire impossible")
        return False

    generated = OBJ / name / (script.stem + "_stndlone.c")
    if generated.exists():
        generated.unlink()
    try:
        if os.name == "nt" and name == "linux-x64":
            inner = (
                f"cd {shlex.quote(to_wsl(DIST / name))} && "
                f"SCRIBACONF={shlex.quote(to_wsl(conf))} "
                f"{shlex.quote(to_wsl(interp))} -C -o "
                f"{shlex.quote(to_wsl(generated))} "
                f"{shlex.quote(to_wsl(script))}")
            proc = run(["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c", inner],
                       verbose)
        else:
            env = dict(os.environ)
            if target_os(name) != "windows":
                env["SCRIBACONF"] = str(conf)
            proc = run([str(interp), "-C", "-o", str(generated), str(script)],
                       verbose, cwd=str(DIST / name), env=env)
    except OSError as exc:
        print(f"  ECHEC  {name} : {exc}")
        return False

    output = (proc.stdout or "") + (proc.stderr or "")
    if (proc.returncode != 0 or "error &H" in output or
            "error 0x" in output or not generated.is_file() or
            generated.stat().st_size == 0):
        if proc.stdout.strip():
            print(proc.stdout.strip())
        if proc.stderr.strip():
            print(proc.stderr.strip())
        if generated.is_file():
            generated.unlink()
        print(f"  ECHEC  {name} : generation du C par scriba -C")
        return False

    generated_obj = OBJ / name / (script.stem + "_stndlone.o")
    stndlone_obj = OBJ / name / "stndlone.o"
    flags = [zig, "cc", "-target", triple] + COMMON + spec["defines"] + [
        opt, "-I" + str(SRC), "-c"]
    for source, obj in ((generated, generated_obj),
                        (SRC / "stndlone.c", stndlone_obj)):
        if obj.exists() and obj.stat().st_mtime >= source.stat().st_mtime:
            continue
        proc = run(flags + [str(source), "-o", str(obj)], verbose)
        if proc.returncode != 0:
            print(proc.stderr.strip())
            print(f"  ECHEC  {name} : compilation de {source.name}")
            return False

    core = [object_path(name, s) for s in SOURCES
            if s not in ("variations/standard/scribacmd.c", "lmt_none.c")]
    missing = [p for p in core if not p.is_file()]
    if missing:
        print(f"  ECHEC  {name} : objet(s) absent(s) : "
              f"{', '.join(p.name for p in missing)}")
        return False

    out = DIST / name / (script.stem + "_stndlone" +
                         (".exe" if target_os(name) == "windows" else ""))
    if out.exists():
        out.unlink()
    cmd = ([zig, "cc", "-target", triple] + list(spec["libs"]) +
           [str(p) for p in core] + [str(generated_obj), str(stndlone_obj),
                                     "-o", str(out)])
    proc = run(cmd, verbose)
    if proc.returncode != 0 or not out.is_file() or out.stat().st_size == 0:
        print(proc.stderr.strip())
        if out.is_file():
            out.unlink()
        print(f"  ECHEC  {name} : liaison de {out.name}")
        return False
    if target_os(name) != "windows" and os.name != "nt":
        out.chmod(out.stat().st_mode | 0o111)
    elif os.name == "nt" and name == "linux-x64":
        run(["wsl.exe", "-d", WSL_DISTRO, "--", "sh", "-c",
             f"chmod +x {shlex.quote(to_wsl(out))}"], verbose)
    print(f"  stndlone {out.relative_to(ROOT)}")
    return True


def main():
    ap = argparse.ArgumentParser(description="Compile ScriptBasic avec zig cc.")
    ap.add_argument("--target", default=None,
                    help="cible a construire (defaut : celle de la machine hote)")
    ap.add_argument("--list", action="store_true", help="liste les cibles")
    ap.add_argument("--all", action="store_true", help="toutes les cibles")
    ap.add_argument("--zig", default=None)
    ap.add_argument("-O", dest="opt", default="-O2")
    ap.add_argument("--clean", action="store_true")
    ap.add_argument("--verbose", action="store_true")
    ap.add_argument("--httpd", action="store_true",
                    help="construit sbhttpd (serveur HTTP) au lieu de scriba")
    ap.add_argument("--modules", action="store_true",
                    help="construit les modules (hash, re, t, trial, cgi, mt, "
                         "zlib, odbc, dyc, curl 64 bits, cio et win sous "
                         "Windows, unix sous UNIX)")
    ap.add_argument("--standalone", nargs="?", const="tests/hello.bas",
                    default=None, metavar="SCRIPT",
                    help="construit un executable autonome (tests/hello.bas par defaut)")
    ap.add_argument("--stndlone", nargs="?", const="tests/hello.bas",
                    default=None, metavar="SCRIPT",
                    help="construit le chemin -C + stndlone (tests/hello.bas par defaut)")
    ap.add_argument("--libscriba", action="store_true",
                    help="construit l'archive statique libscriba.a et ses en-tetes")
    ap.add_argument("--re-engine", action="store_true",
                    help="construit l'essai C du moteur d'expressions regulieres")
    ap.add_argument("--release", action="store_true",
                    help="avec --modules : exclut les modules de test (trial)")
    args = ap.parse_args()

    if args.standalone is not None and (args.all or args.httpd or
                                       args.stndlone is not None or args.libscriba):
        ap.error("--standalone exige une seule cible et ne se combine pas "
                 "avec --all, --httpd, --stndlone ou --libscriba "
                 "(--modules est permis : modules colles dans l'autonome)")
    if args.stndlone is not None and (args.all or args.modules or args.httpd or
                                      args.libscriba):
        ap.error("--stndlone exige une seule cible et ne se combine pas "
                 "avec --all, --modules, --httpd ou --libscriba")
    if args.libscriba and (args.modules or args.httpd or args.standalone is not None or
                           args.stndlone is not None):
        ap.error("--libscriba ne se combine pas avec --modules, --httpd, "
                 "--standalone ou --stndlone")
    if args.re_engine and (args.all or args.modules or args.httpd or
                           args.standalone is not None or
                           args.stndlone is not None or args.libscriba):
        ap.error("--re-engine exige une seule cible et ne se combine pas "
                 "avec --all, --modules, --httpd, --standalone, --stndlone "
                 "ou --libscriba")

    if args.list:
        for name, spec in TARGETS.items():
            print(f"  {name:<14} {spec['triples'][0]:<18} "
                  f"dist/{name}/{spec['exe']}  |  {spec['httpd_exe']}")
        print("\n  modules : " + ", ".join(MODULES) + "  (" +
              ", ".join(f"{os_}: {ext}" for os_, ext in MODULE_EXT.items()) + ")")
        print("  standalone : --standalone [SCRIPT] -> "
              "dist/<cible>/<script>_standalone[.exe]")
        print("  stndlone   : --stndlone [SCRIPT] -> "
              "dist/<cible>/<script>_stndlone[.exe]")
        print("  libscriba  : --libscriba -> dist/<cible>/libscriba.a")
        print("  re_engine  : --re-engine -> dist/<cible>/re_engine[.exe]")
        return 0

    if args.clean:
        shutil.rmtree(OBJ, ignore_errors=True)
        shutil.rmtree(DIST, ignore_errors=True)
        print("obj/ et dist/ effaces")
        return 0

    script_arg = args.standalone if args.standalone is not None else args.stndlone
    script = None
    if script_arg is not None:
        script = Path(script_arg)
        if not script.is_absolute():
            script = ROOT / script
        script = script.resolve()
        if not script.is_file():
            sys.exit(f"script introuvable : {script}")

    names = list(TARGETS) if args.all else [args.target or host_target()]
    unknown = [n for n in names if n not in TARGETS]
    if unknown:
        sys.exit(f"cible inconnue : {', '.join(unknown)} (voir --list)")

    zig = zig_path(args)
    started = time.time()
    results = {}
    for name in names:
        if args.libscriba:
            results[name] = build_libscriba(name, zig, args.opt, args.verbose)
        elif args.re_engine:
            results[name] = build_re_engine(name, zig, args.opt, args.verbose)
        elif args.stndlone is not None:
            results[name] = build_stndlone(name, zig, args.opt, args.verbose,
                                           script)
        elif args.standalone is not None and args.modules:
            results[name] = build_standalone_modules(name, zig, args.opt,
                                                     args.verbose, script)
        elif args.standalone is not None:
            results[name] = build_standalone(name, zig, args.opt, args.verbose,
                                             script)
        elif args.modules:
            results[name] = build_modules(name, zig, args.opt, args.verbose,
                                          args.release)
        else:
            program = "httpd" if args.httpd else "scriba"
            results[name] = build_target(name, zig, args.opt, args.verbose,
                                         program)
    if (not args.modules and args.standalone is None and
            args.stndlone is None and not args.libscriba and
            not args.re_engine):
        for name in names:
            if results[name]:
                make_config(name)
    print()
    for name in names:
        if args.modules:
            kind = "modules"
        elif args.standalone is not None:
            kind = "standalone"
        elif args.stndlone is not None:
            kind = "stndlone"
        elif args.libscriba:
            kind = "libscriba"
        elif args.re_engine:
            kind = "re_engine"
        else:
            kind = ""
        print(f"  {name:<14} {'OK' if results[name] else 'ECHEC'} {kind}".rstrip())
    print(f"\n{sum(results.values())}/{len(names)} cible(s) construite(s) "
          f"en {time.time() - started:.1f} s")
    return 0 if all(results.values()) else 1


def target_os(name):
    """Famille d'OS d'une cible : windows, linux ou macos."""
    return name.split("-")[0]


def host_target():
    if sys.platform.startswith("win"):
        return "windows-x64" if sys.maxsize > 2**32 else "windows-x86"
    machine = os.uname().machine
    if sys.platform == "darwin":
        return "macos-arm64" if machine == "arm64" else "macos-x64"
    return "linux-arm64" if machine in ("aarch64", "arm64") else "linux-x64"


if __name__ == "__main__":
    sys.exit(main())
