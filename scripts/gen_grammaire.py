#!/usr/bin/env python3
"""Inventaire de la grammaire : mots-cles, fonctions, trous de numerotation.

Lit src/syntax.c et src/syntax.h, ecrit docs/GRAMMAIRE.md. Script jetable
(place dans obj/, ignore par Git) : il sert a produire le releve, pas a le
maintenir. Les invariants, eux, sont verifies par `tests/run.py --grammaire`.
"""
import re
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
S = (ROOT / "src" / "syntax.c").read_text(encoding="utf-8", errors="replace")
H = (ROOT / "src" / "syntax.h").read_text(encoding="utf-8", errors="replace")


def table(marker):
    i = S.find(marker)
    assert i >= 0, marker
    j = S.find("};", i)
    return S[i:j]


def num(name):
    m = re.search(r"#define\s+" + name + r"\s+(\d+)", H)
    return int(m.group(1)) if m else None


CMD = {n: int(v) for n, v in re.findall(r"#define\s+(CMD_\w+)\s+(\d+)", H)}
KW = {n: int(v) for n, v in re.findall(r"#define\s+(KEYWORDCODE_\w+)\s+(\d+)", H)}
START = num("START_CMD")
END = num("END_CMD")
NUM = num("NUM_CMD")
ENDEXEC = num("END_EXEC")

BODIES = {int(n): name for name, n in
          re.findall(r"(\w+)\s*,\s*/\*\s*(\d+)\s*\*/", table("CommandFunction[]"))}
NAMES = {int(n): name for name, n in
         re.findall(r'"([^"]*)"\s*,\s*/\*\s*(\d+)\s*\*/',
                    table("char *COMMANDSYMBOLS[]"))}

ASY = []
for line in table("LexNASymbol ASYMBOLS[]").splitlines():
    m = re.match(r'\{\s*"([^"]+)"\s*,\s*(CMD_\w+|\d+)\s*\}', line.strip())
    if m:
        v = m.group(2)
        ASY.append((m.group(1), CMD[v] if v in CMD else int(v)))

NASY = [(m.group(1), CMD[m.group(2)]) for m in
        re.finditer(r'\{\s*"([^"]+)"\s*,\s*(CMD_\w+)\s*\}',
                    table("LexNASymbol NASYMBOLS[]"))]

PAT = []
for m in re.finditer(r"\{ (CMD_\w+) , (\w+), \s*\{(.*?)\r?\n \}",
                     table("LineSyntax COMMANDS[]"), re.S):
    lex = re.findall(r"\{ (EX_LEX_\w+) , ([^}]+) \}", m.group(3))
    PAT.append((CMD[m.group(1)], lex))

INTERN = [(CMD[m.group(1)], int(m.group(2)), int(m.group(3)), m.group(4).strip())
          for m in re.finditer(
              r"\{\s*(CMD_\w+)\s*,\s*(\d+)\s*,\s*(\d+)\}\s*,\s*/\*([^*]+)\*/",
              table("BFun INTERNALFUNCTIONS[]"))]

BINS = [CMD[x] for x in re.findall(r"CMD_\w+", table("unsigned long BINARIES[]"))]
UNARS = [CMD[x] for x in re.findall(r"CMD_\w+", table("unsigned long UNARIES[]"))]

REV = defaultdict(list)
for name, code in ASY:
    REV[code].append(name)

PREPROC_CMD = {"module": "CMD_MODULE", "local": "CMD_LOCAL",
               "const": "CMD_CONST", "global": "CMD_GLOBAL", "var": "CMD_VAR",
               "label": "CMD_LABEL", "nlabel": "CMD_NLABEL",
               "empty": "CMD_EMPTY", "pragma": "CMD_PRAGMA",
               "gconst": "CMD_GCONST", "endmodule": "CMD_ENDMODULE"}
PREPROC = {k: CMD[v] for k, v in PREPROC_CMD.items()}


