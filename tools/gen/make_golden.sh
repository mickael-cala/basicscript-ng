#!/bin/sh
# make_golden.sh — génère les sorties de référence (golden files) avec le binaire legacy.
# Requires: old/bin/exe/scriba compilé (voir BOOTSTRAP.md, perl setup.pl --unix --makefile)
set -u
cd "$(dirname "$0")/../.."
SCRIBA=./old/bin/exe/scriba
[ -x "$SCRIBA" ] || { echo "scriba introuvable : compilez d'abord old/ (BOOTSTRAP.md)"; exit 1; }

run_one() { # $1 = .bas , $2 = dossier sortie
  base=$(basename "$1" .bas)
  out="$2/$base.expected"
  # -c : mode console ; timeout 10s pour éviter les scripts bloquants (http, sleep...)
  if timeout 10 "$SCRIBA" -c "$1" >"$out" 2>"$2/$base.stderr"; then
    st=OK
  else
    st="RC=$?"
  fi
  [ -s "$2/$base.stderr" ] || rm -f "$2/$base.stderr"
  echo "$st $1 -> $out"
}

for f in test/scripts/legacy/*.bas; do run_one "$f" test/golden/legacy; done
for f in $(find test/scripts/testsuite -name '*.bas'); do run_one "$f" test/golden/testsuite; done
echo DONE
