#!/usr/bin/env python3
"""Extraction temporaire (passe initiale du portage) — à supprimer après la Phase 1.

Lit old/syntax.def et produit :
  - build/gen/syntax.json          données structurées (source de vérité machine)
  - doc/spec/grammar.md            grammaire des mots-clés + tables opérateurs/fonctions
"""
import json, re, sys, pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]
DEF = ROOT / "old" / "syntax.def"
OUT_JSON = ROOT / "build" / "gen" / "syntax.json"
OUT_MD = ROOT / "doc" / "spec" / "grammar.md"

SECTIONS = ["COMMANDS", "UNARIES", "BINARIES", "PRECEDENCE", "FUNCTIONS"]


def parse(text: str):
    data = {s: [] for s in SECTIONS}
    cur = None
    for line in text.splitlines():
        if line.startswith("%"):
            tag = line[1:].split()[0].upper()
            cur = tag if tag in SECTIONS else None
            continue
        if cur is None or not line.strip() or line.lstrip().startswith("#"):
            continue
        if cur == "PRECEDENCE":
            data[cur].append(line.strip())
            continue
        m = re.match(r"^([A-Z_][A-Z0-9_]*)\s*:\s*(.*)$", line)
        if not m:
            continue  # lignes de continuation (ex. bloc IF multi-lignes)
        name, rest = m.group(1), m.group(2).strip()
        entry = {"name": name, "raw": rest}
        if cur == "FUNCTIONS":
            p = rest.split()
            entry.update({"token": p[0] if p else "",
                          "minargs": int(p[1]) if len(p) > 1 and p[1].isdigit() else None,
                          "maxargs": int(p[2]) if len(p) > 2 and p[2].isdigit() else None})
        elif cur in ("UNARIES", "BINARIES"):
            p = rest.split(None, 1)
            entry["token"] = p[0]
            if len(p) > 1:
                entry["opcode"] = p[1]
        elif cur == "COMMANDS":
            toks = re.findall(r"'([^']*)'", rest)
            entry["terminals"] = toks
            entry["symbols"] = [t for t in rest.split() if not t.startswith("'") and not t.startswith("#")]
        data[cur].append(entry)
    return data


def to_md(data):
    L = ["<!-- Généré par tools/gen/extract_syntax.py depuis old/syntax.def — ne pas éditer à la main.",
         "     Document de travail Phase 1 ; voir ROADMAP.md. -->", "",
         "# Grammaire BasicScript (extraite de l'héritage ScriptBasic 2.0)", "",
         f"- Source : `old/syntax.def` ({sum(len(v) for v in data.values())} entrèves)".replace("entrèves", "entrées"),
         "", "## Mots-clés de commande (%COMMANDS)", "",
         "| Mot-clé | Terminaux | Non-terminaux |", "|---|---|---|"]
    for e in data["COMMANDS"]:
        L.append(f"| `{e['name']}` | {' '.join('`'+t+'`' for t in e['terminals'])} | {' '.join(e['symbols'][:6])} |")
    L += ["", "## Fonctions internes (%FUNCTIONS)", "",
          "| Fonction | Token | Args min/max |", "|---|---|---|"]
    for e in data["FUNCTIONS"]:
        L.append(f"| `{e['name']}` | `{e.get('token','')}` | {e.get('minargs')}–{e.get('maxargs')} |")
    L += ["", "## Opérateurs unaires (%UNARIES)", "", "| Nom | Token | Opcode |", "|---|---|---|"]
    for e in data["UNARIES"]:
        L.append(f"| `{e['name']}` | `{e.get('token','')}` | `{e.get('opcode','')}` |")
    L += ["", "## Opérateurs binaires (%BINARIES)", "", "| Nom | Token | Opcode |", "|---|---|---|"]
    for e in data["BINARIES"]:
        L.append(f"| `{e['name']}` | `{e.get('token','')}` | `{e.get('opcode','')}` |")
    L += ["", "## Précédences (%PRECEDENCE)", "", "```", *data["PRECEDENCE"], "```", ""]
    return "\n".join(L)


def main():
    data = parse(DEF.read_text(encoding="latin-1"))
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    OUT_JSON.write_text(json.dumps(data, indent=1, ensure_ascii=False))
    OUT_MD.parent.mkdir(parents=True, exist_ok=True)
    OUT_MD.write_text(to_md(data), encoding="utf-8")
    print({k: len(v) for k, v in data.items()})


if __name__ == "__main__":
    main()
