#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""pod2md.py - engendre la documentation Markdown a partir des sources.

Les blocs de documentation vivent dans le code, au-dessus de leur
implementation, entre `/**` et `*/` (commandes du noyau et des modules) ou
entre `/*POD` et `CUT*` (introductions narratives). Ce script les extrait,
convertit leur balisage POD en Markdown et ecrit :

    docs/MANUEL.md              les commandes du noyau, par section
    docs/modules/<module>.md    les fonctions documentees d'un module
    docs/modules/README.md      l'index des pages de modules

Le balisage reconnu, en debut de ligne :

    =section nom        regroupe les entrees (n'apparait pas dans le texte)
    =display / =title   signature de l'entree, portee par le titre
    =subtitle           sous-titre en italique
    =H texte            titre de niveau 2 (niveau 4 dans le manuel)
    =subsection texte   titre de niveau 3 (niveau 5 dans le manuel)
    =verbatim ... =noverbatim   bloc de code
    =item texte         element de liste
    =itemize / =noitemize / =details / =abstract / =end   ignorés

et dans le texte : T<...> et C<...> en code, B<...> en gras, I<...> en
italique, R<...> en code (reference vers une autre fonction ou un module).

Un bloc `/**` sans nom de fichier et sans `=display` est du texte
d'introduction : il est rendu tel quel, avant les sections.

Usage :
    python pod2md.py            ecrit les fichiers
    python pod2md.py --check    verifie qu'ils sont a jour (code 1 sinon)
"""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SRC = ROOT / "src"
DOCS = ROOT / "docs"
MANUEL = DOCS / "MANUEL.md"
MODULES_DIR = DOCS / "modules"

ENTETE = (
    "<!-- Fichier engendre par pod2md.py depuis les sources. -->\n"
    "<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->\n"
)

DIRECTIVE = re.compile(r"^\s*=([A-Za-z0-9_]+)\s*(.*)$")
BLOC_ETOILE = r"/\*\*(?P<nom>[A-Za-z0-9_]*)\r?\n(?P<corps>.*?)\*/"
BLOC_POD = r"/\*POD\r?\n(?P<corps>.*?)CUT\*/"


def inline(texte):
    """Convertit le balisage POD d'une ligne en Markdown."""
    texte = re.sub(r"[TCR]<([^<>]*)>", r"`\1`", texte)
    texte = re.sub(r"B<([^<>]*)>", r"**\1**", texte)
    texte = re.sub(r"I<([^<>]*)>", r"*\1*", texte)
    texte = re.sub(r"[A-Z]<([^<>]*)>", r"\1", texte)
    return texte.rstrip()


def convertir(corps, niveau_h=2, sauter_signature=False):
    """Rend le Markdown d'un bloc de documentation.

    niveau_h est le niveau des `=H` ; les `=subsection` en prennent un de plus.
    sauter_signature retire `=display` et `=title`, que le titre de l'entree
    porte deja.
    """
    sortie = []
    verbatim = False
    for brute in corps.replace("\r\n", "\n").replace("\r", "\n").split("\n"):
        ligne = brute.rstrip()
        m = DIRECTIVE.match(ligne)
        if m:
            mot = m.group(1).lower()
            argument = m.group(2).strip()
            if mot == "verbatim":
                verbatim = True
                sortie.append("```")
                continue
            if mot == "noverbatim":
                verbatim = False
                sortie.append("```")
                continue
            if verbatim:
                sortie.append(ligne)
                continue
            if mot == "section":
                continue
            if mot in ("display", "displax", "title"):
                if argument and not sauter_signature:
                    sortie.append("`" + argument + "`")
                continue
            if mot == "subtitle":
                if argument:
                    sortie.append("*" + inline(argument) + "*")
                continue
            if mot == "h":
                if argument:
                    sortie.append("#" * niveau_h + " " + inline(argument))
                continue
            if mot == "subsection":
                if argument:
                    sortie.append("#" * (niveau_h + 1) + " " + inline(argument))
                continue
            if mot == "item":
                sortie.append("- " + inline(argument))
                continue
            if mot in ("itemize", "noitemize", "details", "abstract", "end",
                       "pod", "cut"):
                continue
            # directive inconnue : le texte reste, sans le « = »
            sortie.append(inline(argument))
            continue
        if verbatim:
            sortie.append(ligne)
            continue
        sortie.append(inline(ligne))
    if verbatim:
        sortie.append("```")
    texte = "\n".join(sortie)
    texte = re.sub(r"\n{3,}", "\n\n", texte)
    return texte.strip("\n")


def blocs(chemin, motif):
    """Rend [(nom, corps)] pour les blocs de documentation d'un fichier."""
    source = chemin.read_text(encoding="latin-1")
    return [((m.groupdict().get("nom") or "").strip(), m.group("corps"))
            for m in re.finditer(motif, source, re.S)]


def section_du_bloc(corps, defaut="autre"):
    m = re.search(r"^\s*=section\s+(\S+)", corps, re.M | re.I)
    return m.group(1) if m else defaut


def titre_entree(nom, corps):
    """Titre d'une entree : sa signature, sinon son =H, sinon son nom."""
    for directive in ("display", "displax", "h", "title"):
        m = re.search(r"^\s*=" + directive + r"\s+(.+)$", corps, re.M | re.I)
        if m:
            return m.group(1).strip()
    return nom


def est_une_entree(nom, corps):
    """Une entree porte un nom de fichier ou une signature ; sinon c'est du
    texte d'introduction (souvent un =H de module)."""
    if nom:
        return True
    return re.search(r"^\s*=(display|displax)\s+\S", corps,
                     re.M | re.I) is not None


def commandes_du_noyau():
    """[(section, titre, corps)] pour tous les blocs /** de src/commands."""
    entrees = []
    for chemin in sorted((SRC / "commands").glob("*.c")):
        for nom, corps in blocs(chemin, BLOC_ETOILE):
            if est_une_entree(nom, corps):
                entrees.append((section_du_bloc(corps),
                                titre_entree(nom, corps), corps))
    return entrees


def page_du_manuel():
    par_section = {}
    for section, titre, corps in commandes_du_noyau():
        par_section.setdefault(section, []).append((titre, corps))

    lignes = [ENTETE, "# Manuel du langage", "",
              "Reference des commandes et fonctions du noyau, extraite des",
              "commentaires de `src/commands/`. Le texte est celui des sources,",
              "en anglais ; les documents de `docs/` sont en francais.", "",
              "## Sommaire", ""]
    for section in sorted(par_section):
        nombre = len(par_section[section])
        mot = "entree" if nombre == 1 else "entrees"
        lignes.append(f"- [{section}](#{section}) — {nombre} {mot}")
    lignes.append("")

    for section in sorted(par_section):
        lignes.append(f"## {section}")
        lignes.append("")
        vus = set()
        for titre, corps in sorted(par_section[section]):
            if (titre, corps) in vus:
                continue
            vus.add((titre, corps))
            lignes.append(f"### {titre}")
            lignes.append("")
            texte = convertir(corps, niveau_h=4, sauter_signature=True)
            if texte:
                lignes.append(texte)
                lignes.append("")
    return "\n".join(lignes).rstrip() + "\n"


def page_du_module(module, chemin):
    """Page d'un module : blocs /** des fonctions et blocs /*POD narratifs."""
    etoiles = blocs(chemin, BLOC_ETOILE)
    pods = blocs(chemin, BLOC_POD)
    if not etoiles and not pods:
        return None

    lignes = [ENTETE, f"# Module `{module}`", "",
              f"Source : `{chemin.relative_to(ROOT).as_posix()}`", ""]
    for _, corps in pods:
        texte = convertir(corps, niveau_h=2)
        if texte:
            lignes.append(texte)
            lignes.append("")

    par_section = {}
    for nom, corps in etoiles:
        if not est_une_entree(nom, corps):
            texte = convertir(corps, niveau_h=2)
            if texte:
                lignes.append(texte)
                lignes.append("")
            continue
        par_section.setdefault(section_du_bloc(corps), []).append(
            (titre_entree(nom, corps), corps))

    for section in sorted(par_section):
        lignes.append(f"## {section}")
        lignes.append("")
        for titre, corps in sorted(par_section[section]):
            lignes.append(f"### {titre}")
            lignes.append("")
            texte = convertir(corps, niveau_h=4, sauter_signature=True)
            if texte:
                lignes.append(texte)
                lignes.append("")
    return "\n".join(lignes).rstrip() + "\n"


def pages_des_modules():
    """{chemin: contenu} pour tous les modules qui ont de la documentation."""
    pages = {}
    index = [ENTETE, "# Modules", "",
             "Pages engendrees depuis les commentaires des `interface.c`.",
             ""]
    for dossier in sorted((SRC / "extensions").iterdir()):
        interface = dossier / "interface.c"
        if not dossier.is_dir() or not interface.is_file():
            continue
        page = page_du_module(dossier.name, interface)
        if page is None:
            continue
        pages[MODULES_DIR / (dossier.name + ".md")] = page
        index.append(f"- [{dossier.name}]({dossier.name}.md)")
    index.append("")
    pages[MODULES_DIR / "README.md"] = "\n".join(index)
    return pages


def ecrire(pages, verifier):
    """Ecrit les pages, ou verifie qu'elles sont deja a jour."""
    if verifier:
        perimes = [chemin for chemin, contenu in sorted(pages.items())
                   if not chemin.is_file() or
                   chemin.read_text(encoding="utf-8") != contenu]
        if perimes:
            for chemin in perimes:
                print(f"  PERIME {chemin.relative_to(ROOT).as_posix()}")
            print(f"{len(perimes)} fichier(s) a regenerer : python pod2md.py")
            return 1
        print(f"{len(pages)} fichier(s) a jour")
        return 0
    for chemin, contenu in sorted(pages.items()):
        chemin.parent.mkdir(parents=True, exist_ok=True)
        chemin.write_text(contenu, encoding="utf-8", newline="\n")
        print(f"  ecrit  {chemin.relative_to(ROOT).as_posix()}")
    return 0


def main():
    verifier = "--check" in sys.argv[1:]
    pages = {MANUEL: page_du_manuel()}
    pages.update(pages_des_modules())
    if not verifier:
        print("documentation :")
    return ecrire(pages, verifier)


if __name__ == "__main__":
    sys.exit(main())