def code_of(token):
    token = token.strip()
    if token in CMD:
        return CMD[token]
    if token in KW:
        return KW[token]
    return int(token) if token.isdigit() else None


LEAD = defaultdict(set)
AUX = defaultdict(set)
for op, lex in PAT:
    if not lex:
        continue
    typ, tok = lex[0]
    if typ == "EX_LEX_NSYMBOL":
        c = code_of(tok)
        if c is not None and 250 <= c < 350:
            LEAD[c].add(op)
    for typ, tok in lex[1:]:
        if typ == "EX_LEX_NSYMBOL":
            c = code_of(tok)
            if c is not None and 250 <= c < 350:
                AUX[c].add(op)

KWNUM = sorted({c for _, c in ASY if 250 <= c < 350})
FREE_KW = [n for n in range(250, 350) if n not in KWNUM]
DEAD = [(n, NAMES[n]) for n in sorted(BODIES)
        if BODIES[n] == "NULL" and n not in {x[0] for x in INTERN}
        and n not in BINS and n not in UNARS
        and n not in {op for _, lex in PAT for op in [0]} | set()
        and n not in PREPROC.values()]
DEAD = [(n, nm) for n, nm in DEAD if n not in
        {op for op, lex in PAT}]
EXTOP = [(n, nm) for n, nm in DEAD if nm.startswith("EXTOP")]
LEGACY = [(n, nm) for n, nm in DEAD if not nm.startswith("EXTOP")]
UNPATT = sorted(n for n in BODIES
                if BODIES[n] != "NULL" and n not in {op for op, _ in PAT}
                and n not in {x[0] for x in INTERN}
                and n not in BINS and n not in UNARS)
UNCITED = sorted(set(CMD.values()) -
                 {CMD[x] for x in re.findall(r"CMD_\w+", S)})

FUNCS = defaultdict(list)
for op, lo, hi, name in INTERN:
    FUNCS[op].append((name, lo, hi))

L = []
add = L.append

add("# GRAMMAIRE — mots-clés, fonctions et numéros")
add("")
add("Relevé de la grammaire vivante, produit le 2026-10-25 en relisant")
add("[syntax.c](../src/syntax.c) et [syntax.h](../src/syntax.h). Il répond à")
add("trois questions : **quel nom porte quel numéro**, **quels numéros ne servent")
add("à rien**, et **comment attribuer les suivants**. `python tests/run.py")
add("--grammaire` vérifie déjà les tables en paires `{ \"nom\", CMD_X }` : tout")
add("nom y a un corps, tout `CMD_` a un `#define`, aucun `NOTIMP` n'est armé.")
add("Les noms de `COMMANDSYMBOLS[]`, eux, sont de simples chaînes indexées par")
add("opcode : le contrôle ne les voit pas, et c'est là que vivent les 255 noms")
add("morts (§ 6.2). Étendre le contrôle à cette table est l'étape 2 de § 7.4 ;")
add("ce document est la vue d'ensemble, pas le contrôle automatique.")
add("")
add("| en un coup d'œil | nombre |")
add("|---|---|")
add(f"| codes de mots-clés utilisés | {len(KWNUM)} sur 100 |")
add(f"| codes de mots-clés libres | {len(FREE_KW)} |")
add(f"| emplacements d'opcodes | {NUM} ({START}–{END}) |")
add(f"| opcodes avec un corps | {sum(1 for b in BODIES.values() if b != 'NULL')} |")
add(f"| opcodes sans corps | {sum(1 for b in BODIES.values() if b == 'NULL')}, "
    f"dont {len(PREPROC)} mots du préprocesseur |")
add(f"| noms morts | {len(DEAD)} |")
add(f"| commandes (opcodes / lignes) | {len({op for op, _ in PAT})} / {len(PAT)} |")
add(f"| fonctions internes (opcodes / noms) | {len(FUNCS)} / {len(INTERN)} |")
add(f"| opérateurs (binaires / unaires) | {len(BINS)} / {len(UNARS)} |")
add("")
add("## 1. Trois espaces de numéros")
add("")
add("| espace | plage | qui l'utilise | consommé |")
add("|---|---|---|---|")
add(f"| codes de mots-clés | 250–349 | `ASYMBOLS` (texte → code), les motifs de "
    f"`COMMANDS[]` | {len(KWNUM)} / 100 |")
