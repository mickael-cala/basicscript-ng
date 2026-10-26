#!/usr/bin/env python3
# inventory.py - les comptes de docs/INVENTAIRE.md sont generes, pas
# devines. Les nombres viennent de git (ls-files) ; les natures, elles,
# restent une decision ecrite ici. Regeneration entre marqueurs :
#
#     python scripts/inventory.py --write
#     python scripts/inventory.py --check
#
# Rc 0 si l'inventaire est a jour.

import argparse
import subprocess
import sys
from pathlib import Path

RACINE = Path(__file__).resolve().parent.parent
INVENTAIRE = RACINE / "docs" / "INVENTAIRE.md"
DEBUT = "<!-- DEBUT COMPTES ENGENDRES - scripts/inventory.py -->"
FIN = "<!-- FIN COMPTES ENGENDRES -->"

# Le prefixe git le plus long gagne : src/vendor avant src, etc.
CATEGORIES = [
    ("src/vendor/", "src/vendor/",
     "zlib 1.3.1, en-tetes curl 8.21.0 et unixODBC - intouchables, "
     "avec leur README de provenance"),
    ("src/", "src/",
     "le code C : 93 `.c`, 55 `.h`, 20 `.bas` de modules, donnees "
     "(`regex.h`, `notunix.txt`, `copyright.txt`...)"),
    ("tests/contrat/", "tests/contrat/",
     "les contrats snapshot (`.bas` + `.out`), compares octet par octet"),
    ("tests/upstream/", "tests/upstream/",
     "la suite d'origine, telle quelle, et son manifeste"),
    ("tests/", "tests/",
     "les programmes d'essai, le harnais du manuel et `run.py`"),
    ("docs/modules/", "docs/modules/",
     "pages engendrees par `pod2md.py`"),
    ("docs/", "docs/",
     "la memoire du depot (dont PLAN, ERRATA, et ANNEXE-GRAMMAIRE "
     "engendree par `scripts/docs.py`)"),
    ("examples/", "examples/",
     "les exemples d'origine (copie de l'amont, ne pas reformater)"),
    ("include/", "include/",
     "`error.bas`, `time.bas`, `modinst.bas`"),
    ("webtest/", "webtest/",
     "`index.bas`, `404.bas`"),
    ("scripts/", "scripts/",
     "l'outillage documentaire : `gen_grammaire.py`, `docs.py`, "
     "`arch.py`, `inventory.py`"),
    (".github/", ".github/",
     "le workflow de CI (en pause, PLAN phase 5)"),
]
RACINE_NATURE = ("`build.py`, `pod2md.py`, `scriba.conf.lsp`, `README.md`, "
                 "`AGENTS.md`, `LICENSE`, `.gitignore`, `.gitattributes`")


def fichiers_git():
    sortie = subprocess.run(["git", "-C", str(RACINE), "ls-files"],
                            capture_output=True, text=True, check=True)
    return [l for l in sortie.stdout.splitlines() if l]


def comptes():
    noms = fichiers_git()
    restants = list(noms)
    lignes = []
    total_par_prefixe = {}
    for prefixe, etiquette, nature in CATEGORIES:
        n = sum(1 for f in restants if f.startswith(prefixe))
        restants = [f for f in restants if not f.startswith(prefixe)]
        if n:
            total_par_prefixe[etiquette] = n
            lignes.append((etiquette, n, nature))
    racine_n = len([f for f in restants if "/" not in f])
    bas = sum(1 for f in noms if f.endswith(".bas"))
    return lignes, racine_n, len(noms), bas


def engendre():
    lignes, racine_n, total, bas = comptes()
    l = [DEBUT, "",
         "| dossier | fichiers | nature |",
         "|---|---|---|"]
    for etiquette, n, nature in lignes:
        l.append(f"| `{etiquette}` | {n} | {nature} |")
    l.append(f"| racine | {racine_n} | {RACINE_NATURE} |")
    l += ["",
          f"Total : **{total} fichiers versionnes**, dont "
          f"**{bas} fichiers `.bas`** (modules, tests, exemples, pages web "
          f"d'essai et `include/`). `GRAMMAIRE.md`, `MANUEL.md`, "
          f"`ANNEXE-GRAMMAIRE.md` et la table ci-dessus sont engendres "
          f"(`scripts/gen_grammaire.py`, `pod2md.py`, `scripts/docs.py`, "
          f"`scripts/inventory.py`) et versionnes avec eux.",
          "", FIN, ""]
    return "\n".join(l)


def principal():
    ap = argparse.ArgumentParser(
        description="Regenere ou verifie les comptes de docs/INVENTAIRE.md.")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--write", action="store_true")
    g.add_argument("--check", action="store_true")
    args = ap.parse_args()

    contenu = engendre()
    texte = INVENTAIRE.read_text(encoding="utf-8")
    if DEBUT not in texte or FIN not in texte:
        print("  ECHEC  marqueurs absents de docs/INVENTAIRE.md")
        return 1
    # Le contenu engendre porte ses propres marqueurs, ferme par la ligne
    # FIN + sa fin de ligne : on remplace du DEBUT inclus jusqu'au BOUT de
    # la ligne FIN. S'arreter au seul FIN laisserait la vieille fin de
    # ligne s'accumuler d'un octet a chaque passage.
    i = texte.index(DEBUT)
    j = texte.index(FIN)
    j = texte.index("\n", j) + 1
    reconstruit = texte[:i] + contenu + texte[j:]

    if args.check:
        if texte == reconstruit:
            print("  inventaire : a jour")
            return 0
        print("  ECHEC  docs/INVENTAIRE.md a diverge : "
              "python scripts/inventory.py --write")
        return 1

    INVENTAIRE.write_text(reconstruit, encoding="utf-8", newline="")
    _, _, total, _ = comptes()
    print(f"  ecrit  docs/INVENTAIRE.md ({total} fichiers comptes)")
    return 0


if __name__ == "__main__":
    sys.exit(principal())
