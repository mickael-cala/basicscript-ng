/*

--GNU LGPL
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

#include "errcodes.h"
#include "report.h"

/* public declarations of this module */
#include "lexer.h"
#include "ipreproc.h"

/*POD

This module contains the functions and structures that are used by ScriptBasic to perform
lexical analysis of the source code. The module was originally developed for ScriptBasic
but was developed to be general enough to be used in other projects.

CUT*/
#define BUFFERINCREASE 1024

#if (!defined(_WIN32) && !defined(__MACOS__))
int stricmp(char *,char*);
#endif

#define REPORT(x1,x2,x3,x4) if( pLex->report )pLex->report(pLex->reportptr,x1,x2,x3,REPORT_ERROR,&(pLex->iErrorCounter),x4,&(pLex->fErrorFlags))

#define CALL_PREPROCESSOR(X,Y) if( pLex->pPREP && pLex->pPREP->n )ipreproc_Process(pLex->pPREP,X,Y)

static int isinset(int ch,char *string){
   while( ch != *string && *++string );
   return *string;
}

static double pow10(double a)
{
   int j,i;
   double pro,k;

   for( (i= a<0.0) && (a = -a) , j=(int)a , pro=1.0 , k=10; j ;
       j%2 && (pro *=k) , j /= 2 , k *= k )
      continue;
   i && (pro=1.0/pro);
   return pro;
}

static int __GETC(int (*pfGetCharacter)(void *),
                void *pvInput,
                int *UngetBuffer,
                int *UngetCounter
               ){
  if( *UngetCounter ){
    (*UngetCounter) --;
    return UngetBuffer[*UngetCounter];
    }
  return pfGetCharacter(pvInput);
  }

static void __UNGETC(int *UngetBuffer,
              int *UngetCounter,
              int ch
             ){
  UngetBuffer[(*UngetCounter)++] = ch;
  }

/* these macroes help to call the local get and unget functions */
/* note that these macros are safe and should be (i.e.: UNGETC(buffer[i--]) )*/
#define GETC() __GETC(pLex->pfGetCharacter,pLex->pvInput,UngetBuffer,&UngetCounter)
#define UNGETC(x) __UNGETC(UngetBuffer,&UngetCounter,x)
#define UNGET_BUFFER_LENGTH 10 /* more than enough */

#define lexALLOC(x) (pLex->memory_allocating_function((x),pLex->pMemorySegment))
#define lexFREE(x) (pLex->memory_releasing_function((x),pLex->pMemorySegment),(x)=NULL)

/* allocate a new lexeme safely initialized */
static pLexeme _NewLexeme(pLexObject pLex){
  pLexeme p;
  p = (pLexeme)lexALLOC(sizeof(Lexeme));
  if( p ){
    p->sLen = 0;
    p->value.lValue = 0;
    p->szFileName = NULL;
    p->lLineNumber = 0;
    p->next = NULL;
    }
  return p;
  }
#define NewLexeme() _NewLexeme(pLex)

/*POD
=H lex_SymbolicName()

This function usually is for debug purposes. This searches the
table of the predefined symbols and returns the string which is
the predefined symbols for which the code was passsed.
/*FUNCTION*/
char *lex_SymbolicName(pLexObject pLex,
                       long OpCode
  ){
/*noverbatim
CUT*/
  pLexNASymbol p;

  p = pLex->pNASymbols;
  while( p->Symbol && p->Code != OpCode )
    p++;
  if( p->Symbol )return p->Symbol;

  p = pLex->pASymbols;
  while( p->Symbol && p->Code != OpCode )
    p++;
  if( p->Symbol )return p->Symbol;

  p = pLex->pCSymbols;
  while( p->Symbol && p->Code != OpCode )
    p++;
  if( p->Symbol )return p->Symbol;

  return "INTERNAL ERROR";
  }

/*POD
=H lex_HandleContinuationLines()

This function is called from the main function before syntax analysis is started. 
This function handles the usual basic continuation lines. If the last character on a 
line is a _ character, which is either recognised during lexical analysis as a 
character or as a symbol then this lexical element and the following new-line character
token is removed from the list of tokens.

/*FUNCTION*/
void lex_HandleContinuationLines(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLexeme *p,r;

  p = &(pLex->pLexResult);
  while( *p ){
    if( *p && (
        ((*p)->type == LEX_T_CHARACTER && (*p)->value.lValue == '_')
                           ||
        ((*p)->type == LEX_T_ASYMBOL && *(*p)->value.sValue == '_' && (*p)->value.sValue[1] == (char)0)
        )
        && (*p)->next && (*p)->next->type == LEX_T_CHARACTER && (*p)->next->value.lValue == '\n' ){
      r = *p;
      *p = (*p)->next->next;
      lexFREE(r->next);
      lexFREE(r);
      }else
      p = &((*p)->next);
    }
  }

