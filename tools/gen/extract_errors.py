#!/usr/bin/env python3
"""Extraction temporaire (passe initiale du portage) — à supprimer après la Phase 1.

Lit old/errors.def (+ old/errcodes.h pour les numéros C réels) et produit :
  - build/gen/errors.json     codes + messages structurés
  - doc/spec/errors.md        catalogue complet (remplace le squelette doc/language/errors.md)

Injecte aussi les artefacts générés automatiquement par Jamal mais absents du .def :
  - paires SUCCESS/MEMORY_LOW pour chaque module (SUCCESS=0 garanti, MEMORY_LOW=1) ;
  - sentinelles MAX_ERROR_CODE et LAST_<MODULE>.
Les 9 codes implicites utilisés par le code (commands/file.c, hookers.c, filesys.c,
basext.h) sans entrée dans le catalogue sont listés avec leur message d'usage.
"""
import json, re, pathlib

ROOT = pathlib.Path(__file__).resolve().parents[2]
DEF = ROOT / "old" / "errors.def"
HDR = ROOT / "old" / "errcodes.h"
OUT_JSON = ROOT / "build" / "gen" / "errors.json"
OUT_MD = ROOT / "doc" / "spec" / "errors.md"

# Codes implicites : jamais passés par ERROR() avec message catalogue, donc absents
# de errors.def, mais référencés dans le code legacy (source des messages : usage).
IMPLICIT = [
    # id, basic_alias, message, source legacy
    ("CREATIME_FAIL", "SetCreateTime", "Setting the create time of the file has failed.", "old/filesys.c"),
    ("MODTIME_FAIL", "SetModifyTime", "Setting the modify time of the file has failed.", "old/hookers.c"),
    ("ACCTIM_FAIL", "SetAccessTime", "Setting the access time of the file has failed", "old/hookers.c"),
    ("CHDIR", "ChDir", "The current directory can not be changed.", "old/commands/file.c"),
    ("CURDIR", "CurDir", "The current directory can not be retrieved for some reason.", "old/commands/file.c"),
    ("DIR_NO_OPEN", "DirNoOpen", "The directory can not be opened.", "old/commands/file.c"),
    ("INV_DNAME", "InvalidDirectoryName", "The directory name is invalid.", "old/commands/file.c"),
    ("INV_DO_OPTION", "InvalidDirOption", "The directory option argument is undefined.", "old/commands/file.c"),
    ("UNDEF_DIR", "UndefDir", "The directory argument is undefined.", "old/commands/file.c"),
]


def parse(text: str):
    errors, module, kind, pending_id, pending_alias = [], "", "", None, None
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
        m = re.match(r"^([A-Z][A-Z0-9_]*)(?:\s+([A-Za-z][A-Za-z0-9_]*))?\s*$", s)
        if m and not s.startswith("%"):
            pending_id, pending_alias = m.group(1), m.group(2)
            continue
        if pending_id:
            errors.append({"module": module, "kind": kind, "id": pending_id,
                           "basic_alias": pending_alias,
                           "define": f"{module.upper()}_ERROR_{pending_id}" if module else pending_id,
                           "message": s})
            pending_id, pending_alias = None, None
    return errors


def c_numbers():
    """Numérotation C réelle depuis old/errcodes.h (générée historiquement par Jamal)."""
    nums = {}
    for m in re.finditer(r"#define\s+([A-Z0-9_]+_ERROR_[A-Z0-9_]+|MAX_ERROR_CODE)\s+(\d+)",
                         HDR.read_text(encoding="latin-1")):
        nums[m.group(1)] = int(m.group(2))
    return nums


