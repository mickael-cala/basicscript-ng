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
 *	@(#)regex2.h	8.4 (Berkeley) 3/20/94
 */

/*
 * ---------------------------------------------------------------------------
 * ALTERED VERSION (2008 ScriptBasic, 2026 portage TinyCC).
 *
 * Fichier absent de la distribution, reconstitue pour le portage a partir des
 * consommateurs (regcomp.c, regexec.c, regfree.c et engine.ci), conformement a
 * la clause 3 de la licence ci-dessus.
 *
 * Le codage retenu est celui de la generation 4.4BSD : un operateur et son
 * operande tiennent dans UN mot (`sop`), l'operateur dans les bits hauts et
 * l'operande dans les bits bas. C'est ce que le code impose : regcomp.c borne
 * l'operande par `assert(opnd < 1<<OPSHIFT)` (doemit, dofwd) et engine.ci
 * saute d'un operateur a l'autre par `es += OPND(m->g->strip[es])`. Le
 * mecanisme des categories, lui, vient de la generation precedente : regcomp.c
 * s'en sert largement (categorize, ordinary) et le champ catspace doit donc
 * exister dans struct re_guts.
 * ---------------------------------------------------------------------------
 */

/*
 * internals of regex_t
 */
#define	MAGIC1	((('r'^0200)<<8) | 'e')

/*
 * The internal representation is a *strip*, a sequence of operators ending
 * with an endmarker.  (Some terminology etc. is a historical relic of earlier
 * versions which used multiple strips.)  Certain oddities in the
 * representation are there to permit running the machinery backwards; in
 * particular, any deviation from sequential flow must be marked at both its
 * source and its destination.  Some fine points:
 *
 * - OPLUS_ and O_PLUS are *inside* the loop they create.
 * - OQUEST_ and O_QUEST are *outside* the bypass they create.
 * - OCH_ and O_CH are *outside* the multi-way branch they create, while
 *   OOR1 and OOR2 are respectively the end and the beginning of one of the
 *   branches.  Note that there is an implicit OOR2 following OCH_ and an
 *   implicit OOR1 preceding O_CH.
 *
 * In state representations, an operator's bit is on to signify a state
 * immediately *preceding* "execution" of that operator.
 */
typedef unsigned long sop;	/* strip operator */
typedef long sopno;
#define	OPRMASK	0xf8000000L
#define	OPDMASK	0x07ffffffL
#define	OPSHIFT	((unsigned)27)
#define	OP(n)	((n)&OPRMASK)
#define	OPND(n)	((n)&OPDMASK)
#define	SOP(op, opnd)	((op)|(opnd))
/* operators			   meaning	operand			*/
/*					(back, fwd are offsets)	*/
#define	OEND	(1L<<OPSHIFT)	/* endmarker	-			*/
#define	OCHAR	(2L<<OPSHIFT)	/* character	unsigned char		*/
#define	OBOL	(3L<<OPSHIFT)	/* left anchor	-			*/
#define	OEOL	(4L<<OPSHIFT)	/* right anchor	-			*/
#define	OANY	(5L<<OPSHIFT)	/* .		-			*/
#define	OANYOF	(6L<<OPSHIFT)	/* [...]	set number		*/
#define	OBACK_	(7L<<OPSHIFT)	/* begin \d	paren number		*/
#define	O_BACK	(8L<<OPSHIFT)	/* end \d	paren number		*/
#define	OPLUS_	(9L<<OPSHIFT)	/* + prefix	fwd to suffix		*/
#define	O_PLUS	(10L<<OPSHIFT)	/* + suffix	back to prefix		*/
#define	OQUEST_	(11L<<OPSHIFT)	/* ? prefix	fwd to suffix		*/
#define	O_QUEST	(12L<<OPSHIFT)	/* ? suffix	back to prefix		*/
#define	OLPAREN	(13L<<OPSHIFT)	/* (		fwd to )		*/
#define	ORPAREN	(14L<<OPSHIFT)	/* )		back to (		*/
#define	OCH_	(15L<<OPSHIFT)	/* begin choice	fwd to OOR2		*/
#define	OOR1	(16L<<OPSHIFT)	/* | pt. 1	back to OOR1 or OCH_	*/
#define	OOR2	(17L<<OPSHIFT)	/* | pt. 2	fwd to OOR2 or O_CH	*/
#define	O_CH	(18L<<OPSHIFT)	/* end choice	back to OOR1		*/
#define	OBOW	(19L<<OPSHIFT)	/* begin word	-			*/
#define	OEOW	(20L<<OPSHIFT)	/* end word	-			*/

/*
 * Structure for [] character-set representation.  Character sets are done as
 * bit vectors, grouped 8 to a byte vector for compactness.  The individual set
 * therefore has both a pointer to the byte vector and a mask to pick out the
 * relevant bit of each byte.  A hash code simplifies testing whether two sets
 * could be identical.  The hash is the sum of the member characters, not their
 * xor: see the comment on freezeset() in regcomp.c, [aA] sets would otherwise
 * collide under REG_ICASE.
 *
 * As preliminary hooks for multicharacter collating elements, a string of
 * multi-character elements is carried along, and the size of the vectors is
 * decided at run time.
 */
typedef struct {
	uch *ptr;		/* -> uch [csetsize] */
	uch mask;		/* bit within array */
	uch hash;		/* hash code */
	size_t smultis;
	char *multis;		/* -> char[smulti]  ab\0cd\0ef\0\0 */
} cset;
/* note that CHadd and CHsub are unsafe if the arguments are evaluated more
 * than once, so they must not be called with expressions that have side
 * effects */
#define	CHIN(cs, ch)	(((cs)->ptr[(uch)(ch)] & (cs)->mask) != 0)
#define	CHadd(cs, ch)	((cs)->ptr[(uch)(ch)] |= (cs)->mask, (cs)->hash += (uch)(ch))
#define	CHsub(cs, ch)	((cs)->ptr[(uch)(ch)] &= ~(cs)->mask, (cs)->hash -= (uch)(ch))

/*
 * main compiled-expression structure
 */
struct re_guts {
	int magic;
#		define	MAGIC2	((('R'^0200)<<8)|'E')
	sop *strip;		/* malloced area for strip */
	int csetsize;		/* number of bits in a cset vector */
	int ncsets;		/* number of csets in use */
	cset *sets;		/* -> cset [ncsets] */
	uch *setbits;		/* -> uch[csetsize][ncsets/CHAR_BIT] */
	int cflags;		/* copy of regcomp() cflags argument */
	sopno nstates;		/* = number of sops */
	sopno firststate;	/* the initial OEND (normally 0) */
	sopno laststate;	/* the final OEND */
	int iflags;		/* internal flags */
#		define	USEBOL	01	/* used ^ */
#		define	USEEOL	02	/* used $ */
#		define	BAD	04	/* something wrong */
	int nbol;		/* number of ^ used */
	int neol;		/* number of $ used */
	char *must;		/* match must contain this string */
	int mlen;		/* length of must */
	size_t nsub;		/* copy of re_nsub */
	int backrefs;		/* does it use back references? */
	sopno nplus;		/* how deep does it nest +s? */
/* the following are new in alpha3.2, and used by categorize() in regcomp.c */
	cat_t *categories;	/* -> catspace[-(CHAR_MIN)] */
	int ncategories;	/* number of categories in use */
	cat_t catspace[1];	/* actually longer: NC entries, allocated with
				 * the struct itself */
};

/*
 * Les fonctions internes de regcomp.c qui manipulent les elements
 * multi-caracteres sont definies en minuscules et appelees en CamelCase :
 * c'est le mecanisme par lequel ces fonctions privees pouvaient etre renommees
 * par l'outil mkh.  Seul MCadd est employe par la copie de regcomp.c livree
 * avec ScriptBasic (p_b_cclass) ; les cinq autres sont donnes pour que la
 * correspondance soit complete si la source evolue.
 */
#define	MCadd(p, cs, cp)	mcadd(p, cs, cp)
#define	MCsub(cs, cp)		mcsub(cs, cp)
#define	MCin(cs, cp)		mcin(cs, cp)
#define	MCfind(cs, cp)		mcfind(cs, cp)
#define	MCinvert(p, cs)		mcinvert(p, cs)
#define	MCcase(p, cs)		mccase(p, cs)

/* misc utilities */
/* OUT (la valeur non-caractere utilisee comme sentinelle) est defini par
 * utils.h, toujours inclus avant ce fichier. */
#define	ISWORD(c)	(isalnum((uch)(c)) || (c) == '_')