/*POD
=H lex_RemoveSkipSymbols()

This function is called from R<lex_DoLexicalAnalysis()> to remove the lexical elements
from the list of tokens that were denoted by the preprocessors to be deleted.

Some lexical elements are used to give information to some of the preprocessors. These
tokens should be deleted, because later processing can not deal with them and confuses syntax
analysis.

In those cases the preprocessor should set the type of the token to be LT<LEX_T_SKIP> or
T<LEX_T_SKIP_SYMBOL>. The type T<LEX_T_SKIP> should be used in case the token is handled due to
T<ProcessLexSymbol> preprocessor command and T<LEX_T_SKIP> otherwise.

When the type is set T<LEX_T_SKIP_SYMBOL> the lexical analyzer knows to release the string holding
the symbol. If the type is T<LEX_T_SKIP> only the token record is released.

If the symbol string is not released due to erroneously setting the type to T<LEX_T_SKIP> instead
T<LEX_T_SKIP_SYMBOL> the memory will not be released until the interpreter finishes pre execution
steps. So usually if you do not know how to set the type to skip a token T<LEX_T_SKIP> is safe.

/*FUNCTION*/
void lex_RemoveSkipSymbols(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLexeme *p,r;

  p = &(pLex->pLexResult);
  while( *p ){
    if( *p && ((*p)->type == LEX_T_SKIP || (*p)->type == LEX_T_SKIP_SYMBOL)){
      r = *p;
      *p = (*p)->next;
      if( (*p)->type == LEX_T_SKIP_SYMBOL )lexFREE(r->value.sValue);
      lexFREE(r);
      }else
      p = &((*p)->next);
    }
  }

/*POD
=H lex_RemoveComments()

This function called from the function R<lex_DoLexicalAnalysis()> function to remove the comments before the 
syntax analysis starts.

It should be called before calling the continuation line handling because usually REM
lines are not continuable
/*FUNCTION*/
void lex_RemoveComments(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLexeme *p,r,q,*w;

  p = &(pLex->pLexResult);
  while( *p ){
    if( ((*p)->type == LEX_T_ASYMBOL && !stricmp((*p)->value.sValue,"rem") )   ||
        ((*p)->type == LEX_T_CHARACTER && (*p)->value.lValue == '\''       )   ||
        ((*p)->type == LEX_T_ASYMBOL && *(*p)->value.sValue == '\'' && (*p)->value.sValue[1] == (char)0)
      ){
      r = (*p);
      w = p;
      while( (*p) && ( (*p)->type != LEX_T_CHARACTER || (*p)->value.lValue != '\n') )
        p=&((*p)->next);
      if( *p )p=&((*p)->next);
      *w = *p;
      p = w;
      while( r && r->next && (r->next->type != LEX_T_CHARACTER || r->next->value.lValue != '\n') ){
        q=r;
        r = r->next;
        lexFREE(q);
        }
      }else{
      while( (*p) && (*p)->type != LEX_T_CHARACTER && (*p)->value.lValue != '\n' )p=&((*p)->next);
      if( *p )p=&((*p)->next);
      }
    }
  }

/*POD
=H lex_NextLexeme()

Use this function during iteration to get the next lexeme from the list of lexemes.

/*FUNCTION*/
void lex_NextLexeme(pLexObject pLex
  ){
/*noverbatim
CUT*/
  if( pLex->pLexCurrentLexeme )/* allow sloppy call at the end of the file */
    pLex->pLexCurrentLexeme = pLex->pLexCurrentLexeme->next;
  }

/*POD
=H lex_SavePosition()

Use this function to save the current position of the iteration. This is neccessary during
syntactical analysis to return to a certain position when syntactical analysis fails and
the program has to go back and try a different command syntax.

/*FUNCTION*/
void lex_SavePosition(pLexObject pLex,
                      pLexeme *ppPosition
  ){
/*noverbatim

The second argument is a T<pLexeme *> type variable that holds the position and should be passed
as argument to the function R<lex_RestorePosition()>.
CUT*/
  *ppPosition = pLex->pLexCurrentLexeme;
  }

/*POD
=H lex_RestorePosition()

Use this function to restore the lexeme position that was saved calling the function R<lex_SavePosition()>

/*FUNCTION*/
void lex_RestorePosition(pLexObject pLex,
                         pLexeme *ppPosition
  ){
/*noverbatim
CUT*/
  pLex->pLexCurrentLexeme = *ppPosition;
  }

/*POD
=H lex_StartIteration()

You should call this function when the list of lexemes was built up before starting the iteration
of the syntax analyzer. This function sets the iteration pointer to the first lexeme.

/*FUNCTION*/
void lex_StartIteration(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLex->pLexCurrentLexeme = pLex->pLexResult;
  }

/*POD
=H lex_EOF()

Call this function to check if the iteration has reached the last lexeme.

/*FUNCTION*/
int lex_EOF(pLexObject pLex
  ){
/*noverbatim
CUT*/
  return pLex->pLexCurrentLexeme == NULL;
  }

