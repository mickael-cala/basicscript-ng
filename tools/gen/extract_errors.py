#!/usr/bin/env python3
"""Extraction temporaire (passe initiale du portage) — à supprimer après la Phase 1.

Lit old/errors.def et produit :
  - build/gen/errors.json     codes + messages structurés
  - doc/spec/errors.md        catalogue complet (remplace le squelette doc/language/errors.md)
"""
import json, re, pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]
DEF = ROOT / "old" / "errors.def"
OUT_JSON = ROOT / "build" / "gen" / "errors.json"
OUT_MD = ROOT / "doc" / "spec" / "errors.md"


def parse(text: str):
    errors, module, kind, pending_id = [], "", "", None
    for line in text.splitlines():
        s = line.strip()
        if not s or s.startswith("#"):
            continue
        m = re.match(r"^%MODULE\s+(\S+)", s)
        if m:
            module, kind = m.group(1), ""
            continue
        m = re.match(r"^%(RT|CT)\b", s)
        if m:
            kind = m.group(1)
            continue
        m = re.match(r"^([A-Z][A-Z0-9_]*)\s*$", s)
        if m and not s.startswith("%"):
            pending_id = m.group(1)
            continue
        if pending_id:
            errors.append({"module": module, "kind": kind, "id": pending_id,
                           "define": f"{module.upper()}_ERROR_{pending_id}" if module else pending_id,
                           "message": s})
            pending_id = None
    return errors


def main():
    errors = parse(DEF.read_text(encoding="latin-1"))
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    OUT_JSON.write_text(json.dumps(errors, indent=1, ensure_ascii=False))

    mods = {}
    for e in errors:
        mods.setdefault(e["module"] or "(noyau)", []).append(e)
    L = ["<!-- Généré par tools/gen/extract_errors.py depuis old/errors.def — ne pas éditer à la main.",
         "     Catalogue des codes d'erreur hérités ; décisions de conservation dans ROADMAP Phase 1. -->", "",
         "# Codes d'erreur BasicScript (héritage ScriptBasic 2.0)", "",
         f"**{len(errors)} erreurs** réparties en {len(mods)} modules."
         " `CT` = compilation, `RT` = exécution.", ""]
    for mod, es in mods.items():
        L += [f"## {mod}", "", "| ID | #define C | Type | Message |", "|---|---|---|---|"]
        for e in es:
            msg = e["message"].replace("|", "\\|")
            L.append(f"| `{e['id']}` | `{e['define']}` | {e['kind']} | {msg} |")
        L.append("")
    OUT_MD.write_text("\n".join(L), encoding="utf-8")
    print(len(errors), dict((m, len(v)) for m, v in mods.items()))


if __name__ == "__main__":
    main()
