/* appeler.c - l'hote qui appelle une fonction BASIC avec un argument
   et recupere la valeur rendue : le pont C -> BASIC. */
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
  long noeud;
  pSbData retour;   /* un seul SbData : la valeur rendue   */
  pSbData arguments;
  int erreur;

  p = scriba_new(malloc, free);
  if (p == NULL) return 1;
  scriba_SetFileName(p, "bonus.bas");
  if (scriba_LoadSourceProgram(p)) return 1;
  /* executer d'abord : le segment des globales n'existe qu'apres Run,
     et l'appel d'une fonction en a besoin. */
  if (scriba_Run(p, "")) return 1;

  /* la table interne est en minuscules : la fonction Double s'y
     appelle main::double, et un noeud non positif est un echec. */
  noeud = scriba_LookupFunctionByName(p, "main::double");
  if (noeud <= 0) return 1;
  retour = scriba_NewSbData(p);               /* la valeur rendue            */
  arguments = scriba_NewSbArgs(p, "i", 21L);  /* un entier : 21              */
  erreur = scriba_CallArgEx(p, noeud, retour, 1, arguments);
  if (erreur) { printf("hote : appel %d\n", erreur); return 1; }
  printf("embed appeler : OK - Double(21) = %ld\n", scriba_GetLong(p, *retour));
  scriba_DestroySbData(p, retour);
  scriba_DestroySbArgs(p, arguments, 1);
  scriba_destroy(p);
  return 0;
}