/*POD
=H lex_Type()

During lexeme iteration this function can be used to retrieve the typeof the current lexeme. The
type of a lexeme can be:

=itemize
=item T<LEX_T_DOUBLE> a double value. A number which is not integer.
=item T<LEX_T_LONG> an long value. A number which is integer.
=item T<LEX_T_STRING> a string.
=item T<LEX_T_ASYMBOL> an alpha symbol, like a variable. This symbol is not predefined. The value
of the lexeme is the string of the symbol.
=item T<LEX_T_NSYMBOL> a predefined symbol. The actual value of the lexeme is the token value
of the symbol. If you wan to get the actual string of the symbol you have to call the function
R<lex_SymbolicName()>.
=item T<LEX_T_CHARACTER> A character that is not a predefined symbol and does not fit into any string.
=noitemize

/*FUNCTION*/
int lex_Type(pLexObject pLex
  ){
/*noverbatim
CUT*/
  return pLex->pLexCurrentLexeme->type;
  }

/*POD
=H lex_Double()

When the type of the current lexeme is T<LEX_T_DOUBLE> during the lexeme iteration
this function should be used to retrieve the actual value of the current lexeme.

/*FUNCTION*/
double lex_Double(pLexObject pLex
/*noverbatim
CUT*/
  ){
  return pLex->pLexCurrentLexeme->value.dValue;
  }

/*POD
=H lex_String()

When the type of the current lexeme is T<LEX_T_STRING> during the lexeme iteration
this function should be used to retrieve the actual value of the current lexeme.
/*FUNCTION*/
char *lex_String(pLexObject pLex
  ){
/*noverbatim
CUT*/
  return pLex->pLexCurrentLexeme->value.sValue;
  }

/*POD
=H lex_StrLen()

When the type of the current lexeme is T<LEX_T_STRING> during the lexeme iteration
this function should be used to retrieve the length of the current lexeme. This is
more accurate than calling T<strlen> on the actual string because the string itself may
contain zero characters.
/*FUNCTION*/
long lex_StrLen(pLexObject pLex
  ){
/*noverbatim
CUT*/
  return pLex->pLexCurrentLexeme->sLen;
  }

/*POD
=H lex_Long()

When the type of the current lexeme is T<LEX_T_LONG> during the lexeme iteration
this function should be used to retrieve the actual value of the current lexeme.

/*FUNCTION*/
long lex_Long(pLexObject pLex
  ){
/*noverbatim
CUT*/
  return pLex->pLexCurrentLexeme->value.lValue;
  }

/*POD
=H lex_LineNumber()

This function returns the line number that the actual lexeme is in the source file.
This function is needed to print out syntax and lexical error messages.

See also R<lex_FileName()>.
/*FUNCTION*/
long lex_LineNumber(pLexObject pLex
  ){
/*noverbatim
CUT*/
  if( pLex->pLexCurrentLexeme )
    return pLex->pLexCurrentLexeme->lLineNumber;
  return 0L;
  }

/*POD
=H lex_FileName()

This function returns a pointer to a constant string which is the file name
that the lexeme was read from. Use this function to print out error messages when
syntax or lexical error occures.

See also R<lex_LineNumber()>.
/*FUNCTION*/
char *lex_FileName(pLexObject pLex
  ){
/*noverbatim
CUT*/
  if( pLex->pLexCurrentLexeme )
    return pLex->pLexCurrentLexeme->szFileName;
  return "";
  }

/*POD
=H lex_XXX()

These access functions are implemented as macros and are declared in <lexer.h>

The macros access T<Int>, T<Symbol>, T<Float> etc values of the current lexeme. However these are strored
in a location which is named a bit different. For example the string of a symbol is stored in the string
field of the lexeme. To be readable and to be compatible with future versions use these macros to access
lexeme values when lexeme has any of these types.

=verbatim
/*
*/
/*
=noverbatim
CUT*/

/*POD
=H lex_Finish()

Call this functionto release all memory allocated by the lexical analyzer.

/*FUNCTION*/
void lex_Finish(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLexeme p,r;

  p = pLex->pLexResult;
  while( p ){
    r = p->next;
    switch( p->type ){
      case LEX_T_STRING:
      case LEX_T_ASYMBOL:
        lexFREE(p->value.sValue);
      case LEX_T_DOUBLE:
      case LEX_T_NSYMBOL:
      case LEX_T_CHARACTER:
      case LEX_T_LONG:
        lexFREE(p);
      }
    p = r;
    }
  }