def inject_synthetic(errors, nums):
    """Paires SUCCESS/MEMORY_LOW (guaranteed by Jamal) + codes implicites COMMAND."""
    mods = []
    for e in errors:
        if e["module"] not in mods:
            mods.append(e["module"])
    synthetic = []
    for mod in mods:
        for eid, num in (("SUCCESS", 0), ("MEMORY_LOW", 1)):
            define = f"{mod}_ERROR_{eid}"
            synthetic.append({"module": mod, "kind": "SYN", "id": eid, "basic_alias": None,
                              "define": define, "number": nums.get(define, num),
                              "message": "(no error — success)" if eid == "SUCCESS"
                                         else "Memory allocation failed (out of memory).",
                              "synthetic": True})
    for eid, alias, msg, src in IMPLICIT:
        define = f"COMMAND_ERROR_{eid}"
        synthetic.append({"module": "COMMAND", "kind": "RT", "id": eid, "basic_alias": alias,
                          "define": define, "number": nums.get(define),
                          "message": msg, "implicit": True, "source": src})
    sentinels = [{"module": "(sentinelle)", "kind": "—", "id": "MAX_ERROR_CODE",
                  "basic_alias": None, "define": "MAX_ERROR_CODE",
                  "number": nums.get("MAX_ERROR_CODE"),
                  "message": "Sentinelle : nombre total de codes (tableau de messages).",
                  "synthetic": True}]
    for mod in mods:
        last = max((e["number"] for e in errors + synthetic
                    if e["module"] == mod and e.get("number")), default=None)
        sentinels.append({"module": "(sentinelle)", "kind": "—", "id": f"LAST_{mod}",
                          "basic_alias": None, "define": f"LAST_{mod}", "number": last,
                          "message": f"Borne haute implicite du module {mod}.",
                          "synthetic": True})
    return errors + synthetic + sentinels


def main():
    errors = parse(DEF.read_text(encoding="latin-1"))
    n_def = len(errors)
    nums = c_numbers()
    for e in errors:  # attacher les numéros C réels quand connus
        e["number"] = nums.get(e["define"])
    errors = inject_synthetic(errors, nums)
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    OUT_JSON.write_text(json.dumps(errors, indent=1, ensure_ascii=False))

    mods = {}
    for e in errors:
        mods.setdefault(e["module"] or "(noyau)", []).append(e)
    n_impl = sum(1 for e in errors if e.get("implicit"))
    n_syn = sum(1 for e in errors if e.get("synthetic") and "LAST_" not in e["id"]
                and e["id"] != "MAX_ERROR_CODE")
    L = ["<!-- Généré par tools/gen/extract_errors.py depuis old/errors.def + old/errcodes.h — ne pas éditer à la main.",
         "     Catalogue des codes d'erreur hérités ; décisions de conservation dans ROADMAP Phase 1. -->", "",
         "# Codes d'erreur BasicScript (héritage ScriptBasic 2.0)", "",
         f"**{n_def} entrées du catalogue** (`errors.def`) + **{n_syn} paires SUCCESS/MEMORY_LOW** injectées"
         f" + **{n_impl} codes implicites** + sentinelles = {len(errors)} lignes.",
         " `CT` = compilation, `RT` = exécution, `SYN` = constant généré automatiquement par Jamal.",
         " Numérotation C issue de `old/errcodes.h` (référence pour la compatibilité ERR() BASIC).", ""]
    for mod, es in mods.items():
        L += [f"## {mod}", "", "| N° | ID | Alias BASIC | #define C | Type | Message |",
              "|---|---|---|---|---|---|"]
        for e in sorted(es, key=lambda x: (x["number"] is None, x["number"])):
            msg = e["message"].replace("|", "\\|")
            num = e.get("number")
            flag = " *(implicite)*" if e.get("implicit") else ""
            L.append(f"| {num if num is not None else '—'} | `{e['id']}`{flag} | "
                     f"{e.get('basic_alias') or '—'} | `{e['define']}` | {e['kind']} | {msg} |")
        L.append("")
    L += ["## Notes", "",
          "- Les paires `SUCCESS`(0)/`MEMORY_LOW`(1) sont générées automatiquement par Jamal pour chaque"
          " module (garanti par l'en-tête de `old/errors.def`) ; elles n'apparaissent pas dans le `.def`.",
          "- `MAX_ERROR_CODE` et `LAST_<MODULE>` sont des sentinelles internes (taille de tableau / borne haute).",
          "- Les 9 codes *implicites* du module COMMAND (dossier/heure de fichier) sont utilisés par le code"
          " sans entrée message dans `errors.def` ; leurs messages ci-dessus sont reconstitués depuis l'usage"
          " (`old/commands/file.c`, `old/hookers.c`, `old/filesys.c`). Voir ERRATA.md E-004.", ""]
    OUT_MD.write_text("\n".join(L), encoding="utf-8")
    print(f"def={n_def} total={len(errors)} implicit={n_impl} syn={n_syn}")
    print(dict((m, len(v)) for m, v in mods.items()))


if __name__ == "__main__":
    main()
