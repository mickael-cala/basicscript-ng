#!/usr/bin/env python3
# exemples.py - l'etat reel des 178 exemples, mesure et publie.
#
# Chaque .bas de examples/ est joue par l'interprete construit, avec un
# delai borne, depuis son propre dossier. Le verdict est classe :
#
#   OK          - exit 0, sans ECHEC ni error &H dans la sortie
#   module      - un module demande n'est pas construit (chargement)
#   analyse     - refuse a l'analyse (error &H68/&H74/&H77...)
#   interaction - depasse le delai (attend une saisie, un reseau, un serveur)
#   execution   - echec a l'execution ou code de sortie non nul, autre
#
# Le compte rendu est engendre entre marqueurs dans docs/EXEMPLES.md :
#
#     python scripts/exemples.py --target windows-x64 --write
#     python scripts/exemples.py --target windows-x64 --check
#
# Un exemple n'est jamais "repare" ici : s'il tourne mal, c'est une
# information, pas une faute a corriger dans la copie de l'amont.

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

RACINE = Path(__file__).resolve().parent.parent
EXEMPLES = RACINE / "examples"
PUBLIE = RACINE / "docs" / "EXEMPLES.md"
DEBUT = "<!-- DEBUT EXEMPLES ENGENDRES - scripts/exemples.py -->"
FIN = "<!-- FIN EXEMPLES ENGENDRES -->"
DELAI = 5  # secondes par exemple

MODULES = ("can not be loaded", "0x00080004", "module can not")

# Scripts Ecartes de la mesure, jamais executes : leur nom annonce une
# action materielle (arret, redemarrage, services Windows, popup GUI)
# qu'on ne tente pas, meme bornee a 5 s - deux arrets machine ont precede
# cette liste. Le verdict "ecarte" le documente au lieu de le cacher.
ECARTES = {
    "examples/1runrestart.bas": "redemarrage presume (nom)",
    "examples/runrestart.bas": "redemarrage presume (nom)",
    "examples/testshutdown.bas": "arret machine presume (nom)",
    "examples/teststservice.bas": "services Windows (nom)",
    "examples/msgbox.bas": "popup GUI bloquante",
    "examples/testmsgbox.bas": "popup GUI bloquante",
}


def classer(produit):
    """Rend (classe, detail) pour un CompletedProcess."""
    if produit.returncode < 0 or produit.returncode == 3221225477:
        return "execution", f"crash (0x{-produit.returncode:08x})"
    sortie = produit.stdout + produit.stderr
    texte = sortie.decode("utf-8", errors="replace")
    if "error &H" in texte or "ECHEC" in texte:
        if "&H3e" in texte:
            return "include", "fichier d'inclusion absent"
        for ligne in texte.splitlines():
            if "error &H" in ligne:
                return "analyse", ligne.strip()[:70]
        return "analyse", "ECHEC dans la sortie"
    if any(m in texte for m in MODULES):
        return "module", "module non construit"
    if produit.returncode != 0:
        return "execution", f"code {produit.returncode}"
    return "OK", ""


def mesurer(exe, cible):
    """Joue tous les exemples, rend {chemin relatif: (classe, detail)}."""
    conf = conf_mesure(cible)
    resultats = {}
    scripts = sorted(EXEMPLES.rglob("*.bas"))
    for script in scripts:
        rel = script.relative_to(RACINE).as_posix()
        if rel in ECARTES:
            resultats[rel] = ("ecarte", ECARTES[rel])
            continue
        cmd, cwd, env = commande(exe, cible, script, script.parent, conf)
        try:
            produit = subprocess.run(cmd, cwd=cwd, env=env,
                                     capture_output=True, timeout=DELAI)
            resultats[rel] = classer(produit)
        except subprocess.TimeoutExpired:
            resultats[rel] = ("interaction", f"> {DELAI} s")
    return resultats, len(scripts)


def conf_mesure(cible):
    """Ecrit la configuration de mesure : include et modules du depot.

    Les exemples amont supposent une distribution installee : ils
    incluent les declarations des modules (`include bdb.bas`) et
    attendent la cle `module`. La mesure assemble donc un dossier
    d'inclusion (les `.bas` du depot + ceux des modules construits) et
    une configuration dediee ; sans cela, la mesure confondrait
    'include absent' avec les vrais refus du langage.
    """
    fam = cible.split("-")[0]
    ext_dll = {"windows": ".dll", "linux": ".so", "macos": ".dylib"}[fam]
    modules = RACINE / "dist" / cible / "modules"
    inclut = RACINE / "obj" / f"exemples-{cible}-include"
    inclut.mkdir(parents=True, exist_ok=True)
    for source in list((RACINE / "include").glob("*.bas")) + \
            list((modules / "include").glob("*.bas")):
        (inclut / source.name).write_bytes(source.read_bytes())
    texte = (RACINE / "scriba.conf.lsp").read_text(encoding="utf-8")
    texte = re.sub(r'(?m)^dll\s+"[^"]*"', f'dll "{ext_dll}"', texte)
    texte = re.sub(r'(?m)^module\s+"[^"]*"',
                   f'module "{modules.as_posix()}/"', texte)
    texte = re.sub(r'(?m)^include\s+"[^"]*"',
                   f'include "{inclut.as_posix()}/"', texte)
    sortie = RACINE / "obj" / f"exemples-{cible}.conf.lsp"
    sortie.parent.mkdir(parents=True, exist_ok=True)
    sortie.write_text(texte, encoding="utf-8", newline="")
    binaire = RACINE / "dist" / cible / ("scriba" +
                                         (".exe" if fam == "windows" else ""))
    conf = RACINE / "obj" / f"exemples-{cible}.conf"
    subprocess.run([str(binaire), "-f", str(conf), "-k", str(sortie)],
                   capture_output=True, check=True)
    return conf


