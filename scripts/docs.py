#!/usr/bin/env python3
"""
docs.py - Gestion "Documentation comme Code" pour ScriptBasic-ng
Génère et vérifie MANUEL.md à partir de syntax.def.
"""

import argparse
import hashlib
import re
from pathlib import Path

def parse_syntax_def(filepath):
    """Parse le fichier syntax.def et extrait les mots-clés et fonctions."""
    keywords = []
    functions = []
    current_section = "unknown"
    
    with open(filepath, 'r', encoding='utf-8') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith(';'):
                continue
            
            if line.startswith('; ---') and line.endswith('---'):
                current_section = line.split('---')[1].strip()
                continue
            
            # Déterminer si c'est une fonction (liste dans le bloc FONCTIONS) ou un mot-clé
            if "FONCTIONS INTERNES" in current_section:
                functions.extend(line.split())
            else:
                keywords.append(line)
                
    return keywords, functions

def generate_manual(keywords, functions, output_path):
    """Génère le fichier MANUEL.md."""
    content = "# MANUEL.md - Référence du Langage ScriptBasic-ng\n\n"
    content += "> ⚠️ **CE FICHIER EST GÉNÉRÉ AUTOMATIQUEMENT.**\n"
    content += "> Toute modification manuelle sera écrasée par `python scripts/docs.py --write`.\n\n"
    
    content += "## 📚 Mots-clés du langage\n\n"
    content += ", ".join(keywords) + "\n\n"
    
    content += "## ⚙️ Fonctions internes\n\n"
    content += ", ".join(functions) + "\n"
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f"[OK] Documentation générée : {output_path}")

def check_manual(keywords, functions, manual_path):
    """Vérifie que MANUEL.md est synchronisé avec syntax.def."""
    if not manual_path.exists():
        print(f"[ERREUR] {manual_path} n'existe pas. Lancez '--write' d'abord.")
        return False
        
    with open(manual_path, 'r', encoding='utf-8') as f:
        content = f.read()
        
    # Vérification simple : tous les mots-clés et fonctions doivent être présents
    missing_kw = [kw for kw in keywords if kw not in content]
    missing_fn = [fn for fn in functions if fn not in content]
    
    if missing_kw or missing_fn:
        print("[ERREUR] Divergence détectée entre syntax.def et MANUEL.md !")
        if missing_kw:
            print(f"  Mots-clés manquants dans la doc : {missing_kw}")
        if missing_fn:
            print(f"  Fonctions manquantes dans la doc : {missing_fn}")
        print("[ACTION] Exécutez 'python scripts/docs.py --write' pour corriger.")
        return False
        
    print("[OK] La documentation est parfaitement synchronisée avec la grammaire.")
    return True

def main():
    parser = argparse.ArgumentParser(description="Gestion de la documentation ScriptBasic-ng")
    parser.add_argument("action", choices=["write", "check"], help="Action à effectuer")
    args = parser.parse_args()
    
    syntax_file = Path("syntax.def")
    manual_file = Path("docs/MANUEL.md")
    
    if not syntax_file.exists():
        print("[ERREUR] syntax.def introuvable à la racine du projet.")
        sys.exit(1)
        
    keywords, functions = parse_syntax_def(syntax_file)
    
    if args.action == "write":
        generate_manual(keywords, functions, manual_file)
    elif args.action == "check":
        if not check_manual(keywords, functions, manual_file):
            sys.exit(1) # Code de sortie 1 pour faire échouer la CI

if __name__ == "__main__":
    main()