#!/usr/bin/env python3
# arch.py - un produit ne doit pas mentir sur son architecture.
#
# Lit chaque binaire construit sous dist/ et verifie que la machine
# annoncee correspond a la cible de son dossier : en-tete PE (MZ) pour
# Windows, ELF pour Linux, Mach-O pour macOS. Un fichier renomme qui se
# ferait passer pour du 64 bits est detecte ici.
#
#     python scripts/arch.py            # verifie tout dist/
#     python scripts/arch.py --quiet    # n'imprime que les echecs
#
# Rc 0 si tout concorde, 1 sinon. Les archives (.a/.lib) sont ignorees.

import argparse
import struct
import sys
from pathlib import Path

RACINE = Path(__file__).resolve().parent.parent
DIST = RACINE / "dist"

# e_machine / cputype attendus, par cible, avec le nom humain.
ATTENDU = {
    "windows-x86": ("pe", 0x014c, "i386"),
    "windows-x64": ("pe", 0x8664, "x86_64"),
    "linux-x64": ("elf", 0x3e, "x86_64"),
    "linux-arm64": ("elf", 0xb7, "aarch64"),
    "macos-x64": ("macho", 0x01000007, "x86_64"),
    "macos-arm64": ("macho", 0x0100000c, "aarch64"),
}


def machine(fichier):
    """Rend (famille, code) lu dans l'en-tete, ou (None, None)."""
    donnees = fichier.read_bytes()
    if len(donnees) < 20:
        return None, None
    if donnees[:2] == b"MZ":  # PE : e_lfanew puis machine du COFF
        if len(donnees) < 0x40:
            return None, None
        e_lfanew = struct.unpack_from("<I", donnees, 0x3C)[0]
        if donnees[e_lfanew:e_lfanew + 4] != b"PE\0\0":
            return None, None
        code = struct.unpack_from("<H", donnees, e_lfanew + 4)[0]
        return "pe", code
    if donnees[:4] == b"\x7fELF":  # e_machine a l'octet 18
        code = struct.unpack_from("<H", donnees, 18)[0]
        return "elf", code
    if donnees[:4] in (b"\xcf\xfa\xed\xfe", b"\xce\xfa\xed\xfe"):
        # Mach-O 64 : cputype a l'octet 4 (petit boutiste sur disque)
        code = struct.unpack_from("<I", donnees, 4)[0]
        return "macho", code
    return None, None


def principal():
    ap = argparse.ArgumentParser(
        description="Verifie l'architecture des produits de dist/.")
    ap.add_argument("--quiet", action="store_true",
                    help="n'imprime que les echecs")
    args = ap.parse_args()

    if not DIST.is_dir():
        print(f"ECHEC : {DIST} absent, rien a verifier (construisez d'abord)")
        return 1
    echecs = 0
    verifies = 0
    for cible_dir in sorted(DIST.iterdir()):
        if not cible_dir.is_dir() or cible_dir.name not in ATTENDU:
            continue
        famille, code, nom = ATTENDU[cible_dir.name]
        produits = sorted(p for p in cible_dir.rglob("*")
                          if p.is_file() and p.suffix in
                          ("", ".exe", ".dll", ".so", ".dylib"))
        for produit in produits:
            lue, valeur = machine(produit)
            if lue is None:
                continue  # ni PE ni ELF ni Mach-O (conf binaire, bytecode)
            verifies += 1
            if (lue, valeur) == (famille, code):
                if not args.quiet:
                    print(f"  OK      {cible_dir.name}/"
                          f"{produit.relative_to(cible_dir)} : {nom}")
            else:
                echecs += 1
                print(f"  ECHEC   {cible_dir.name}/"
                      f"{produit.relative_to(cible_dir)} : annonce "
                      f"{nom} ({famille} 0x{code:x}), lu "
                      f"{lue} 0x{valeur:x}" if valeur is not None else
                      f"  ECHEC   {produit} : format illisible")
    if not args.quiet or echecs:
        print(f"{verifies - echecs}/{verifies} produit(s) conforme(s) "
              f"({echecs} ecart(s))")
    return 0 if echecs == 0 else 1


if __name__ == "__main__":
    sys.exit(principal())
