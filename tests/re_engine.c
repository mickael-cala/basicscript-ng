/*
 * re_engine.c - essai direct du moteur d'expressions regulieres, en C.
 *
 * Le module re (dist/<cible>/modules/re[.dll|.so]) ne peut pas tout montrer :
 * son interface impose
 * REG_EXTENDED et REG_PEND, et en mode etendu un antislash rend le caractere
 * suivant ordinaire, si bien que les references arriere y sont inaccessibles.
 * Ce programme appelle donc le moteur par son interface publique, comme le
 * ferait un programme C qui l'utiliserait directement (c'est le cas de la
 * libc BSD d'origine), et couvre ce que tests/re.bas ne peut pas atteindre :
 *
 *   - le mode BASIC (REG_BASIC), donc p_bre() et les references arriere, qui
 *     passent par dupl(), OBACK_ et O_BACK ;
 *   - l'option REG_PEND, qui fait borner le motif par re_endp ;
 *   - la table cnames de cname.h, atteinte seulement par [.nom.] ;
 *   - l'optimisation findmust, qui remplit g->must et g->mlen ;
 *   - la semantique de REG_NEWLINE, et le code d'erreur REG_NOMATCH.
 *
 * Le but est precis : les en-tetes du moteur (regex.h, regex2.h, utils.h,
 * cclass.h, cname.h) ont ete reconstruits pour ce portage a partir des seuls
 * fichiers .c. Si une structure, un type ou une constante y est mal declare,
 * c'est ici que cela se voit - ailleurs, cela passerait pour un motif qui ne
 * correspond pas.
 *
 * Compilation : python build.py --target <cible> --re-engine.
 * Le programme rend 0 si tout est vert.
 */

#include <stdio.h>
#include <string.h>

#include "regex.h"

static int failures = 0;

/*
 * Correspondance etendue ou basic : compile le motif, cherche dans le texte, et
 * verifie le code de retour puis, si demande, la position de la correspondance
 * entiere et du premier groupe.
 */
static void
essai(const char *titre, const char *texte, const char *motif, int cflags,
      int attendu, int so_attendu, int eo_attendu,
      const char *groupe_attendu)
{
	regex_t reg;
	regmatch_t pm[10];
	char errbuf[200];
	size_t nmatch = 10;
	int code;
	int ok;
	char groupe[64];

	code = regcomp(&reg, motif, cflags);
	if (code) {
		regerror(code, &reg, errbuf, sizeof errbuf);
		printf("  ECHEC  %-34s compilation : %s\n", titre, errbuf);
		failures++;
		return;
	}

	memset(pm, 0, sizeof pm);
	code = regexec(&reg, texte, nmatch, pm, 0);

	ok = 1;
	groupe[0] = '\0';
	if (attendu == 0) {
		/* on attend une correspondance */
		if (code != 0) {
			ok = 0;
		} else {
			if (so_attendu >= 0 && (int)pm[0].rm_so != so_attendu)
				ok = 0;
			if (eo_attendu >= 0 && (int)pm[0].rm_eo != eo_attendu)
				ok = 0;
			if (groupe_attendu) {
				size_t n = (size_t)(pm[1].rm_eo - pm[1].rm_so);
				if (n >= sizeof groupe) n = sizeof groupe - 1;
				memcpy(groupe, texte + pm[1].rm_so, n);
				groupe[n] = '\0';
				if (strcmp(groupe, groupe_attendu) != 0)
					ok = 0;
			}
		}
	} else {
		/* on attend REG_NOMATCH */
		if (code != REG_NOMATCH)
			ok = 0;
	}

	if (ok) {
		printf("  OK     %-34s\n", titre);
	} else {
		printf("  ECHEC  %-34s attendu %s, obtenu ", titre,
		       attendu == 0 ? "une correspondance" : "REG_NOMATCH");
		if (code == 0)
			printf("correspondance [%d,%d] groupe [%s]\n",
			       (int)pm[0].rm_so, (int)pm[0].rm_eo, groupe);
		else
			printf("code %d\n", code);
		failures++;
	}

	regfree(&reg);
}