/*POD
=H lex_DumpLexemes()

Us this function for debugging. This function dumps the list of lexemes to the file T<psDump>.

/*FUNCTION*/
void lex_DumpLexemes(pLexObject pLex,
                     FILE *psDump
  ){
/*noverbatim
CUT*/

  lex_StartIteration(pLex);
  while( !lex_EOF(pLex) ){
    fprintf(psDump,"%s(%ld) ",pLex->pLexCurrentLexeme->szFileName,pLex->pLexCurrentLexeme->lLineNumber);
    switch( lex_Type(pLex) ){
      case LEX_T_DOUBLE:
        fprintf(psDump,"Double %f\n",lex_Double(pLex));
        break;
      case LEX_T_LONG:
        fprintf(psDump,"Long %d\n",lex_Long(pLex));
        break;
      case LEX_T_STRING:
        fprintf(psDump,"String %s\n",lex_String(pLex));
        break;
      case LEX_T_ASYMBOL:
        fprintf(psDump,"Symbol %s\n",lex_Symbol(pLex));
        break;
      case LEX_T_NSYMBOL:
        fprintf(psDump,"NAsymb %d\n",lex_Token(pLex));
        break;
      case LEX_T_CHARACTER:
        if( lex_Char(pLex) == '\n' ){
          fprintf(psDump,"Charac NewLine\n");
          break;
          }
        if( lex_Char(pLex) == '\t' ){
          fprintf(psDump,"Charac Tab\n");
          break;
          }
        if( lex_Char(pLex) == '\r' ){
          fprintf(psDump,"Charac CR\n");
          break;
          }
        fprintf(psDump,"Charac %c\n",lex_Char(pLex));
        break;
       }
    lex_NextLexeme(pLex);
    }
  }

static void lex_StoreCharacter(pLexObject pLex, int ch, int i){
  char *s,*r;
  int NewBufferSize;

  while( i >= pLex->cbBuffer-1 ){
    NewBufferSize = BUFFERINCREASE + pLex->cbBuffer;
    s = lexALLOC(NewBufferSize*sizeof(char));
    if( s == NULL ){
      pLex->report(pLex->reportptr,
                   pLex->pfFileName(pLex->pvInput),
                   pLex->pfLineNumber(pLex->pvInput),
                   LEX_ERROR_MEMORY_LOW,
                   REPORT_ERROR,&(pLex->iErrorCounter),
                   pLex->buffer,&(pLex->fErrorFlags));
    return;
    }
    memcpy(s,pLex->buffer,pLex->cbBuffer);
    pLex->cbBuffer = NewBufferSize;
    r = pLex->buffer;
    pLex->buffer = s;
    lexFREE(r);
    }
  pLex->buffer[i] = ch;
  }

/*POD
=H lex_ReadInput()

Call this function after proper initialization to read the input file. This function performs the laxical analysis and
builds up an internal linked list that contains the lexemes.

/*FUNCTION*/
int lex_ReadInput(pLexObject pLex
  ){
/*noverbatim
CUT*/
  double intpart,fracpart,exppart,man;
  long hintpart; /* for hexadecimal calculation */
  long long llint; /* phase 8.2 : accumulation exacte des litteraux entiers */
  int deborde;
  int esig,ibase;
  int nch,ch,i,cStringStartCharacter,iErrorWasReported;
  pLexeme *plexLastLexeme;
  int UngetCounter,StringIsBinary;
  int *UngetBuffer;
  char *s;
  char *pszFileName;
  long lLineNumber;
  pLexNASymbol pNA;
  int iResult;

  UngetCounter = 0; /* init the local unget counter */
  /* allocate unget size which is capable ungetting all symbol characters if a "symbol" contains only
     characters that can not be final character */
  UngetBuffer = (int *)lexALLOC(pLex->cbBuffer*sizeof(int));

  pNA = pLex->pNASymbols;

  if( ! pNA )pLex->cbNASymbolLength = 1;

  /* calculate the length of the longest non-alphanumeric symbol length */
  if ( !pLex->cbNASymbolLength )
    while( pNA->Symbol ){
      if( (i=strlen(pNA->Symbol)) > pLex->cbNASymbolLength )
        pLex->cbNASymbolLength = i;
      pNA++;
      }

  pLex->cbNASymbolLength++; /* count the trailing zchar */

/* for readability  we define this 'variable' */
#define pfLexeme (*plexLastLexeme)

  pLex->pLexResult = NULL;
  plexLastLexeme = &(pLex->pLexResult);
  while( (ch = GETC()) != EOF ){

    /*** Skip spaces if we do not care spaces ***/
    while( isinset(ch,pLex->SKIP) )ch = GETC();

    pszFileName = pLex->pfFileName(pLex->pvInput);
    lLineNumber = pLex->pfLineNumber(pLex->pvInput);

    /*** PROCESS A SYMBOL ***/

    if( isinset(ch,pLex->SSC) ){
      for( i = 0 ; i < pLex->cbBuffer-1 && isinset(ch,pLex->SCC) ;i++ )
        (pLex->buffer[i] = ch) , (ch = GETC());
      if( i == pLex->cbBuffer-1 ){
         pLex->buffer[pLex->cbBuffer-1] = (char)0;
         while( isinset(ch,pLex->SCC) )ch = GETC();
         REPORT(pszFileName,lLineNumber,LEX_ERROR_TOO_LONG_SYMBOL,pLex->buffer);
         }
      UNGETC(ch);
      while( i && ! isinset(pLex->buffer[i-1],pLex->SFC) ){
        i--;
        UNGETC(pLex->buffer[i]);
        }
      if( i ){/* if there remained at least one characters in the symbol after
                 ungetching the final characters that should not appear
                 as final character */
        pLex->buffer[i++]=(char)0;

        /* try to look up the symbol in the alpha symbol list */
        if( pNA = pLex->pASymbols ){
          while( pNA->Symbol && 
                  ( (pLex->fFlag&LEX_ASYMBOL_CASE_SENSITIVE) ? 
                                     strcmp(pLex->buffer,pNA->Symbol) : 
                                     stricmp(pLex->buffer,pNA->Symbol) )
            ) pNA++;
          if( pNA->Symbol ){/* symbol is found */
            if( (pfLexeme = NewLexeme()) == NULL ){
              lexFREE(UngetBuffer);
              return LEX_ERROR_MEMORY_LOW;
              }
            pfLexeme->type = LEX_T_NSYMBOL;
            pfLexeme->value.lValue = pNA->Code;
            pfLexeme->lLineNumber = lLineNumber;
            pfLexeme->szFileName = pszFileName;
            if( pLex->pPREP && pLex->pPREP->n ){
              iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexASymbol,&plexLastLexeme);
              if( iResult )return iResult;
              }
            plexLastLexeme = &(pfLexeme->next);
            continue;
            }
          }

        if( (pfLexeme = NewLexeme()) == NULL ){
          lexFREE(UngetBuffer);
          return LEX_ERROR_MEMORY_LOW;
          }
        pfLexeme->next = NULL;
        pfLexeme->type = LEX_T_ASYMBOL;
        pfLexeme->lLineNumber = lLineNumber;
        pfLexeme->szFileName = pszFileName;
        if( (pfLexeme->value.sValue = (char *)lexALLOC(i)) == NULL ){
          lexFREE(UngetBuffer);
          return LEX_ERROR_MEMORY_LOW;
          }
        strcpy(pfLexeme->value.sValue,pLex->buffer);
        if( pLex->pPREP && pLex->pPREP->n ){
          iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexSymbol,&plexLastLexeme);
          if( iResult )return iResult;
          }
        plexLastLexeme = &(pfLexeme->next);
        continue;
        }else{
        ch = GETC(); /* we have ungetched the last character when we checked the last character of the symbol */
        }
      }