def commande(exe, cible, script, cwd, conf):
    """Construit la ligne de commande, WSL compris (comme tests/run.py)."""
    if cible == "linux-x64" and sys.platform == "win32":
        def wsl(p):
            p = Path(p).resolve().as_posix()
            return "/mnt/" + p[0].lower() + p[2:] if p[1] == ":" else p
        inner = (f"cd '{wsl(cwd)}' && '{wsl(exe)}' -f '{wsl(conf)}' "
                 f"'{wsl(script)}'")
        return ["wsl.exe", "-d", "Ubuntu-26.04", "--", "sh", "-c", inner], \
            cwd, None
    return [str(exe), "-f", str(conf), str(script)], cwd, None


def engendre(resultats, total, cible):
    ordre = ["OK", "module", "include", "analyse", "interaction",
             "execution", "ecarte"]
    l = [DEBUT, "",
         f"_Mesure du {time.strftime('%Y-%m-%d')} sur `{cible}`, delai "
         f"{DELAI} s par exemple, configuration du depot fournie "
         f"(include + modules construits), publication par "
         f"`scripts/exemples.py --write`. Un exemple n'est jamais repare "
         f"dans la copie de l'amont._", ""]
    l += ["| verdict | exemples |", "|---|---|"]
    for classe in ordre:
        n = sum(1 for v in resultats.values() if v[0] == classe)
        l.append(f"| `{classe}` | {n} |")
    l += [f"| **total** | **{total}** |", "",
          "`module` et `interaction` sont normaux (module non construit, "
          "saisie, reseau, serveur). `include` attend une declaration "
          "d'un module hors distribution (bdb, mysql...). Les `analyse` "
          "listent la face du langage que les exemples amont frappent - "
          "matiere de la passe langage (PLAN phase 8). Detail des "
          "exemples hors OK :", "",
          "| exemple | verdict | detail |", "|---|---|---|"]
    for rel in sorted(resultats):
        classe, detail = resultats[rel]
        if classe == "OK":
            continue
        l.append(f"| `{rel}` | {classe} | {detail} |")
    l += ["", FIN, ""]
    return "\n".join(l)


def principal():
    ap = argparse.ArgumentParser(
        description="Mesure et publie l'etat des exemples amont.")
    ap.add_argument("--target", default="windows-x64")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--write", action="store_true")
    g.add_argument("--check", action="store_true")
    args = ap.parse_args()

    famille = args.target.split("-")[0]
    ext = {"windows": ".exe"}.get(famille, "")
    exe = RACINE / "dist" / args.target / ("scriba" + ext)
    if not exe.exists():
        print(f"  ECHEC  {exe} absent : construisez d'abord")
        return 1

    resultats, total = mesurer(exe, args.target)
    contenu = engendre(resultats, total, args.target)
    texte = PUBLIE.read_text(encoding="utf-8") if PUBLIE.exists() else ""
    if DEBUT not in texte or FIN not in texte:
        if args.check:
            print(f"  ECHEC  marqueurs absents de {PUBLIE.name}")
            return 1
        PUBLIE.write_text(contenu, encoding="utf-8", newline="")
        print(f"  ecrit  {PUBLIE.relative_to(RACINE)} ({total} exemples)")
        return 0
    i = texte.index(DEBUT)
    j = texte.index("\n", texte.index(FIN)) + 1
    reconstruit = texte[:i] + contenu + texte[j:]
    if args.check:
        if texte == reconstruit:
            print(f"  exemples : a jour ({total})")
            return 0
        print("  ECHEC  docs/EXEMPLES.md a diverge : "
              "python scripts/exemples.py --write")
        return 1
    PUBLIE.write_text(reconstruit, encoding="utf-8", newline="")
    print(f"  ecrit  {PUBLIE.relative_to(RACINE)} ({total} exemples)")
    return 0


if __name__ == "__main__":
    sys.exit(principal())
