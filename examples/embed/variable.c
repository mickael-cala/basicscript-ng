/* variable.c - l'hote qui lit une variable BASIC apres execution :
   le pont BASIC -> C. */
#include <libscriba.h>
#include <stdio.h>
#include <stdlib.h>
/* Le coeur reference GetC : la definir ici evite de tirer de l'archive
   l'objet qui porte un main() de demonstration. */
int GetC(void *f) { return getc((FILE *)f); }
#ifndef WIN32
char **_environ;
#endif

int main(void) {
  pSbProgram p;
  long numero;
  pSbData donnees;

  p = scriba_new(malloc, free);
  if (p == NULL) return 1;
  scriba_SetFileName(p, "bonus.bas");
  if (scriba_LoadSourceProgram(p)) return 1;
  if (scriba_Run(p, "")) return 1;

  numero = scriba_LookupVariableByName(p, "main::total");
  if (numero == -1) return 1;
  if (scriba_GetVariable(p, numero, &donnees)) return 1;
  if (scriba_GetVariableType(p, numero) != SBT_LONG) return 1;
  printf("embed variable : OK - total = %ld\n", scriba_GetLong(p, *donnees));
  scriba_destroy(p);
  return 0;
}
