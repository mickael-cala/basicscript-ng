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

This file contains mainly header information. Its public declarations are in
basext.h, which is an ordinary source file.

We do not created basext.h using an editor because all .h files may be deleted
in this project during cleanup and we may loose this file accidentally.

*/

#include <stdio.h>
#include <stdarg.h>
#include "basext.h"

/*POD
=H basext_GetArgsF()

This function can be used to get arguments simple and fast in extension modules.
All functionality of this function can be individually programmed using the
T<besXXX> macros. Here it is to ease the programming of extension modules for most of
the cases.

This function should be called like

=verbatim
  iError = besGETARGS "ldz",&l1,&d1,&s besGETARGE
=noverbatim

The macro T<besGETARGS> (read GET ARGument Start) hides the complexity of the
function call and the macro T<besGETARGE> (read Get ARGument End) simply closes the
function call.

The first argument is format string. Each character specifies how the next argument
should be treated.

/*FUNCTION*/
int basext_GetArgsF(pSupportTable pSt,
                    pFixSizeMemoryObject pParameters,
                    char *pszFormat,
                    ...
  ){
/*noverbatim
The following characters are recognized:

=itemize
=item T<i> the next argument of the function call should point to a T<long> variable.
           The ScriptBasic argument will be converted to T<long> using the macro
           T<besCONVERT2LONG> and will be stored in the T<long> variable.
=item T<r> the same as T<l> except that the argument should point a T<double> and the
           basic argument is converted to T<double> using T<besCONVERT2DOUBLE>.
=item T<z> the next argument should point to a T<char *> pointer. The function takes
           the next BASIC argument as string, converts it to zero terminated string
           allocating space for it. These variables SHOULD be released by the caller
           using the macro T<besFREE>.
=item T<s> the next argument should point to a T<unsigned char *> pointer. The function takes
           the next BASIC argument as string, converting it in case conversion is needed, and
           sets the T<unsigned char *> pointer to point to the string. This format character
           should be used together with the character T<l>
=item T<l> the next argument should point to a T<long> and the value of the variable
           will be the length of the last string atgument (either T<z> or T<s>).
           If there was no previous string argument the value returned will be zero.
=item T<p> the next argument should point to a T<void *> pointer. The BASIC argument value
           should be a string of T<sizeof(void *)> characters that will be copied into the
           pointer argument. If the argument is not string or has not the proper size the function
           returns T<COMMAND_ERROR_ARGUMENT_RANGE>.
=item T<[> The arguments following this character are optional. Optional arguments may be
           unspecified. This is the case when the BASIC function call has less number of
           arguments or when the actual argument value is T<undef>. In case of optional
           arguments the T<undef> values are converted to zero value of the appropriate
           type. This means 0 in case of long, 0.0 in case of double, NULL in case of pointer
           and zero length string in case of strings.
=item T<]> Arguments following this character are mandatory (are not optional). When the
           function starts to process the arguments they are mandatory by default. Using this
           notation you can enclode the optional arguments between T<[> and T<]>. For example
           the format string T<"ii[z]"> means two long arguments and an optional zero terminated
           string argument.
=item T<*> The argument is skipped. This may be used during development of a function.
=noitemize

The return value of the function is zero in case there is no error or the error code.
CUT*/
  va_list marker;
  long ArgNr;
  void *argptr;
  char *s;
  long *pL;
  double *pD;
  unsigned char **ppS;
  void **ppV;
  VARIABLE Argumentum;
  int mandatory; /* true when we process mandatory arguments (should not be undef) */
  char **ppszStrAlloc;
  long iStrArg,iArgNr;
  unsigned long lLastStrLen = 0;

  /* allocate a string array that keeps track of all allocated zchar strings
     if the memory allocation fails somewhere in between the string are freed
     and we do not loose memory */
  ppszStrAlloc = (char **)besALLOC(sizeof(char *) * (iArgNr=strlen(pszFormat)));
  if( ppszStrAlloc == NULL )return COMMAND_ERROR_MEMORY_LOW;
  for( iStrArg = 0 ; iStrArg < iArgNr ; iStrArg++ )ppszStrAlloc[iStrArg] = NULL;
  iStrArg = 0; /* the next string */
  va_start(marker,pszFormat);
  ArgNr = 1;
  mandatory = 1;
#define NEXT_ARG argptr = va_arg(marker, void * );
  NEXT_ARG
  for( s = pszFormat ; *s ; s++ ){
    switch( *s ){
      case '[': mandatory = 0;break;
      case ']': mandatory = 1;break;
      case '*': ArgNr++; break; /* Skip this argument */
      case 'l': /* Get the length of the last string either 'z' or 's' */
        pL = (long *)argptr;
        NEXT_ARG
        *pL = lLastStrLen;
        break;
      case 'i': /* Get a long */
        pL = (long *)argptr;
        NEXT_ARG
        Argumentum = besARGUMENT(ArgNr);
        besDEREFERENCE(Argumentum);
        ArgNr++;
        if( memory_IsUndef(Argumentum) && mandatory ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MANDARG;
          }
        if( memory_IsUndef(Argumentum) ){
          *pL = 0;
          }else{
          Argumentum = besCONVERT2LONG(Argumentum);
          *pL = LONGVALUE(Argumentum);
          }
        break;
      case 'r': /* Get a double */
        pD = (double *)argptr;
        NEXT_ARG
        Argumentum = besARGUMENT(ArgNr);
        ArgNr++;
        besDEREFERENCE(Argumentum);
        if( memory_IsUndef(Argumentum) && mandatory ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MANDARG;
          }
        if( memory_IsUndef(Argumentum) ){
          *pD = 0.0;
          }else{
          *pD = besGETDOUBLEVALUE(Argumentum);
          }
        break;
      case 'z': /* Get a zchar string */
        ppS = (unsigned char **)argptr;
        NEXT_ARG
        Argumentum = besARGUMENT(ArgNr);
        ArgNr++;
        besDEREFERENCE(Argumentum);
        if( memory_IsUndef(Argumentum) && mandatory ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MANDARG;
          }
        Argumentum = besCONVERT2STRING(Argumentum);
        *ppS = besALLOC( (lLastStrLen = STRLEN(Argumentum))+1);
        if( *ppS == NULL ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MEMORY_LOW;
          }
        ppszStrAlloc[iStrArg++] = *ppS;
        memcpy(*ppS,STRINGVALUE(Argumentum),lLastStrLen);
        (*ppS)[STRLEN(Argumentum)] = (char)0;
        break;
      case 'p': /* Get a pointer */
        ppV = (void **)argptr;
        NEXT_ARG
        Argumentum = besARGUMENT(ArgNr);
        ArgNr++;
        besDEREFERENCE(Argumentum);
        if( memory_IsUndef(Argumentum) && mandatory ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MANDARG;
          }
        if( ! memory_IsUndef(Argumentum) && 
           (TYPE(Argumentum) != VTYPE_STRING || 
            STRLEN(Argumentum) != sizeof( void * )) ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_ARGUMENT_RANGE;
          }
        if( memory_IsUndef(Argumentum) )
          *ppV = NULL;
        else
          memcpy(ppV, STRINGVALUE(Argumentum),sizeof( void * ) );
        break;
      case 's': /* Get a string */
        ppS = (unsigned char **)argptr;
        NEXT_ARG
        Argumentum = besARGUMENT(ArgNr);
        ArgNr++;
        besDEREFERENCE(Argumentum);
        if( memory_IsUndef(Argumentum) && mandatory ){
          while( iStrArg -- )besFREE(ppszStrAlloc[iStrArg]);
          besFREE(ppszStrAlloc);
          return COMMAND_ERROR_MANDARG;
          }
        Argumentum = besCONVERT2STRING(Argumentum);
        *ppS = STRINGVALUE(Argumentum);
        lLastStrLen = STRLEN(Argumentum);
        break;
      }
    }
  va_end( marker );
  besFREE(ppszStrAlloc);
  return 0;
  }