add(f"| opcodes | {START}–{END} | `CommandFunction[]`, `COMMANDSYMBOLS[]`, "
    f"`COMMANDS[]`, `INTERNALFUNCTIONS`, `BINARIES`/`UNARIES` | {NUM} / {NUM} |")
add(f"| index de table | `opcode - {START}` | accès direct dans `execute.c`, "
    f"`builder.c` | {NUM} places |")
add("")
add("Un **mot-clé** est un mot réservé que l'analyseur lexical transforme en code")
add("(250–349) ; ce code sert de jetons dans les motifs de `COMMANDS[]`. Une")
add("**commande** est un opcode (350+) avec un corps C dans `CommandFunction[]`.")
add("Une **fonction interne** est un opcode appelable dans une expression, décrit")
add("par `INTERNALFUNCTIONS[]` (arité minimale et maximale). Un **opérateur** est")
add("un opcode présent dans `BINARIES[]` ou `UNARIES[]`. Les trois se recoupent :")
add("`time` est une fonction interne, `print` une commande, `+` un opérateur.")
add("")
add("## 2. Les mots-clés (77 codes)")
add("")
add("### 2.1 Mots qui ouvrent une ligne")
add("")
add("`rôle` distingue un mot de commande (au moins un de ses opcodes a un")
add("corps), un mot du préprocesseur (aucun corps) et un mot mixte, comme")
add("`end`, qui ouvre aussi bien `end` que `end if` ou `end module`.")
add("")
add("| code | mot | opcode(s) | rôle |")
add("|---|---|---|---|")
for c in sorted(LEAD):
    words = ", ".join(sorted(REV.get(c, [])))
    ops = ", ".join(str(o) for o in sorted(LEAD[c]))
    live = [o for o in LEAD[c] if BODIES.get(o) != "NULL"]
    dead = [o for o in LEAD[c] if BODIES.get(o) == "NULL"]
    if live and dead:
        role = "mixte"
    elif live:
        role = "commande"
    else:
        role = "préprocesseur"
    add(f"| {c} | `{words}` | {ops} | {role} |")
add("")
add("### 2.2 Mots auxiliaires (dans la ligne, pas en tête)")
add("")
add("| code | mot | apparaît dans les opcodes |")
add("|---|---|---|")
for c in sorted(AUX):
    words = ", ".join(sorted(REV.get(c, [])))
    ops = ", ".join(str(o) for o in sorted(AUX[c]))
    add(f"| {c} | `{words}` | {ops} |")
add("")
add(f"### 2.3 Mots du préprocesseur ({len(PREPROC)}, aucun corps, jamais exécutés)")
add("")
add("| mot | opcode |")
add("|---|---|")
for w, c in sorted(PREPROC.items(), key=lambda kv: kv[1]):
    add(f"| `{w}` | {c} |")
add("")
add("`--grammaire` connaît cette liste (`PREPROCESSEUR_SANS_CORPS`) et refuse")
add("tout autre nom dont l'emplacement de `CommandFunction[]` est vide.")
add("")
add("## 3. Les fonctions internes (144 opcodes, 165 noms)")
add("")
add("L'arité est celle de `INTERNALFUNCTIONS[]` : minimum, maximum. Un nom en")
add("plusieurs exemplaires est un alias du même opcode (par exemple `upper$` et")
add("`ucase`).")
add("")
add("| nom(s) | opcode | arité |")
add("|---|---|---|")
for op in sorted(FUNCS, key=lambda o: sorted(FUNCS[o])[0][0]):
    names = ", ".join(f"`{n}`" for n, _, _ in sorted(FUNCS[op]))
    lo = min(x[1] for x in FUNCS[op])
    hi = max(x[2] for x in FUNCS[op])
    add(f"| {names} | {op} | {lo}–{hi} |")