/* Store a string if there is space and report error at first occasion when string is too long. */
#define STORE_CH(x) lex_StoreCharacter(pLex,x,i)

    /*** PROCESS A STRING ***/

    if( isinset(ch,pLex->SStC) ){
      cStringStartCharacter = ch;
      ch = GETC();
      if( ch != cStringStartCharacter ){
        UNGETC(ch);
        ch = cStringStartCharacter;
        goto SimpleString;
        }
      nch = GETC();
      if( nch != cStringStartCharacter ){
        UNGETC(nch);
        UNGETC(ch);
        ch = cStringStartCharacter;
        goto SimpleString;
        }
      /* PROCESS A MULTILINE STRING */
      i = 1;
      ch = GETC();
      if( StringIsBinary = (ch == '&') )ch = GETC();
      iErrorWasReported = 0;
      for( i = 0 ; ch != EOF ; i++ ){
        while( StringIsBinary && ch == '\n' && ch != EOF )ch = GETC();
        if( ch == EOF )break;
        if( ch == cStringStartCharacter ){
          ch = GETC();
          if( ch == cStringStartCharacter ){
            nch = GETC();
            if( nch == cStringStartCharacter )break;
            UNGETC(nch);
            }
          UNGETC(ch);
          ch = cStringStartCharacter;
          }
        if( ch == *(pLex->ESCS) ){
          ch = GETC();
          
          if( ((pLex->fFlag&LEX_PROCESS_STRING_NUMBER) && isdigit(ch)) ||
              ((pLex->fFlag&LEX_PROCESS_STRING_NUMBER) &&  (pLex->fFlag&LEX_PROCESS_STRING_HEX_NUMBER) && ch == 'x') ){
            esig = 0;
            /* choose the radix */
            if( ch == 'x' ){ch = GETC();ibase = 16; }else
              if( (pLex->fFlag&LEX_PROCESS_STRING_OCTAL_NUMBER) && ch == '0' )ibase = 8; 
                else ibase = 10;

            while( isdigit(ch) || (ibase == 16 && isinset(ch,"0123456789ABCDEFabcdef")) ){
              esig = ibase*esig + ch;
              if( isdigit(ch) )
                esig -= '0';
              else
                if( islower(ch) )
                  esig -= 'a'-10;
                else
                  esig -= 'A'-10;

              ch = GETC();
              }
            STORE_CH(esig);
            continue;
            }

          s = pLex->ESCS+1;
          while( *s && *s != ch )s+=2;
          if( *s ){
            /* if there is a replacement character use that */
            s++;
            STORE_CH(*s);
            ch = GETC();
            continue;
            }else{
            /* if there is no other character for this to be replaced use the original */
            STORE_CH(ch);
            ch = GETC();
            continue;
            }
          }/* End handling escape character. */
        STORE_CH(ch);
        ch = GETC();
        }/* End of string building. */
      if( ch == EOF ){/* string was not terminated before EOF */
        REPORT(pszFileName,
               lLineNumber,
               LEX_ERROR_STRING_NOT_TERMINATED,
               pLex->buffer);
        }
      /* to be safe we store the terminating zero character */
      STORE_CH( (char)0 );
      i++;
      if( (pfLexeme = NewLexeme()) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      pfLexeme->next = NULL;
      pfLexeme->type = LEX_T_STRING;
      pfLexeme->lLineNumber = lLineNumber;
      pfLexeme->szFileName = pszFileName;
      pfLexeme->sLen = i-1; /* do not count the terminating zero */
      if( (pfLexeme->value.sValue = (char *)lexALLOC(i)) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      memcpy(pfLexeme->value.sValue,pLex->buffer,i);
      if( pLex->pPREP && pLex->pPREP->n ){
        iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexString,&plexLastLexeme);
        if( iResult )return iResult;
        }
      plexLastLexeme = &(pfLexeme->next);
      continue;
      }

SimpleString:
    if( isinset(ch,pLex->SStC) ){
      cStringStartCharacter = ch;
      i = 1;
      ch = GETC();
      iErrorWasReported = 0;
      for( i = 0 ; ch != cStringStartCharacter && ch != EOF ; i++ ){
        if( ch == '\n' ){
          REPORT(pszFileName,
                 lLineNumber,
                 LEX_ERROR_STRING_NEW_LINE,
                 pLex->buffer);
          break;
          }
        if( ch == *(pLex->ESCS) ){
          ch = GETC();
          
          if( ((pLex->fFlag&LEX_PROCESS_STRING_NUMBER) && isdigit(ch)) ||
              ((pLex->fFlag&LEX_PROCESS_STRING_NUMBER) &&  (pLex->fFlag&LEX_PROCESS_STRING_HEX_NUMBER) && ch == 'x') ){
            esig = 0;
            /* choose the radix */
            if( ch == 'x' ){ch = GETC();ibase = 16; }else
              if( (pLex->fFlag&LEX_PROCESS_STRING_OCTAL_NUMBER) && ch == '0' )ibase = 8; 
                else ibase = 10;

            while( isdigit(ch) || (ibase == 16 && isinset(ch,"0123456789ABCDEFabcdef")) ){
              esig = ibase*esig + ch;
              if( isdigit(ch) )
                esig -= '0';
              else
                if( islower(ch) )
                  esig -= 'a'-10;
                else
                  esig -= 'A'-10;

              ch = GETC();
              }
            STORE_CH(esig);
            continue;
            }

          s = pLex->ESCS+1;
          while( *s && *s != ch )s+=2;
          if( *s ){
            /* if there is a replacement character use that */
            s++;
            STORE_CH(*s);
            ch = GETC();
            continue;
            }else{
            /* if there is no other character for this to be replaced use the original */
            STORE_CH(ch);
            ch = GETC();
            continue;
            }
          }/* End handling escape character. */
        STORE_CH(ch);
        ch = GETC();
        }/* End of string building. */
      if( ch == EOF ){/* string was not terminated before EOF */
        REPORT(pszFileName,
               lLineNumber,
               LEX_ERROR_STRING_NOT_TERMINATED,
               pLex->buffer);
        }
      STORE_CH( (char)0 );
      i++;
      if( (pfLexeme = NewLexeme()) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      pfLexeme->next = NULL;
      pfLexeme->type = LEX_T_STRING;
      pfLexeme->lLineNumber = lLineNumber;
      pfLexeme->szFileName = pszFileName;
      pfLexeme->sLen = i-1; /* do not coun the terminating zero */
      if( (pfLexeme->value.sValue = (char *)lexALLOC(i)) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      memcpy(pfLexeme->value.sValue,pLex->buffer,i);
      if( pLex->pPREP && pLex->pPREP->n ){
        iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexMString,&plexLastLexeme);
        if( iResult )return iResult;
        }
      plexLastLexeme = &(pfLexeme->next);
      continue;
      }
#undef STORE_CH

    /*** PROCESS A NUMBER ***/

    nch = GETC();
    UNGETC(nch);
    if( (ch == '0' && ( nch == 'x' || nch == 'X' )) ||
        (ch == '&' && ( nch == 'h' || nch == 'H' )) ){
      /** This is 0x... or &H... hexadecimal number **/
      GETC();ch = GETC();
      for( hintpart = 0 ; isinset(ch,"0123456789ABCDEFabcdef") ; ch = GETC() ){
        hintpart = 16* hintpart + ch;
        if( isdigit(ch) )
          hintpart -= '0';
        else
          if( islower(ch) )
            hintpart -= 'a'-10;
          else
            hintpart -= 'A'-10;
        }
      if( (pfLexeme = NewLexeme()) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      pfLexeme->next = NULL;
      pfLexeme->type = LEX_T_LONG;
      pfLexeme->value.lValue = hintpart;
      pfLexeme->lLineNumber = lLineNumber;
      pfLexeme->szFileName = pszFileName;
      if( pLex->pPREP && pLex->pPREP->n ){
        iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexInteger,&plexLastLexeme);
        if( iResult )return iResult;
        }
      plexLastLexeme = &(pfLexeme->next);
      UNGETC(ch);
      continue;
      }

    /* We can in no way handle -nnn or +nn as numbers. The are
       unary operators followed by an expression consiting of a single number.
       If we say at this level that -nnn is a number then for the expression
       eg: 6-3 becomes meaning less, a number 6 followed by another number -3
       without operator. Wherever higher level of syntax analysis accept a
       number they should also accept a whole expression. Even though there is a pseudo
       terminal called number in the file synrax.def, but we never use it. */
    if( isdigit(ch) || ch == '.' ){
      /* phase 8.2 : la partie entiere est accumulee en long long EXACTE
         tant qu'elle tient ; au-dela, on bascule en double (la valeur
         devient un reel, comme pour tout debordement). intpart reste en
         double pour la partie fractionnaire et l'exposant. */
      llint = 0;
      deborde = 0;
      for( intpart = 0 ; isdigit(ch) ; ch = GETC() ){
        if( ! deborde && llint <= (LLONG_MAX - (ch-'0')) / 10 )
          llint = llint*10 + (ch-'0');
        else
          deborde = 1;
        intpart *= 10;
        intpart += ch-'0';
        }
      i = 1; /* this is an integer so far */
      if( ch == '#' ){
        /* in this case this was the base of the number and the number comes now */
        if( intpart < 2 || intpart > 36 )return LEX_ERROR_BAD_RADIX;
        ibase = (int)intpart;
        ch = GETC();
        if( ! isinset(ch,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") )return LEX_ERROR_INVALID_NUMBER;
        hintpart = 0;
        while( isinset(ch,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") ){
          nch = ch;
          if( isdigit(nch) )
            nch -= '0';
          else
            if( islower(nch) )
              nch -= 'a'-10;
            else
              nch -= 'A'-10;
          if( nch >= ibase )break;
          hintpart = ibase * hintpart + nch;
          ch = GETC();
          }
        if( (pfLexeme = NewLexeme()) == NULL ){
          lexFREE(UngetBuffer);
          return LEX_ERROR_MEMORY_LOW;
          }
        pfLexeme->next = NULL;
        pfLexeme->type = LEX_T_LONG;
        pfLexeme->value.lValue = hintpart;
        pfLexeme->lLineNumber = lLineNumber;
        pfLexeme->szFileName = pszFileName;
        if( pLex->pPREP && pLex->pPREP->n ){
          iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexInteger,&plexLastLexeme);
          if( iResult )return iResult;
          }
        plexLastLexeme = &(pfLexeme->next);
        UNGETC(ch);
        continue;
        }
      fracpart = 0.0; /* fractional part */
      if( ch == '.' ){
        i = 0; /* this is not an integer anymore */
        man = 1.0;      /* actual mantissa */
        for( ch = GETC() ; isdigit(ch) ; ch = GETC() )
          fracpart += (man *= 0.1) * (ch-'0');
        }
      exppart = 0.0; /* In case there is no E part then we need this value. */
      esig = 1; /* this is not really interesting because exppart is set to zero anyway but here it is */
      if( ch == 'E' || ch == 'e' ){
        i = 0; /* this is not an integer anymore if it has exponential part */
        ch = GETC(); /* step over the character E */
        if( ch == '-' )esig=-1; else esig = 1;
        if( ch == '+' || ch == '-')ch = GETC(); /* step over the exponential sign */
        for( exppart=0.0 , i = 0 ; isdigit(ch) ; ch = GETC() )
          exppart = 10*exppart + ch-'0';
        }
      if( (pfLexeme = NewLexeme()) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      pfLexeme->next = NULL;
      if( i ){/* finally this is an integer */
        /* phase 8.2 : le litteral decimal est accumule en long long
           exact ; seul un litteral au-dela de LLONG_MAX devient un reel
           (et la valeur est rendue exacte pour 9223372036854775808 = -2^63,
           le seul cas limite representable). */
        if( ! deborde ){
          pfLexeme->type = LEX_T_LONG;
          pfLexeme->value.lValue = llint;
          }else{
          pfLexeme->type = LEX_T_DOUBLE;
          pfLexeme->value.dValue = intpart;
          }
        }else{
        pfLexeme->type = LEX_T_DOUBLE;
        pfLexeme->value.dValue = (intpart + fracpart)*pow10(esig*exppart);
        }
      pfLexeme->lLineNumber = lLineNumber;
      pfLexeme->szFileName = pszFileName;
      if( pLex->pPREP && pLex->pPREP->n ){
        iResult = ipreproc_Process(pLex->pPREP,
                                   pfLexeme->type == LEX_T_DOUBLE ? PreprocessorLexReal : PreprocessorLexInteger,
                                   &plexLastLexeme);
        if( iResult )return iResult;
        }
      plexLastLexeme = &(pfLexeme->next);
      UNGETC(ch);
      continue;
      }
    
    /*** PROCESS A NON ALPHA SYMBOL ***/
    if( pNA = pLex->pNASymbols ){
      pLex->buffer[0] = ch;
      for( i = 1 ; i < pLex->cbNASymbolLength-1 ; i++ )
         pLex->buffer[i] = GETC();
      while( pNA->Symbol && strncmp(pLex->buffer,pNA->Symbol,ibase=strlen(pNA->Symbol)) )
        pNA ++;
      if( pNA->Symbol ){/* symbol is found */
        while( i > ibase )UNGETC(pLex->buffer[--i]);
        if( (pfLexeme = NewLexeme()) == NULL ){
          lexFREE(UngetBuffer);
          return LEX_ERROR_MEMORY_LOW;
          }
        pfLexeme->next = NULL;
        pfLexeme->type = LEX_T_NSYMBOL;
        pfLexeme->value.lValue = pNA->Code;
        pfLexeme->lLineNumber = lLineNumber;
        pfLexeme->szFileName = pszFileName;
        if( pLex->pPREP && pLex->pPREP->n ){
          iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexNASymbol,&plexLastLexeme);
          if( iResult )return iResult;
          }
        plexLastLexeme = &(pfLexeme->next);
        continue;
        }
      /* Symbol was not found. */
      i--;
      while( i )UNGETC(pLex->buffer[i--]);/* unget all but one character */
      if( (pfLexeme = NewLexeme()) == NULL ){
        lexFREE(UngetBuffer);
        return LEX_ERROR_MEMORY_LOW;
        }
      pfLexeme->next = NULL;
      pfLexeme->type = LEX_T_CHARACTER;
      pfLexeme->value.lValue = *(pLex->buffer);
      pfLexeme->lLineNumber = lLineNumber;
      pfLexeme->szFileName = pszFileName;
      if( pLex->pPREP && pLex->pPREP->n ){
        iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexCharacter,&plexLastLexeme);
        if( iResult )return iResult;
        }
      plexLastLexeme = &(pfLexeme->next);
      continue;
      }
    }
  lexFREE(UngetBuffer);
  if( pLex->pPREP && pLex->pPREP->n ){
    iResult = ipreproc_Process(pLex->pPREP,PreprocessorLexDone,pLex);
    if( iResult )return iResult;
    }
  return LEX_ERROR_SUCCESS;
  }

static void *lex_malloc(size_t n, void *pMemorySegment){
  return malloc(n);
  }
static void lex_free(void *p, void *pMemorySegment){
  free(p);
  }
static long _MyLineNumber(void *p){
  return 0L;
  }
static char * _MyFileName(void *p){
  return "No-File";
  }

/*POD
=H lex_InitStructure()

You may but need not call this function to initialize a T<LexObject>. You may
also call this function to use the settings of the function and set some
variables to different values after the function returns.

/*FUNCTION*/
void lex_InitStructure(pLexObject pLex
  ){
/*noverbatim
CUT*/
  pLex->pfGetCharacter = NULL;
  pLex->pfFileName = _MyFileName;
  pLex->pfLineNumber = _MyLineNumber;
  pLex->SSC = "QWERTZUIOPASDFGHJKLYXCVBNMqwertzuiopasdfghjklyxcvbnm_:$";
  pLex->SCC = "QWERTZUIOPASDFGHJKLYXCVBNMqwertzuiopasdfghjklyxcvbnm_1234567890:$";
  pLex->SFC = "QWERTZUIOPASDFGHJKLYXCVBNMqwertzuiopasdfghjklyxcvbnm_1234567890$";
  pLex->SStC = "\"";
  pLex->ESCS = "\\n\nt\tr\r\"\"\'\'";
  pLex->fFlag = LEX_PROCESS_STRING_NUMBER       |
                LEX_PROCESS_STRING_OCTAL_NUMBER |
                LEX_PROCESS_STRING_HEX_NUMBER   |
                0;
  pLex->SKIP = " \t\r"; /* spaces to skip 
                           \r is included to ease compilation of DOS edited 
                           binary transfered files to run on UNIX */
  pLex->pNASymbols = NULL;
  pLex->pASymbols  = NULL;
  pLex->pCSymbols  = NULL;
  pLex->cbNASymbolLength = 0; /* it is to be calculated */

  pLex->buffer = lexALLOC(BUFFERINCREASE*sizeof(char));
  if( pLex->buffer )
    pLex->cbBuffer = BUFFERINCREASE;
  else
    pLex->cbBuffer = 0;

  CALL_PREPROCESSOR(PreprocessorLexInit,pLex);
  }
