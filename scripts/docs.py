#!/usr/bin/env python3
# docs.py - la grammaire entre dans le doc-as-code.
#
# Lit les tables de src/syntax.c - les symboles d'expression ({ "nom",
# CMD_X }) et les mots-cles d'instruction ({ "close", 314 }) - verifie la
# coherence des codes CMD_ avec src/syntax.h, et engendre la reference
# entre marqueurs dans docs/ANNEXE-GRAMMAIRE.md. Toute modification de la
# grammaire doit passer par ici avant le commit (AGENTS.md) :
#
#     python scripts/docs.py --write   # regenere l'annexe
#     python scripts/docs.py --check   # echoue si l'annexe a diverge
#
# L'autorite est src/syntax.c : c'est ce que l'interpreteur fait vraiment.
# Une grammaire declarative (syntax.def) sera recreee a la passe langage
# (PLAN phase 8) ; adopter celle de -ng aujourd'hui documenterait une
# autre langue - les deux grammaires ont diverge (95 terminaux chez -ng,
# 475 symboles ici).

import argparse
import re
import sys
from pathlib import Path

RACINE = Path(__file__).resolve().parent.parent
SYNTAX_C = RACINE / "src" / "syntax.c"
SYNTAX_H = RACINE / "src" / "syntax.h"
ANNEXE = RACINE / "docs" / "ANNEXE-GRAMMAIRE.md"
DEBUT = "<!-- DEBUT TABLES ENGENDREES - scripts/docs.py -->"
FIN = "<!-- FIN TABLES ENGENDREES -->"


def lire_syntaxe():
    """Rend (expression, instructions, defines, problemes).

    expression    : [(nom, code CMD_)] - operateurs et fonctions
    instructions  : [(nom, id numerique)] - mots-cles d'instruction
    defines       : les CMD_ definis dans syntax.h
    problemes     : les incoherences trouvees (codes orphelins, doublons)
    """
    texte = SYNTAX_C.read_text(encoding="utf-8", errors="replace")
    paires = re.findall(r'\{\s*"([^"]+)"\s*,\s*(CMD_[A-Z0-9_]+|\d+)\s*\}',
                        texte)
    expression, instructions = [], []
    vus = {}
    problemes = []
    for nom, code in paires:
        if nom in vus:
            problemes.append(f"doublon : {nom!r} ({vus[nom]} et {code})")
        vus[nom] = code
        if code.startswith("CMD_"):
            expression.append((nom, code))
        else:
            instructions.append((nom, int(code)))
    defines = set(re.findall(r'^#define\s+(CMD_[A-Z0-9_]+)',
                             SYNTAX_H.read_text(encoding="utf-8",
                                                errors="replace"), re.M))
    utilises = {c for _, c in expression}
    orphelins = sorted(utilises - defines)
    if orphelins:
        problemes.append(f"codes CMD_ sans #define : {', '.join(orphelins)}")
    return expression, instructions, defines, problemes


def engendre(expression, instructions, defines, problemes):
    """Rend le texte complet de l'annexe entre marqueurs.

    Sans horodatage : un contenu genere doit etre deterministe, sinon
    --check casse a chaque changement de minute. La provenance est celle
    du commit, pas celle de la regeneration.
    """
    l = [DEBUT, "",
         "_Reference engendree par `scripts/docs.py` depuis "
         "`src/syntax.c`. Ne pas editer a la main : "
         "`python scripts/docs.py --write`._",
         ""]
    ops = [(n, c) for n, c in expression if not re.match(r"[A-Za-z_]", n)]
    noms = [(n, c) for n, c in expression if re.match(r"[A-Za-z_]", n)]
    l += [f"## 1. Symboles d'expression non alphabetiques : {len(ops)}", "",
          "| symbole | code |", "|---|---|"]
    l += [f"| `{n}` | `{c}` |" for n, c in sorted(ops)]
    l += ["", f"## 2. Symboles d'expression alphabetiques : {len(noms)}", "",
          "| nom | code |", "|---|---|"]
    l += [f"| `{n}` | `{c}` |" for n, c in sorted(noms)]
    l += ["", f"## 3. Mots-cles d'instruction : {len(instructions)}", "",
          "| mot-cle | id de commande |", "|---|---|"]
    l += [f"| `{n}` | {i} |" for n, i in sorted(instructions)]
    l += ["", "## 4. Controles", "",
          f"- symboles d'expression : {len(expression)} "
          f"({len(ops)} operateurs, {len(noms)} noms) ;",
          f"- mots-cles d'instruction : {len(instructions)} ;",
          f"- codes CMD_ utilises : {len({c for _, c in expression})}, "
          f"definis dans syntax.h : {len(defines)} ;",
          f"- incoherences : {'aucune' if not problemes else '; '.join(problemes)}.",
          "", FIN, ""]
    return "\n".join(l)


def principal():
    ap = argparse.ArgumentParser(description="Regenere ou verifie "
                                 "docs/ANNEXE-GRAMMAIRE.md depuis src/syntax.c.")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--write", action="store_true", help="regenere l'annexe")
    g.add_argument("--check", action="store_true",
                   help="verifie seulement, echoue si divergence")
    args = ap.parse_args()

    expression, instructions, defines, problemes = lire_syntaxe()
    contenu = engendre(expression, instructions, defines, problemes)

    if problemes:
        for p in problemes:
            print(f"  incoherence : {p}")
        return 1

    if args.check:
        if not ANNEXE.exists():
            print(f"  ECHEC  {ANNEXE.name} absent : lancez --write")
            return 1
        if ANNEXE.read_text(encoding="utf-8") == contenu:
            print("  annexe grammaire : a jour")
            return 0
        print("  ECHEC  docs/ANNEXE-GRAMMAIRE.md a diverge de src/syntax.c : "
              "python scripts/docs.py --write")
        return 1

    ANNEXE.write_text(contenu, encoding="utf-8", newline="")
    n = len(expression) + len(instructions)
    print(f"  ecrit  {ANNEXE.relative_to(RACINE)} ({n} symboles)")
    return 0


if __name__ == "__main__":
    sys.exit(principal())