add("")
add("## 4. Les opérateurs")
add("")
add("### 4.1 Binaires (21)")
add("")
add("| symbole | opcode |")
add("|---|---|")
sym = {c: n for n, c in NASY}
word = {c: n for n, c in ASY if c >= 350}

def symbol(op):
    s = sym.get(op)
    if s is None:
        s = word.get(op) or NAMES.get(op, "?")
    return s.replace(chr(92) + chr(92), chr(92))

for op in BINS:
    add(f"| `{symbol(op)}` | {op} |")
add("")
add("### 4.2 Unaires (5)")
add("")
add("| symbole | opcode |")
add("|---|---|")
for op in UNARS:
    add(f"| `{symbol(op)}` | {op} |")
add("")
add("### 4.3 Jetons d'affectation composée")
add("")
add("Six jetons (`+=`, `-=`, `*=`, `/=`, `\\=`, `&=`) ont un corps mais aucune")
add("ligne de grammaire propre : `ex_IsCommandLET` les convertit en `LETP`,")
add("`LETM`, `LETS`, `LETD`, `LETI`, `LETC`. Leurs corps sont donc du code mort")
add("conservé : " + ", ".join(f"`{BODIES[o]}` ({o})" for o in UNPATT) + ".")
add("")
add("## 5. Les commandes (lignes de grammaire)")
add("")
add(f"{len({op for op, _ in PAT})} opcodes distincts portent les {len(PAT)} lignes")
add("de `COMMANDS[]` : une commande peut avoir plusieurs formes (`EXIT`, `EXIT")
add("FOR`, `EXIT WHILE`…).")
add("")
add("| opcode | nom | lignes | corps |")
add("|---|---|---|---|")
for op in sorted({o for o, _ in PAT}):
    n = sum(1 for o, _ in PAT if o == op)
    add(f"| {op} | `{NAMES.get(op, '?')}` | {n} | "
        f"`{BODIES.get(op, '?')}` |")
add("")
add("## 6. Les trous")
add("")
add(f"### 6.1 Codes de mots-clés libres ({len(FREE_KW)})")
add("")
add("Aucun nom ne les utilise ; ils sont disponibles pour de nouveaux mots-clés,")
add("sans toucher aux codes existants :")
add("")
add("`" + "`, `".join(str(n) for n in FREE_KW) + "`.")
add("")
add("### 6.2 Noms morts (255 opcodes sans corps)")
add("")
add(f"Sur {NUM} emplacements, **{sum(1 for b in BODIES.values() if b == 'NULL')} "
    f"sont vides**, dont les {len(PREPROC)} mots du préprocesseur. Les autres sont")
add(f"des **noms morts** : {len(DEAD)} entrées de `COMMANDSYMBOLS[]` qu'aucun")
add("motif et aucune fonction n'atteint. Elles viennent de deux sources :")
add("")
add(f"- **{len(EXTOP)} jetons `EXTOPxx`** — les opérateurs étendus retirés le")
add("  2026-10-23. Leurs noms subsistent, leurs corps sont vides, aucune ligne de")
add("  grammaire ne les produit. `execute_GetCommandByName(\"EXTOPJP\")` rend")
add("  encore 350, et l'exécuter donnerait l'erreur 28 ;")
add(f"- **{len(LEGACY)} noms de familles retirées** : "
    + ", ".join(f"`{nm}` ({n})" for n, nm in LEGACY) + ".")
add("")
add("Ces noms ne coûtent pas d'opcode (les numéros sont déjà consommés) mais ils")
add("trompent : un nom existe, donc `--grammaire` le croit vivant s'il apparaît")
add("dans une table `{ \"nom\", CMD_X }`, et un module peut le récupérer par nom.")
add("")
add("### 6.3 Autres écarts")
add("")
add(f"- **{len(UNCITED)} `CMD_` définis dans `syntax.h` ne sont cités nulle part")
add("  dans `syntax.c`** : " + ", ".join(f"`{CMD_}`" for CMD_ in
    [n for n, v in CMD.items() if v in UNCITED]) + ".")
