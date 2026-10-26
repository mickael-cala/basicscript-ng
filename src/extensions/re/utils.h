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
 *	@(#)utils.h	8.3 (Berkeley) 3/20/94
 */

/*
 * ---------------------------------------------------------------------------
 * ALTERED VERSION (2008 ScriptBasic, 2026 portage TinyCC).
 *
 * Ce fichier ne figure pas dans la distribution de l'extension : seuls les
 * .c, engine.ci et les tables de documentation y sont. Il a ete reconstitue
 * pour le portage a partir des seuls consommateurs, conformement a la clause 3
 * de la licence ci-dessus : utils.h n'est inclus que par regcomp.c, regexec.c,
 * regerror.c et regfree.c, et le contenu ci-dessous se limite a ce que ces
 * quatre fichiers emploient reellement.
 * ---------------------------------------------------------------------------
 */

/* utility definitions */

/*
 * Bornes de repetition.  _POSIX2_RE_DUP_MAX vaut 255 la ou il est defini ;
 * DUPMAX borne les {,n} et INFINITY marque une borne haute absente.
 */
#ifdef _POSIX2_RE_DUP_MAX
#define	DUPMAX	_POSIX2_RE_DUP_MAX
#else
#define	DUPMAX	255
#endif
#define	INFINITY	(DUPMAX + 1)

/* nombre de valeurs distinctes d'un char : tout le moteur est indexe
 * de 0 a NC-1, caractere reinterpret e par (uch). */
#define	NC		(CHAR_MAX - CHAR_MIN + 1)

/* un caractere, vu comme un numero de 0 a 255 */
typedef unsigned char uch;

/* numero de categorie d'equivalence (voir struct re_guts) */
typedef unsigned char cat_t;

/*
 * Code superieur a tout caractere, utilise par le compilateur comme
 * sentinelle de fin d'expression : regcomp.c appelle p_ere(p, OUT).
 */
#define	OUT		(CHAR_MAX + 1)
