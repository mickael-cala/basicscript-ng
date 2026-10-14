#!/bin/sh
# regen.sh — passe initiale de génération depuis old/ (outils temporaires legacy)
# Usage: sh tools/gen/regen.sh
# Après la Phase 1, ce script et les extracteurs deviennent obsolètes :
# la spec vivante est doc/spec/ + les en-têtes sont écrits à la main en C23.
set -e
cd "$(dirname "$0")/../.."

echo "[1/3] En-têtes noyau -> include/basicscript/"
# Les 38 en-têtes du moteur ont été extraits de old/*.c lors de la passe initiale
# (old/ ne contient que des .c ; les .h étaient générés par headerer.pl).
# Régénération via l'outil legacy si disponible :
if command -v perl >/dev/null && [ -f old/headerer.pl ]; then
  ( cd old && perl headerer.pl ) || echo "  ! headerer.pl a échoué — en-têtes existants conservés"
else
  echo "  - headerer.pl absent ; en-têtes déjà en place dans include/basicscript/"
fi

echo "[2/3] Spec syntaxe & erreurs -> build/gen/*.json puis doc/spec/"
python3 tools/gen/extract_syntax.py old/syntax.def build/gen/syntax.json
python3 tools/gen/extract_errors.py old/errors.def build/gen/errors.json
# doc/spec/errors.md et doc/spec/grammar.md sont produits à partir des JSON.

echo "[3/3] Golden tests -> test/golden/"
echo "  ! Nécessite le binaire scriba fonctionnel (module Perl Jamal requis)."
echo "  Voir BOOTSTRAP.md section 'Build de référence', puis :"
echo "    for f in test/scripts/**/*.bas; do old/scriba \$f > \${f%.bas}.expected 2>&1; done"
echo "DONE."