add("- `COMMANDSYMBOLS[]` porte encore des noms qui ne correspondent plus au")
add("  corps : `SQR` (359) pointe sur `COMMAND_SQRT`, `MKD`, `MKS`, `MKI`, `MKL`,")
add("  `CVD`, `CVI`, `CVS`, `CVL`, `PAUSE`, `FORK` n'ont plus de corps.")
add("- Les six corps d'affectation composée (§ 4.3) ne sont jamais atteints.")
add("")
add("### 6.4 Trous du langage, hors numérotation")
add("")
add("Deux commandes de l'audit restent absentes : `select case` et `unlock`")
add("(le mot n'existe nulle part dans `syntax.c` ; `lock` est là, son symétrique")
add("non). `dim` est entrée le 2026-10-25 (opcode 893). `exit`/`exit for`/")
add("`exit while`/`exit do`, `elseif` et le `if` sur une ligne sont en place.")
add("`sqrt` remplace `sqr` côté langage ; seul le nom interne")
add("`COMMANDSYMBOLS[359]` dit encore `SQR` (§6.3).")
add("")
add("Une gêne d'analyse subsiste, hors numérotation : une déclaration à")
add("parenthèses vides (`sub s()`) est refusée, il faut écrire `sub s`.")
add("")
add("## 7. Stratégie de renumérotation")
add("")
add("### 7.1 Ce qui casse si les opcodes bougent")
add("")
add("- **Les binaires `.sb`** : `scriba -o` écrit `szCommandArray`, une image des")
add("  `cNODE` avec leur `OpCode` ; `scriba -b` la recharge telle quelle. Un")
add("  numéro déplacé change le sens d'un fichier existant sans le dire.")
add("- **Le C produit par `-C`** : il contient la même image en octets ; le")
add("  recompiler est obligatoire.")
add("- **Les modules externes** : `besSETCOMMAND(CMD_X, f)` et `besGETCOMMAND`")
add("  figent `CMD_*` à la compilation. Un module construit contre d'anciens")
add("  en-têtes appellerait une autre fonction.")
add("- **Les codes de mots-clés (250–349)**, eux, ne sortent pas du compilateur :")
add("  aucun fichier sauvegardé ne les contient. Les déplacer est sûr, à condition")
add("  de mettre à jour `ASYMBOLS` et les motifs ensemble.")
add("")
add("### 7.2 Trois options")
add("")
add("**N1 — Gel des numéros, noms morts marqués, trous consommés")
add("(recommandée).** Les numéros 350–893 sont déclarés ABI et ne bougent plus ;")
add("`END_CMD`/`NUM_CMD`/`END_EXEC` avancent d'un cran par commande ajoutée. Les")
add("255 noms morts de `COMMANDSYMBOLS` deviennent la sentinelle `\"-\"` — jamais")
add("`NULL`, que `strcmp` dereferencerait. En défense supplémentaire,")
add("`execute_GetCommandByName` ignore un nom dont l'emplacement de")
add("`CommandFunction[]` est vide : même un module qui demanderait `EXTOPJP`")
add("reçoit 0 — un module qui voudrait ressusciter une de ces commandes doit")
add("passer par le `CMD_*` compilé, pas par son nom. `COMMANDSYMBOLS[359]` est")
add("renommé `SQRT`, son corps réel. Les 23")
add("codes de mots-clés libres se consomment du plus petit au plus grand.")
add("`--grammaire` est étendu : un nom de `COMMANDSYMBOLS` doit avoir un corps,")
add("être un des onze mots du préprocesseur, ou valoir `\"-\"` ; un `\"-\"` doit")
add("avoir un corps vide. Coût : 255 lignes mécaniques et quelques lignes dans")
add("`execute.c` ; aucun numéro ne bouge. Gain : plus de nom trompeur, plus de")
add("résolution d'un opcode mort, règle d'attribution écrite.")
add("")
add("**N2 — Marquage des noms seulement.** On marque les 255 entrées mortes et")
add("on renomme `SQR`, sans toucher à `execute_GetCommandByName`, à la règle des")
add("codes libres ni à `--grammaire`. Aucun gain de plage : les 255 emplacements")
add("restent entre 350 et 893. C'est N1 sans le verrou de résolution ni la règle")
add("d'attribution.")
add("")
add("**N3 — Renumérotation complète par familles.** On réattribue tout : par")
add("exemple 250–299 préprocesseur, 350–499 commandes, 500–649 fonctions,")
add("650–749 opérateurs, 750–899 réservé. Il faut réécrire les quatre tables")
add("parallèles, `START_CMD`/`END_CMD`/`NUM_CMD`/`END_EXEC`, `syntax.h`, et")
add("introduire une version de bytecode refusant les anciens `.sb`. Les modules")
add("du dépôt se recompilent ; ceux d'un tiers, non. Le bénéfice est")
add("cosmétique — la numérotation actuelle n'a pas de défaut fonctionnel — pour")
add("un risque de rupture réel.")
add("")
add("### 7.3 Règle d'attribution proposée (N1)")
add("")
add("1. Un **nouveau mot-clé** prend le plus petit code libre de 250–349. Le")
add("   bloc 250–255 reste réservé au préprocesseur — aucun de ses onze mots ne")
add("   l'occupe aujourd'hui, et ils n'ont pas de corps, donc ils ne doivent pas")
add("   empiéter sur les commandes.")
add("2. Une **nouvelle commande** prend `END_CMD + 1` ; les trois constantes")
add("   (`END_CMD`, `NUM_CMD`, `END_EXEC`) avancent ensemble, et l'entrée est")
add("   ajoutée en queue de `CommandFunction[]` et `COMMANDSYMBOLS[]`.")
add("3. Une **nouvelle fonction interne** réutilise l'opcode d'une fonction")
add("   existante si c'est un alias, sinon prend `END_CMD + 1` et une ligne dans")
add("   `INTERNALFUNCTIONS`.")
add("4. Un **nom mort** ne se réattribue jamais : l'opcode reste consommé, le nom")
add("   devient la sentinelle. C'est la seule façon de garder les `.sb` lisibles.")
add("5. `--grammaire` est étendu pour refuser un nom de `COMMANDSYMBOLS` dont le")
add("   corps est vide et qui n'est pas un mot du préprocesseur — la classe des")
add("   255 noms morts actuels.")
add("")
add("### 7.4 Ce que je ferais, dans l'ordre")
add("")
add("1. Étendre `--grammaire` au contrôle des trois cas (corps, préprocesseur,")
add("   `\"-\"`). Il échoue sur l'état actuel — 255 noms morts — et sert de")
add("   garde-fou pour la suite.")
add("2. Remplacer les 255 noms morts par `\"-\"` et `SQR` par `SQRT` (même")
add("   nombres, aucun `.sb` ne change de sens). Vérifier que la campagne")
add("   par défaut, `--modules` et `--upstream` restent vertes.")
add("3. Faire ignorer par `execute_GetCommandByName` tout opcode sans corps.")
add("4. Écrire la règle 250–349 dans ce document et dans `syntax.h`.")
add("5. Garder N3 pour un éventuel format `.sb` versionné, jamais pour un simple")
add("   rangement.")
add("")

(ROOT / "docs" / "GRAMMAIRE.md").write_text("\n".join(L) + "\n", encoding="utf-8")
print("ecrit docs/GRAMMAIRE.md", len(L), "lignes")
print("codes mots-cles libres", len(FREE_KW))
print("morts", len(DEAD), "EXTOP", len(EXTOP), "legacy", len(LEGACY))
print("non cites", len(UNCITED))
