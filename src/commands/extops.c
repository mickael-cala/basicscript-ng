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
#include "../command.h"

/* NONULOP appelle RaiseError, defini dans mathops.c (comme mathfunc.c
   le declare au lieu de l'inclure). */
long *RaiseError(pExecuteObject pEo);

/* As a default behaviour this operator returns the same number
   as it gets, because some lasy BASIC programmers write

   OPEN "file" for input as #1

   and they have HARD TIME finding it!
*/
COMMAND(EXTOPL)
  NODE nItem;
  VARIABLE Op1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameters */
  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  RESULT = Op1;
END

COMMAND(EXTOPAG)
  ERROR(COMMAND_ERROR_NOTIMP); /* += */
END

COMMAND(EXTOPBG)
  ERROR(COMMAND_ERROR_NOTIMP); /* -= */
END

COMMAND(EXTOPDD)
  /* << : decalage a gauche, sur des long long (phase 8.2 : l'entier du
     langage est 64 bits sur toutes les cibles, LLP64 compris — ne pas
     ecrire sizeof(unsigned long), qui fait 32 sous Windows). Le
     compteur est ramene a 0..63 ; au-dela, le resultat est 0. Un
     compteur negatif ne decale pas. Le calcul se fait en unsigned
     long long pour eviter le debordement signe, qui serait un
     comportement indefini en C. */
  NODE nItem;
  VARIABLE Op1,Op2;
  unsigned long long uValue;
  long long lShift;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  nItem = CDR(nItem);
  Op2 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op2)

  uValue = (unsigned long long)GETLONGVALUE(Op1);
  lShift = GETLONGVALUE(Op2);
  if( lShift < 0 )lShift = 0;
  if( lShift >= (long long)(8*sizeof(unsigned long long)) ){
    uValue = 0;
    }else{
    uValue <<= lShift;
    }
  RETURN_LONG_VALUE((long long)uValue)
END

COMMAND(EXTOPEE)
  /* >> : decalage a droite arithmetique (le signe est conserve), sans
     dependre du comportement defini par l'implementation pour un long
     negatif : on decale le complement, puis on le recomplemente. */
  NODE nItem;
  VARIABLE Op1,Op2;
  long long lValue,lShift;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  nItem = CDR(nItem);
  Op2 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op2)

  lValue = GETLONGVALUE(Op1);
  lShift = GETLONGVALUE(Op2);
  if( lShift < 0 )lShift = 0;
  if( lShift >= (long long)(8*sizeof(long long)) ){
    lValue = ( lValue < 0 ? -1 : 0 );
    }else if( lValue < 0 ){
    lValue = ~( (~lValue) >> lShift );
    }else{
    lValue >>= lShift;
    }
  RETURN_LONG_VALUE(lValue)
END

COMMAND(EXTOPFF)
  /* ?? : rend l'operande gauche si elle est definie, sinon la droite.
     La droite n'est evaluee que si elle est necessaire. */
  NODE nItem;
  VARIABLE Op1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  ASSERTOKE;
  if( ! memory_IsUndef(Op1) ){
    RESULT = Op1;
    RETURN;
    }
  nItem = CDR(nItem);
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  ASSERTOKE;
  RESULT = Op1;
END

COMMAND(EXTOPHG)
  ERROR(COMMAND_ERROR_NOTIMP); /* *= */
END

COMMAND(EXTOPIG)
  ERROR(COMMAND_ERROR_NOTIMP); /* /= */
END

COMMAND(EXTOPMG)
  ERROR(COMMAND_ERROR_NOTIMP); /* &= */
END

COMMAND(EXTOPNG)
  ERROR(COMMAND_ERROR_NOTIMP); /* \= */
END