int
main(void)
{
	regex_t reg;
	regmatch_t pm[10];
	char errbuf[200];
	int code;

	printf("=== moteur re : essai direct en C (modes etendu et basic) ===\n\n");

	/* --- 1. mode etendu : groupes, repetition bornee, ancres --- */
	essai("etendu : date ISO", "2008-05-12",
	      "^([0-9]{4})-([0-9]{2})-([0-9]{2})$", REG_EXTENDED,
	      0, 0, 10, "2008");
	essai("etendu : date avec suffixe", "2008-05-12x",
	      "^([0-9]{4})-([0-9]{2})-([0-9]{2})$", REG_EXTENDED,
	      REG_NOMATCH, -1, -1, NULL);

	/* --- 2. REG_ICASE --- */
	essai("etendu : casse respectee", "ABC", "abc", REG_EXTENDED,
	      REG_NOMATCH, -1, -1, NULL);
	essai("etendu : REG_ICASE", "ABC", "abc", REG_EXTENDED | REG_ICASE,
	      0, 0, 3, NULL);

	/* --- 3. findmust : le motif doit contenir un litteral --- */
	essai("etendu : litteral obligatoire", "say hello there", "hello",
	      REG_EXTENDED, 0, 4, 9, NULL);
	essai("etendu : litteral absent", "say hallo there", "hello",
	      REG_EXTENDED, REG_NOMATCH, -1, -1, NULL);

	/* --- 4. table cnames (cname.h) : [.nom.] --- */
	essai("etendu : nom de caractere", "a b", "a[[.space.]]b", REG_EXTENDED,
	      0, 0, 3, NULL);

	/* --- 5. table cclasses (cclass.h) deja vue par le module, ici en
	 *        combinaison avec un groupe, pour verifier les longueurs --- */
	essai("etendu : classe et groupe", "abc123", "([[:alpha:]]+)([0-9]+)",
	      REG_EXTENDED, 0, 0, 6, "abc");

	/* --- 6. REG_NEWLINE : sans lui, le point franchit le saut de ligne
	 *        (c'est ce que dit POSIX : le point vaut tout caractere sauf NUL,
	 *        et le saut de ligne est exclu seulement si REG_NEWLINE est pose) */
	essai("etendu : point franchit le saut", "un\ndeux", "un.deux",
	      REG_EXTENDED, 0, 0, 7, NULL);
	essai("etendu : REG_NEWLINE, point borne", "un\ndeux", "un.deux",
	      REG_EXTENDED | REG_NEWLINE, REG_NOMATCH, -1, -1, NULL);
	essai("etendu : REG_NEWLINE", "un\ndeux", "^deux$",
	      REG_EXTENDED | REG_NEWLINE, 0, 3, 7, NULL);
	essai("etendu : sans REG_NEWLINE", "un\ndeux", "^deux$",
	      REG_EXTENDED, REG_NOMATCH, -1, -1, NULL);

	/* --- 7. mode basic : c'est ici que passent les references arriere,
	 *        donc dupl(), OBACK_ et O_BACK --- */
	essai("basic : reference arriere", "abab", "^\\(ab\\)\\1$", REG_BASIC,
	      0, 0, 4, "ab");
	essai("basic : reference arriere fausse", "abcd", "^\\(ab\\)\\1$",
	      REG_BASIC, REG_NOMATCH, -1, -1, NULL);
	essai("basic : groupe et etoile", "aab", "\\(a*\\)b", REG_BASIC,
	      0, 0, 3, "aa");

	/* --- 8. REG_PEND : le motif s'arrete a re_endp, pas au NUL ---
	 * C'est le protocole du module : re_endp est pose sur l'objet regex_t AVANT
	 * l'appel a regcomp, qui le lit pour borner le motif. Ici le motif fait
	 * trois caracteres alors que la zone en compte six : si la borne etait
	 * ignoree, le motif serait "abcZZZ" et ne correspondrait plus.
	 */
	{
		char zone[] = "abcZZZ";
		regex_t borne;

		memset(&borne, 0, sizeof borne);
		borne.re_endp = zone + 3;
		code = regcomp(&borne, zone, REG_EXTENDED | REG_PEND);
		if (code != 0) {
			regerror(code, &borne, errbuf, sizeof errbuf);
			printf("  ECHEC  %-34s compilation : %s\n",
			       "etendu : REG_PEND borne le motif", errbuf);
			failures++;
		} else {
			memset(pm, 0, sizeof pm);
			code = regexec(&borne, "xabcx", 10, pm, 0);
			if (code == 0 && pm[0].rm_so == 1 && pm[0].rm_eo == 4)
				printf("  OK     %-34s\n",
				       "etendu : REG_PEND borne le motif");
			else {
				printf("  ECHEC  %-34s code %d, [%d,%d]\n",
				       "etendu : REG_PEND borne le motif", code,
				       (int)pm[0].rm_so, (int)pm[0].rm_eo);
				failures++;
			}
			regfree(&borne);
		}
	}

	/* --- 9. motif invalide : regerror doit savoir en dire quelque chose --- */
	code = regcomp(&reg, "(abc", REG_EXTENDED);
	if (code == 0) {
		printf("  ECHEC  %-34s motif invalide accepte\n",
		       "erreur : parenthese non fermee");
		failures++;
		regfree(&reg);
	} else {
		errbuf[0] = '\0';
		regerror(code, &reg, errbuf, sizeof errbuf);
		if (errbuf[0] != '\0')
			printf("  OK     %-34s %s (code %d)\n",
			       "erreur : message par regerror", errbuf, code);
		else {
			printf("  ECHEC  %-34s message vide\n",
			       "erreur : message par regerror");
			failures++;
		}
	}

	printf("\nmoteur re : %s\n", failures ? "ECHEC" : "OK");
	return failures ? 1 : 0;
}
