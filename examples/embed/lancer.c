/* lancer.c - l'hote minimal : charger bonus.bas et l'executer.
   Compile par build.py --libscriba contre dist/<cible>/libscriba.a. */
#include <libscriba.h>
#include <stdio.h>
#include <stdlib.h>
/* Le coeur reference GetC : la definir ici evite de tirer de l'archive
   l'objet qui porte un main() de demonstration (et son _environ). */
int GetC(void *f) { return getc((FILE *)f); }
#ifndef WIN32
char **_environ;
#endif

int main(void) {
  pSbProgram p;
  int erreur;

  p = scriba_new(malloc, free);
  if (p == NULL) return 1;
  scriba_LoadConfiguration(p, NULL); /* sans config : le langage pur suffit */
  scriba_SetFileName(p, "bonus.bas");
  erreur = scriba_LoadSourceProgram(p);
  if (erreur) { printf("hote : lecture/analyse %d\n", erreur); return 1; }
  erreur = scriba_Run(p, "");
  if (erreur) { printf("hote : execution %d\n", erreur); return 1; }
  scriba_destroy(p);
  printf("embed lancer : OK\n");
  return 0;
}
