/*-
 * Copyright (c) 1992, 1993, 1994 Henry Spencer.
 * Copyright (c) 1992, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Henry Spencer.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)regex.h	8.2 (Berkeley) 1/3/94
 */

/*
 * ---------------------------------------------------------------------------
 * ALTERED VERSION (2008 ScriptBasic, 2026 portage TinyCC).
 *
 * Fichier absent de la distribution : dans l'arbre d'origine il etait
 * *engendre* par l'outil `mkh`, qui recoltait les lignes `= ` commentees dans
 * les sources. Les quatre prototypes, les drapeaux de regcomp() et de
 * regexec() et les codes d'erreur ci-dessous sont repris *mot pour mot* de ces
 * lignes, restees dans regcomp.c, regexec.c, regerror.c et regfree.c ; seul
 * l'habillage (typedefs, garde d'inclusion, conventions d'appel) est de notre
 * fait. C'est aussi ce qui explique l'absence de correspondance exacte avec
 * <regex.h> de POSIX : les codes de drapeaux sont ceux de Spencer.
 *
 * Conformement a la clause 3 de la licence ci-dessus, ce fichier est marque
 * comme version modifiee.
 * ---------------------------------------------------------------------------
 */

#ifndef _REGEX_H_
#define	_REGEX_H_

#include <sys/types.h>		/* size_t */

/*
 * <assert.h> : les .c du moteur appellent assert() sans l'inclure (ils se
 * contentent de garder les appels derriere `#ifndef NDEBUG`).  Sans l'en-tete,
 * assert() n'est plus une macro mais une fonction implicite qui n'existe nulle
 * part : les controles disparaitraient silencieusement et le lien reclamerait
 * un symbole `assert`.  L'inclure ici, dans l'en-tete commun, reactive les
 * controles partout ou ils etaient prevus.
 */
#include <assert.h>

/* Conventions de prototypes a la mode de 1994 : les .c definissent les
 * fonctions en vieux style (K&R) et declarent par __P(). */
#ifndef	__P
#ifdef	__STDC__
#define	__P(x)	x
#else
#define	__P(x)	()
#endif
#endif

/* the types */
typedef struct {
	int re_magic;
	size_t re_nsub;		/* number of parenthesized subexpressions */
	const char *re_endp;	/* end pointer for REG_PEND, if used */
	struct re_guts *re_g;	/* none of your business :-) */
} regex_t;

/*
 * regoff_t : type des decalages.  engine.ci declare `register regoff_t offsave;`
 * pour sauvegarder un rm_so ; les deux doivent donc etre du meme type.  Un int
 * suffit aux chaines de ScriptBasic et reste signe, ce dont depend la
 * sentinelle -1 utilisee pour les sous-expressions sans correspondance.
 */
typedef int regoff_t;

typedef struct {
	regoff_t rm_so;		/* start of match, -1 if no match */
	regoff_t rm_eo;		/* end of match (one past last) */
} regmatch_t;

/* regcomp() flags */
#define	REG_BASIC	0000
#define	REG_EXTENDED	0001
#define	REG_ICASE	0002
#define	REG_NOSUB	0004
#define	REG_NEWLINE	0010
#define	REG_NOSPEC	0020
#define	REG_PEND	0040
#define	REG_DUMP	0200

/* regexec() flags */
#define	REG_NOTBOL	00001
#define	REG_NOTEOL	00002
#define	REG_STARTEND	00004
#define	REG_TRACE	00400	/* tracing of execution */
#define	REG_LARGE	01000	/* force large representation */
#define	REG_BACKR	02000	/* force use of backref code */

/* regcomp() / regexec() failure codes */
#define	REG_NOMATCH	 1
#define	REG_BADPAT	 2
#define	REG_ECOLLATE	 3
#define	REG_ECTYPE	 4
#define	REG_EESCAPE	 5
#define	REG_ESUBREG	 6
#define	REG_EBRACK	 7
#define	REG_EPAREN	 8
#define	REG_EBRACE	 9
#define	REG_BADBR	10
#define	REG_ERANGE	11
#define	REG_ESPACE	12
#define	REG_BADRPT	13
#define	REG_EMPTY	14
#define	REG_ASSERT	15
#define	REG_INVARG	16
#define	REG_ATOI	255	/* convert name to number (!) */
#define	REG_ITOA	0400	/* convert number to name (!) */

/* L'appel de convention __stdcall est celui des .c de ScriptBasic (les
 * definitions portent `int __stdcall`), c'est donc lui que l'editeur de liens
 * attend ici. Il n'est pas redefini : sous Windows c'est un mot-cle. */

extern int	__stdcall regcomp __P((regex_t *, const char *, int));
extern int	__stdcall regexec __P((const regex_t *, const char *, size_t,
				       regmatch_t [], int));
extern size_t	__stdcall regerror __P((int, const regex_t *, char *, size_t));
extern void	__stdcall regfree __P((regex_t *));

#endif /* !_REGEX_H_ */
