/*mathfunc.c

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

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#include "../command.h"

/* this function is defined in mathops.c */
long *RaiseError(pExecuteObject pEo);

/* pi a la precision du double ; PI(), RAD2DEG(), DEG2RAD() et ACTAN() s'y
   rapportent, et une constante nommee evite trois valeurs legerement
   differentes dans le meme fichier */
#define SBPI 3.14159265358979323846

#define GET_ONE_ARG USE_CALLER_MORTALS; nItem = PARAMETERLIST; Op1 = EVALUATEEXPRESSION(CAR(nItem)); NONULOP(Op1)


/*POD
=H Mathematical Functions

This file defines all the mathematical functions that are implemented in ScriptBasic.
CUT*/


/**POW
=section math
=display POW()

Calculates the x-th exponent of 10. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<POW(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(POW)
#if NOTIMP_POW
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( pow(10.0,GETDOUBLEVALUE(Op1)) )

#endif
END


/**EXP
=section math
=display EXP()

Calculates the x-th exponent of T<e>. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<EXP(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(EXP)
#if NOTIMP_EXP
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG
  RETURN_DOUBLE_VALUE_OR_LONG( exp(GETDOUBLEVALUE(Op1)) )

#endif
END

/**LOG
=display LOG()
=section math

Calculates the natural log of the argument. If the argument is zero or less than zero the result is T<undef>.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<LOG(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(LOG)
#if NOTIMP_LOG
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 <= 0.0 ){
    RETURNUNDEF
    }
  RETURN_DOUBLE_VALUE_OR_LONG( log(dop1) )

#endif
END

/**LOG10
=display LOG10()
=section math

Calculates the log of the argument. If the argument is zero or less than zero the result is T<undef>

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<LOG10(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(LOG10)
#if NOTIMP_LOG10
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 <= 0.0 ){
    RETURNUNDEF
    }
  RETURN_DOUBLE_VALUE_OR_LONG( log10(dop1) )

#endif
END

/**SIN
=section math
=display SIN()

Calculates the sine of the argument. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<SIN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(SIN)
#if NOTIMP_SIN
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( sin(GETDOUBLEVALUE(Op1)) )

#endif
END

/**ASIN
=section math
=display ASIN()

Calculates the arcus sine of the argument, which is the inverse of the function R<SIN>. If the argument is not between (-1.0,+1.0) the return value is T<undef>.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<ASIN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(ASIN)
#if NOTIMP_ASIN
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 < -1 || dop1 > +1 ){
    RETURNUNDEF
    }
  RETURN_DOUBLE_VALUE_OR_LONG( asin(dop1) )

#endif
END

/**ACOS
=display ACOS()
=section math

Calculates the arcus cosine of the argument, which is the inverse of the function R<COS>. If the argument is not between (-1.0,+1.0) the return value is T<undef>.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<ACOS(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(ACOS)
#if NOTIMP_ACOS
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 < -1 || dop1 > +1 ){
    RETURNUNDEF
    }
  RETURN_DOUBLE_VALUE_OR_LONG( acos(dop1) )

#endif
END

/**COS
=section math
=display COS()
Calculates the cosine of the argument.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<COS(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(COS)
#if NOTIMP_COS
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( cos(GETDOUBLEVALUE(Op1)) )

#endif
END

/**RND
=section math
=display RND()

Returns a random number as generated by the C function T<rand()>. Note that this random number generator usually provided by the libraries implemented for the C compiler or the operating system is not the best quality ones. If you need really good random number generator then you have to use some other libraries that implement reliable RND functions.

*/
COMMAND(RND)
#if NOTIMP_RND
NOTIMPLEMENTED;
#else

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;
  RETURN_DOUBLE_VALUE( rand() )

#endif
END

/**RANDOMIZE
=section math
=display RANDOMIZE

Seed the random number generator. If the command is presented without argument the random number generator is seed with the actual time. If argument is provided the random number generator is seed with the argument following the keyword T<RANDOMIZE>.
*/
COMMAND(RANDOMIZA)
#if NOTIMP_RANDOMIZA
NOTIMPLEMENTED;
#else
  time_t timer;
  srand(time(&timer));

#endif
END

COMMAND(RANDOMIZE)
#if NOTIMP_RANDOMIZE
NOTIMPLEMENTED;
#else
  time_t timer;
  NODE nItem;
  VARIABLE ItemResult;

  nItem = PARAMETERNODE;
  if( nItem ){
    ItemResult = _EVALUATEEXPRESSION_A(nItem);
    ASSERTOKE;
    if( ItemResult )
      srand(GETLONGVALUE(ItemResult));
    else
      srand(time(&timer));
    }else{
    srand(time(&timer));
    }

#endif
END

/**SGN
=display SGN()
=section math

Calculates the signum of the argument. This is -1 for negative numbers, +1 for positive numbers and 0 for zero. The return value is always long except that  T<SGN(undef)> may return T<undef>. If the argument is a string it is converted to long or double.

T<SGN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(SGN)
#if NOTIMP_SGN
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dop1;
  long lop1,result;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    lop1 = GETLONGVALUE(Op1);
    if( lop1 == 0 )result = 0;
    else if( lop1 > 0 )result = 1;
    else result = -1;
    }else{
    dop1 = GETDOUBLEVALUE(Op1);
    if( dop1 == 0.0 )result = 0;
    else if( dop1 > 0 )result = 1;
    else result = -1;
    }

  RETURN_LONG_VALUE( result );

#endif
END

/**SQRT
=section math
=display SQRT()

Calculates the square root of the argument. The name is SQRT, as in every other
language; the old SQR spelling is gone.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

T<SQRT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

If the argument is a negative number the result of the function is T<undef> or the function raises error if the option T<RaiseMathError> has the bit T<sbMathErrDiv> set.

If the square root of the argument is an integer number then the function returns an integer number. In other cases the returned value is real even if the argument itself is integer.

*/
COMMAND(SQRT)
#if NOTIMP_SQR
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dResult;
  long lSquare,lop1;
  double dop1;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    lop1 = GETLONGVALUE(Op1);
    /* we return undef or raise error based on OPTION setting */
    if( lop1 < 0 ){
      RETURNUNDEF
      }
    /* calculate the square root of the argument */
    dResult = sqrt((double)lop1);
    /* round the result and convert to long to check if it can be handles as a long*/
    lSquare = (long) floor(dResult + 0.5);
    /* if the square of the rounded result is the same as the argument then it can be returned as a long */
    if( lSquare * lSquare == lop1 ){
      RETURN_LONG_VALUE(lSquare)
      }
    /* if it can not be converted to long w/o loosing precision then return the 'double' value */
    RETURN_DOUBLE_VALUE(dResult)
    }

  /* if the argument is double */
  dop1 = GETDOUBLEVALUE(Op1);
  /* we return undef or raise error based on OPTION setting */
  if( dop1 < 0 ){
    RETURNUNDEF
    }
  /* if the argument was double then the result is double */
  RETURN_DOUBLE_VALUE( sqrt(dop1) )
#endif
END

/**ABS
=display ABS()
=section math
Returns the absolute value of the argument. If the argument is a string then it first converts it to integer or real value. The return value is integer or real value depending on the argument.

T<ABS(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(ABS)
#if NOTIMP_ABS
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  long lop1;
  double dop1;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    /* if the argument is long then the ABS value of it is also long*/
    lop1 = GETLONGVALUE(Op1);
    /* ... with the only exception of LONG_MAX because ABS(LONG_MIN) is LONG_MAX-1 usually (two's complement) */
    /* if ever you implement ScriptBasic on a machine that uses different signed integer not two's complement then you have to rewrite it */
    if( lop1 < LONG_MAX ){
      RETURN_LONG_VALUE( lop1 > 0 ? lop1 : -lop1 );
      }else{
      RETURN_DOUBLE_VALUE( -(double)lop1 );
      }
    }
  /* if the argument is double then the result will also be double */
  dop1 = GETDOUBLEVALUE(Op1);
  RETURN_DOUBLE_VALUE( dop1 > 0.0 ? dop1 : -dop1 );
#endif
END

/**VAL
=display VAL()
=section math

Converts a string to numeric value. If the string is integer it returns an integer value. If the string contains a number presentation which is a float number the returned value is real. In case the argument is already numeric no conversion is done.

T<VAL(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
*/
COMMAND(VAL)
#if NOTIMP_VAL
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;

  GET_ONE_ARG

  if( TYPE(Op1) == VTYPE_STRING ){
    if( ISSTRINGINTEGER(Op1) )
      RESULT = CONVERT2LONG(Op1);
    else
      RESULT = CONVERT2DOUBLE(Op1);
    }else{
    RESULT = Op1;
    }

#endif
END

/**MAXINT
=section math
=display MAXINT

This built-in constant is implemented as an argument less function. Returns the maximal number that can be stored as an integer value.
*/
COMMAND(MAXINT)
#if NOTIMP_MAXINT
NOTIMPLEMENTED;
#else

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  RETURN_LONG_VALUE(LONG_MAX)

#endif
END

/**MININT
=section math
=display MININT

This built-in constant is implemented as an argument less function. Returns the minimal ("maximal negative") number that can be stored as an integer value.
*/
COMMAND(MININT)
#if NOTIMP_MININT
NOTIMPLEMENTED;
#else

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  RETURN_LONG_VALUE(LONG_MIN)

#endif
END

/**PI
=section math
=display PI
This built-in constant is implemented as an argument less function. Returns the approximate value of the constant PI which is the ratio of the circumference of a circle to its diameter.

*/
COMMAND(PI)
#if NOTIMP_PI
NOTIMPLEMENTED;
#else

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  RETURN_DOUBLE_VALUE(SBPI)

#endif
END

COMMAND(UNDEF)
#if NOTIMP_UNDEF
NOTIMPLEMENTED;
#else


  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  RESULT = NULL;

#endif
END

/**TRUE
=section math

This built-in constant is implemented as an argument less function. Returns the value T<true>.
*/
COMMAND(TRUE)
#if NOTIMP_TRUE
NOTIMPLEMENTED;
#else


  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;
  RETURN_LONG_VALUE( -1L )

#endif
END

/**FALSE
=section math

This built-in constant is implemented as an argument less function. Returns the value T<false>.
*/
COMMAND(FALSE)
#if NOTIMP_FALSE
NOTIMPLEMENTED;
#else


  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  RETURN_LONG_VALUE( 0L )

#endif
END

/**FIX
=display FIX()
=section math

This function returns the integral part of the argument. The return value of the function is integer with the exception that T<FIX(undef)> may return T<undef>. 

T<FIX(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.


The difference between T<INT> and T<FIX> is that T<INT> truncates down while T<FIX> truncates towards zero. The two functions are identical for positive numbers. In case of negative arguments T<INT> will give a smaller number if the argument is not integer. For example:

=verbatim
  int(-3.3) = -4
  fix(-3.3) = -3
=noverbatim

See R<INT>.
*/
COMMAND(FIX)
#if NOTIMP_FIX
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dop1;
  long   lop1;
  int isneg;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    RETURN_LONG_VALUE( GETLONGVALUE(Op1) )
    }
  dop1 = GETDOUBLEVALUE(Op1);
  if( isneg = dop1 < 0 )dop1 =  -dop1;
  dop1 = floor(dop1);
  if( isneg )dop1 = - dop1;
  lop1 = (long)dop1;
  if( dop1 == (double)lop1 ){
    RETURN_LONG_VALUE( lop1 );
    }

  RETURN_DOUBLE_VALUE( dop1 );

#endif
END

/**INT
=display INT()
=section math

This function returns the integral part of the argument.
T<INT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.
Other than this the function returns integer value.

The difference between T<INT> and T<FIX> is that T<INT> truncates down 
while T<FIX> truncates towards zero. The two functions are identical for 
positive numbers. In case of negative arguments T<INT> will give a smaller
number if the argument is not integer. For example:

=verbatim
  int(-3.3) = -4
  fix(-3.3) = -3
=noverbatim

See R<FIX>.
*/
COMMAND(INT)
#if NOTIMP_INT
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dop1;
  long   lop1;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    RETURN_LONG_VALUE( GETLONGVALUE(Op1) )
    }

  dop1 = floor(GETDOUBLEVALUE(Op1));
  lop1 = (long)dop1;
  if( dop1 == (double)lop1 ){
    RETURN_LONG_VALUE( lop1 );
    }

  RETURN_DOUBLE_VALUE( dop1 );

#endif
END


/**CINT
=display CINT()
=section math

This function is the same as R<INT> and is present in ScriptBasic to be more
compatible with other BASIC language variants.
*/

/**FRAC
=display FRAC()
=section math

The function returns the fractional part of the
argument. This function always returns a double
except that T<FRAC(undef)> may return T<undef>.
T<FRAC(undef)> is T<undef> or raises an error if
the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>. 

Negative arguments return negative value (or zero if the argument is
a negative integer), positive arguments result positive values
(or zero if the argument is integer).
*/
COMMAND(FRAC)
#if NOTIMP_FRAC
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  double dop1;

  GET_ONE_ARG

  if( ISINTEGER(Op1) ){
    RETURN_DOUBLE_VALUE( 0.0 )
    }

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 < 0 ){
    RETURN_DOUBLE_VALUE( dop1 - floor(dop1) -1 )
    }else{
    RETURN_DOUBLE_VALUE( dop1 - floor(dop1) )
    }

#endif
END


/**ROUND
=display ROUND()
=section math

This function rounds its argument. The first argument is the number to round, and the optional second argument specifies the number of fractional digits to round to.

The function rounds to integer value if the second argument is missing.

The return value is long if the number of decimal places to keep is zero, otherwise the return value is double.

Negative value for the number of decimal places results rounding to integer value.

T<ROUND(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

Also T<ROUND(val,undef)> is equivalent to T<ROUND(value)>.

See also R<INT>, R<FRAC> and R<FIX>

*/
COMMAND(ROUND)
#if NOTIMP_ROUND
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;
  long iNumberOfDigits,lop1;
  double Multiplier,dop1;

  GET_ONE_ARG

  nItem = CDR(nItem);

  iNumberOfDigits = 0;
  if( nItem ){
    iNumberOfDigits = GETLONGVALUE(EVALUATEEXPRESSION(CAR(nItem)));
    ASSERTOKE;
    }
  if( iNumberOfDigits < 0 )iNumberOfDigits = 0;
  if( iNumberOfDigits > 100 )iNumberOfDigits = 100;

  if( ISINTEGER(Op1) ){
    RETURN_LONG_VALUE( GETLONGVALUE(Op1) )
    }

  dop1 = GETDOUBLEVALUE(Op1);
  if( iNumberOfDigits == 0 ){
    dop1 = floor( dop1 + 0.5);
    lop1 = (long)dop1;
    if( dop1 == (double)lop1 ){
      RETURN_LONG_VALUE( lop1 )
      }else{
      RETURN_DOUBLE_VALUE( dop1 )
      }
    }
  Multiplier = 1.0;
  while( iNumberOfDigits -- )Multiplier *= 10.0;
  dop1 *= Multiplier;
  dop1 += 0.5;
  dop1 = floor(dop1);

  dop1 /= Multiplier;
  lop1 = (long)dop1;
  if( dop1 == (double)lop1 ){
    RETURN_LONG_VALUE( lop1 )
    }else{
     RETURN_DOUBLE_VALUE( dop1 )
   }
#endif
END

/**GCD
=section math
=display GCD()

Calculates the greatest common divisor of two or more integer arguments. Each
argument is converted to long (a real value is truncated toward zero), its sign
is ignored, and the result is the greatest non negative integer that divides all
of them. T<GCD(0,0)> is 0.

T<GCD(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<LCM>
*/
COMMAND(GCD)
#if NOTIMP_GCD
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  unsigned long a,b,r;
  long lValue;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* the first argument seeds the accumulator; the sign is dropped here so that
     the rest of the calculation can stay in unsigned arithmetic */
  nItem = PARAMETERLIST;
  Op1 = _EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  lValue = GETLONGVALUE(Op1);
  a = lValue < 0 ? (unsigned long)0 - (unsigned long)lValue : (unsigned long)lValue;

  while( (nItem = CDR(nItem)) != 0 ){
    Op1 = _EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    lValue = GETLONGVALUE(Op1);
    b = lValue < 0 ? (unsigned long)0 - (unsigned long)lValue : (unsigned long)lValue;
    /* euclid; a ends up holding the divisor of everything seen so far, so each
       argument only has to be folded into the running value */
    while( b ){
      r = a % b;
      a = b;
      b = r;
      }
    }

  RETURN_LONG_VALUE( (long)a )
#endif
END

/**LCM
=section math
=display LCM()

Calculates the least common multiple of two or more integer arguments. Each
argument is converted to long (a real value is truncated toward zero) and its
sign is ignored; the result is never negative. T<LCM(0,n)> is 0.

The multiple of several arguments is usually larger than a long, therefore the
calculation is carried in real arithmetic and the result is returned as a real
value whenever it does not fit in a long. That arithmetic is exact up to 2^53;
beyond it the result is an approximation, as everywhere else in the language.

T<LCM(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<GCD>
*/
COMMAND(LCM)
#if NOTIMP_LCM
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double a,b,r,dAccum,dArg,dResult;
  long lValue;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = _EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  lValue = GETLONGVALUE(Op1);
  dAccum = lValue < 0 ? -(double)lValue : (double)lValue;

  while( (nItem = CDR(nItem)) != 0 ){
    Op1 = _EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    lValue = GETLONGVALUE(Op1);
    dArg = lValue < 0 ? -(double)lValue : (double)lValue;

    /* gcd of the running value and the new argument, in real arithmetic: the
       running value is already an integer, possibly larger than a long, and
       fmod is exact on integral doubles */
    a = dAccum;
    b = dArg;
    while( b != 0.0 ){
      r = fmod(a,b);
      a = b;
      b = r;
      }
    /* a is now the gcd: divide before multiplying, so the intermediate value
       stays as small as possible. A zero is absorbing: LCM(0,n) is 0. */
    if( a == 0.0 ){
      dAccum = 0.0;
      }else{
      dAccum = dAccum / a * dArg;
      }
    }

  RETURN_DOUBLE_VALUE_OR_LONG( dAccum )
#endif
END

/**ODD
=section math test
=display ODD()

Return T<true> if the argument is an odd number. T<ODD(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<EVEN>
*/
COMMAND(ODD)
#if NOTIMP_ODD
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;

  GET_ONE_ARG

  RESULT = NEWMORTALLONG;

  if( GETLONGVALUE(Op1) & 1 )
    LONGVALUE(RESULT) = -1;
  else
    LONGVALUE(RESULT) = 0;

#endif
END

/**EVEN
=display EVEN()
=section math test
Return T<true> if the argument is an even number. T<EVEN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<ODD>.
*/
COMMAND(EVEN)
#if NOTIMP_EVEN
NOTIMPLEMENTED;
#else


  NODE nItem;
  VARIABLE Op1;

  GET_ONE_ARG

  RESULT = NEWMORTALLONG;
  ASSERTNULL(RESULT)

  if( GETLONGVALUE(Op1) & 1 )
    LONGVALUE(RESULT) = 0;
  else
    LONGVALUE(RESULT) = -1;

#endif
END

/**LBOUND
=section array
=display LBOUND()

This function can be used to determine the lowest occupied index of an array. Note that arrays are increased in addressable indices automatically, thus it is not an error to use a lower index that the value returned by the function T<LBOUND>. On the other hand all the element having index lower than the returned value are T<undef>.

The argument of this function has to be an array. If the argument is an ordinary value, or a variable that is not an array the value returned by the function will be T<undef>.

T<LBOUND(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<UBOUND>.
*/
COMMAND(LBOUND)
#if NOTIMP_LBOUND
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  NONULOP(ItemResult)

  switch( TYPE(ItemResult) ){
    case VTYPE_LONG:
    case VTYPE_DOUBLE:
    case VTYPE_STRING:
      RESULT = NULL;
      RETURN;
    case VTYPE_ARRAY:
      RESULT = NEWMORTALLONG;
      ASSERTNULL(RESULT)
      LONGVALUE(RESULT) = ItemResult->ArrayLowLimit;
      RETURN;
    }

#endif
END

/**UBOUND
=section array
=display UBOUND()

This function can be used to determine the highest occupied index of an array. Note that arrays are increased in addressable indices automatically, thus it is not an error to use a higher index that the value returned by the function T<UBOUND>. On the other hand all the element having index larger than the returned value are T<undef>.

The argument of this function has to be an array. If the argument is an ordinary value, or a variable that is not an array the value returned by the function will be T<undef>.

T<UBOUND(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<LBOUND>.

*/
COMMAND(UBOUND)
#if NOTIMP_UBOUND
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  NONULOP(ItemResult)

  switch( TYPE(ItemResult) ){
    case VTYPE_LONG:
    case VTYPE_DOUBLE:
    case VTYPE_STRING:
      RESULT = NULL;
      RETURN;
    case VTYPE_ARRAY:
      RESULT = NEWMORTALLONG;
      ASSERTNULL(RESULT)
      LONGVALUE(RESULT) = ItemResult->ArrayHighLimit;
      RETURN;
    }

#endif
END

/**ISARRAY
=section array test
=display ISARRAY()

This function can be used to determine whether a variable holds array value or ordinary value. If the variable passed as argument to the function is an array then the function returns T<true>, otherwise the function returns T<false>.

See also R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/
COMMAND(ISARRAY)
#if NOTIMP_ISARRAY
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ){
    RETURN_FALSE
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_LONG:
    case VTYPE_DOUBLE:
    case VTYPE_STRING:
      RETURN_FALSE
    case VTYPE_ARRAY:
      RETURN_TRUE
    }

#endif
END

/**ISSTRING
=section string test
=display ISSTRING()

This function can be used to determine whether an expression is string or some other type of value. If the argument is a string then the function returns T<true>, otherwise the function returns T<false>.

See also R<ISARRAY>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/
COMMAND(ISSTRING)
#if NOTIMP_ISSTRING
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ){
    RETURN_FALSE
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_LONG:
    case VTYPE_DOUBLE:
    case VTYPE_ARRAY:
      RETURN_FALSE
    case VTYPE_STRING:
      RETURN_TRUE
    }

#endif
END

/**ISINTEGER
=section test
=display ISINTEGER()

This function can be used to determine whether an expression is integer or some other type of value. If the argument is an integer then the function returns T<true>, otherwise the function returns T<false>.

See also R<ISARRAY>, R<ISSTRING>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/
COMMAND(ISLONG)
#if NOTIMP_ISLONG
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ){
    RETURN_FALSE
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_DOUBLE:
    case VTYPE_ARRAY:
    case VTYPE_STRING:
      RETURN_FALSE
    case VTYPE_LONG:
      RETURN_TRUE
    }

#endif
END

/**ISREAL
=section test
=display ISREAL()

This function can be used to determine whether an expression is real or some other type of value. If the argument is a real then the function returns T<true>, otherwise the function returns T<false>.

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/

COMMAND(ISDOUBLE)
#if NOTIMP_ISDOUBLE
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ){
    RETURN_FALSE
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_ARRAY:
    case VTYPE_STRING:
    case VTYPE_LONG:
      RETURN_FALSE
    case VTYPE_DOUBLE:
      RETURN_TRUE
    }

#endif
END

/* Un nombre decimal complet : espaces autour tolerees, signe optionnel,
   chiffres, point decimal, exposant. « 12abc » ou « 1,5 » ne sont pas des
   nombres, « 12 », « -3.5 », « 7 » et « 1e3 » en sont. L'analyse est faite
   a la main : strtod accepterait « nan », « inf » et, selon la locale, la
   virgule (ERRATA E29). */
static int is_number_string(char *s,unsigned long len){
  unsigned long i = 0,digits = 0,exp_digits;

  while( i < len && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') )i++;
  if( i < len && (s[i] == '+' || s[i] == '-') )i++;
  while( i < len && s[i] >= '0' && s[i] <= '9' ){ i++; digits++; }
  if( i < len && s[i] == '.' ){
    i++;
    while( i < len && s[i] >= '0' && s[i] <= '9' ){ i++; digits++; }
    }
  if( ! digits )return 0;
  if( i < len && (s[i] == 'e' || s[i] == 'E') ){
    i++;
    if( i < len && (s[i] == '+' || s[i] == '-') )i++;
    exp_digits = 0;
    while( i < len && s[i] >= '0' && s[i] <= '9' ){ i++; exp_digits++; }
    if( ! exp_digits )return 0;
    }
  while( i < len && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n') )i++;
  return i == len;
  }

/**ISNUMERIC
=section test
=display ISNUMERIC()

This function can be used to determine whether an expression is numeric (real or integer) or some other type of value. If the argument is a real or an integer then the function returns T<true>, otherwise the function returns T<false>.

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/

COMMAND(ISNUMERIC)
#if NOTIMP_ISNUMERIC
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ){
    RETURN_FALSE
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_ARRAY:
      RETURN_FALSE
    case VTYPE_STRING:
      if( is_number_string(STRINGVALUE(ItemResult),STRLEN(ItemResult)) ){
        RETURN_TRUE
        }else{
        RETURN_FALSE
        }
    case VTYPE_LONG:
    case VTYPE_DOUBLE:
      RETURN_TRUE
    }
#endif
END

/**ISDEFINED
=section test
=display ISDEFINED()

This function can be used to determine whether an expression is defined or undefined (aka T<undef>). If the argument is a defined value then the function returns T<true>, otherwise the function returns T<false>.

The argument is evaluated exactly once, like any expression. When it is a
function call the function runs: if it returns T<undef>, or no value at all,
the test answers T<false> and never reads the missing value. A definition
test therefore cannot fail on the very thing it is asked to inspect, even
when the call is the inner argument of the test.

This function is the counter function of R<ISUNDEF>.

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISUNDEF>, R<ISEMPTY>, R<TYPE>.
*/

COMMAND(ISDEF)
#if NOTIMP_ISDEF
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  if( CAR(nItem) == 0 ){
    /* Pas d'expression a inspecter. La grammaire exige un argument, mais un
       test de definition ne doit jamais etre la cause de la panne : sans
       cette garde, CAR(0) ferait lire CommandArray[-1]. */
    RETURN_FALSE
    }
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  /* L'argument peut etre un appel de fonction qui ne rend aucune valeur :
     ItemResult vaut alors NULL. memory_Type() est sur avec NULL et rend
     VTYPE_UNDEF, donc le test repond faux sans dereferencer quoi que ce soit. */
  if( memory_Type(ItemResult) == VTYPE_UNDEF ){
    RETURN_FALSE
    }
  RETURN_TRUE
#endif
END

/**ISUNDEF
=section test
=display ISUNDEF()

This function can be used to determine whether an expression is defined or undefined (aka T<undef>). If the argument is a defined value then the function returns T<false>, otherwise the function returns T<true>.

This function is the counter function of R<ISDEFINED>.

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISEMPTY>, R<TYPE>.
*/

COMMAND(ISUNDEF)
#if NOTIMP_ISUNDEF
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  if( CAR(nItem) == 0 ){
    /* Rien a inspecter : c'est exactement le cas "non defini". */
    RETURN_TRUE
    }
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_Type(ItemResult) == VTYPE_UNDEF ){
    RETURN_TRUE
    }
  RETURN_FALSE
#endif
END

/**ISEMPTY
=section test
=display ISEMPTY()

This function can be used to determine whether an expression holds an empty string. Because programmers tend to use the value T<undef> where empty string would be more precise the function returns T<true> if the argument is T<undef>. Precisely:

The function returns true if the argument is T<undef> or a string containing zero characters. Otherwise the function returns T<false>.

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<TYPE>.
*/

COMMAND(ISEMPTY)
#if NOTIMP_ISEMPTY
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  if( memory_IsUndef(ItemResult) ||
     ( TYPE(ItemResult) == VTYPE_STRING && STRLEN(ItemResult) == 0 ) ){
    RETURN_TRUE
    }
  RETURN_FALSE
#endif
END

/**TYPE
=section test
=display TYPE()

This function can be used to determine the type of an expression.
The function returns a numeric value that describes the type of the argument. Although the numeric values are guaranteed to be the one defined here it is recommended that you use the predefined symbolic constant values to compare the return value of the function against. The function return value is the following 

=itemize
=item T<SbTypeUndef>   0 if the argument is T<undef>.
=item T<SbTypeString>  1 if the argument is string.
=item T<SbTypeReal>    2 if the argument is real.
=item T<SbTypeInteger> 3 if the argument is integer.
=item T<SbTypeArray>   4 if the argument is an array.
=noitemize

See also R<ISARRAY>, R<ISSTRING>, R<ISINTEGER>, R<ISREAL>, R<ISNUMERIC>, R<ISDEFINED>, R<ISUNDEF>, R<ISEMPTY>.
*/

COMMAND(TYPE)
#if NOTIMP_TYPE
NOTIMPLEMENTED;
#else

  VARIABLE ItemResult;
  NODE nItem;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  /* evaluate the parameter */
  nItem = PARAMETERLIST;
  ItemResult = _EVALUATEEXPRESSION_A(CAR(nItem));
  ASSERTOKE;

  RESULT = NEWMORTALLONG;
  ASSERTNULL(RESULT)

  if( memory_IsUndef(ItemResult) ){
    LONGVALUE(RESULT) = 0;
    RETURN;
    }
  switch( TYPE(ItemResult) ){
    case VTYPE_LONG:
      LONGVALUE(RESULT) = 3;
      break;
    case VTYPE_DOUBLE:
      LONGVALUE(RESULT) = 2;
      break;
    case VTYPE_STRING:
      LONGVALUE(RESULT) = 1;
      break;
    case VTYPE_ARRAY:
      LONGVALUE(RESULT) = 4;
      break;
    }
#endif
END

/**ATN
=section math
=display ATN()

Calcule l'arc tangent de l'argument, en radians. C'est l'alias historique de
T<ATAN>. Si le resultat tient dans un entier, il est rendu en entier ; sinon
en reel.

T<ATN(undef)> vaut T<undef>, ou leve une erreur si l'option
T<RaiseMatherror> contient le bit T<sbMathErrUndef>.
*/
COMMAND(ATN)
#if NOTIMP_ATN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( atan(GETDOUBLEVALUE(Op1)) )

#endif
END

/**ATAN
=section math
=display ATAN()

Calcule l'arc tangent de l'argument, en radians. Si le resultat tient dans un
entier, il est rendu en entier ; sinon en reel.

T<ATAN(undef)> vaut T<undef>, ou leve une erreur si l'option
T<RaiseMatherror> contient le bit T<sbMathErrUndef>.
*/
COMMAND(ATAN)
#if NOTIMP_ATAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( atan(GETDOUBLEVALUE(Op1)) )

#endif
END
/**RAD2DEG
=section math
=display RAD2DEG()

Converts an angle from radians to degrees: T<RAD2DEG(PI())> is 180 within the
precision of the machine, and T<RAD2DEG(PI()/2)> is 90. The conversion is the
plain multiplication by T<180/PI>, so it is exact only as far as the real
arithmetic of the machine is; round the result with R<ROUND> when a whole number
is expected.

T<RAD2DEG(undef)> is T<undef>.

See also R<DEG2RAD>, R<PI>, R<TAN>
*/
COMMAND(RAD2DEG)
#if NOTIMP_RAD2DEG
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( GETDOUBLEVALUE(Op1) * (180.0/SBPI) )

#endif
END

/**DEG2RAD
=section math
=display DEG2RAD()

Converts an angle from degrees to radians: T<DEG2RAD(180)> is PI within the
precision of the machine, and T<DEG2RAD(90)> is PI/2. The conversion is the plain
multiplication by T<PI/180>, so it is exact only as far as the real arithmetic of
the machine is; round the result with R<ROUND> when a whole number is expected.

T<DEG2RAD(undef)> is T<undef>.

See also R<RAD2DEG>, R<PI>, R<SIN>
*/
COMMAND(DEG2RAD)
#if NOTIMP_DEG2RAD
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( GETDOUBLEVALUE(Op1) * (SBPI/180.0) )

#endif
END

/**TAN
=section math
=display TAN()

Calculates the tangent of the argument, given in radians: T<TAN(x)> is
T<SIN(x)/COS(x)>. The poles at odd multiples of PI/2 are not detected, because
the machine value of PI/2 does not make the cosine exactly zero: the result is a
very large real, not T<undef>.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<TAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<ATN>, R<TAN2>, R<COTAN>
*/
COMMAND(TAN)
#if NOTIMP_TAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( tan(GETDOUBLEVALUE(Op1)) )

#endif
END


/**TAN2
=section math
=display TAN2()

Calculates the tangent of the ratio of its two arguments: T<TAN2(x,y)> is
T<TAN(x/y)>. A zero divisor yields T<undef>, or the error T<sbMathErrDiv> when
the option T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<TAN2(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<TAN>, R<COTAN2>
*/
COMMAND(TAN2)
#if NOTIMP_TAN2
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1,Op2;
  double dop1,dop2,dResult;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  nItem = CDR(nItem);
  Op2 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op2)

  dop1 = GETDOUBLEVALUE(Op1);
  dop2 = GETDOUBLEVALUE(Op2);
  if( dop2 == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( tan(dop1/dop2) )

#endif
END


/**COTAN
=section math
=display COTAN()

Calculates the cotangent of the argument, given in radians: T<COTAN(x)> is
T<COS(x)/SIN(x)>, the inverse of R<TAN>. Where the sine is exactly zero, at
T<COTAN(0)> for instance, the result is T<undef>, or the error T<sbMathErrDiv>
when the option T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<COTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<TAN>, R<COTAN2>, R<SECANT>, R<COSECANT>
*/
COMMAND(COTAN)
#if NOTIMP_COTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dSin,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  dSin = sin(dop1);
  if( dSin == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( cos(dop1)/dSin )

#endif
END


/**COTAN2
=section math
=display COTAN2()

Calculates the cotangent of the ratio of its two arguments: T<COTAN2(x,y)> is
T<COTAN(x/y)>. A zero divisor, or a ratio whose sine is exactly zero, yields
T<undef>, or the error T<sbMathErrDiv> when the option T<RaiseMatherror> asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<COTAN2(undef,...)> is T<undef> or raises an error if the option
T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<COTAN>, R<TAN2>
*/
COMMAND(COTAN2)
#if NOTIMP_COTAN2
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1,Op2;
  double dop1,dop2,dArg,dSin,dResult;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  Op1 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op1)
  nItem = CDR(nItem);
  Op2 = EVALUATEEXPRESSION(CAR(nItem));
  NONULOP(Op2)

  dop1 = GETDOUBLEVALUE(Op1);
  dop2 = GETDOUBLEVALUE(Op2);
  if( dop2 == 0.0 ){
    RETURNUNDEF
    }
  dArg = dop1/dop2;
  dSin = sin(dArg);
  if( dSin == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( cos(dArg)/dSin )

#endif
END


/**ACTAN
=section math
=display ACTAN()

Calculates the arcus cotangent of the argument, the inverse of R<COTAN>, and
returns it in radians. The branch chosen is the continuous one, between 0 and PI:
T<ACTAN(1)> is PI/4, T<ACTAN(0)> is PI/2 and T<ACTAN(-1)> is 3*PI/4. It is
T<PI/2-ATN(x)>, which stays on the same branch on both sides of zero, unlike
T<ATN(1/x)>.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<ACTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<ATN>, R<COTAN>
*/
COMMAND(ACTAN)
#if NOTIMP_ACTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( SBPI/2.0 - atan(GETDOUBLEVALUE(Op1)) )

#endif
END


/**SECANT
=section math
=display SECANT()

Calculates the secant of the argument, given in radians: T<SECANT(x)> is
T<1/COS(x)>, the inverse of R<COS>. Where the cosine is exactly zero the result
is T<undef>, or the error T<sbMathErrDiv> when the option T<RaiseMatherror> asks
for it. As for R<TAN>, the machine value of PI/2 does not make the cosine exactly
zero, so the pole needs an argument the machine represents as such; T<SECANT(0)>
is 1.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<SECANT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<COS>, R<COSECANT>, R<ASECANT>
*/
COMMAND(SECANT)
#if NOTIMP_SECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dCos,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  dCos = cos(dop1);
  if( dCos == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( 1.0/dCos )

#endif
END


/**COSECANT
=section math
=display COSECANT()

Calculates the cosecant of the argument, given in radians: T<COSECANT(x)> is
T<1/SIN(x)>, the inverse of R<SIN>. Where the sine is exactly zero, at
T<COSECANT(0)> for instance, the result is T<undef>, or the error
T<sbMathErrDiv> when the option T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<COSECANT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<SIN>, R<SECANT>, R<ACOSECANT>
*/
COMMAND(COSECANT)
#if NOTIMP_COSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dSin,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  dSin = sin(dop1);
  if( dSin == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( 1.0/dSin )

#endif
END


/**ASECANT
=section math
=display ASECANT()

Calculates the arcus secant of the argument, the inverse of R<SECANT>, and
returns it in radians, between 0 and PI. The domain is T<|x|>=1>: outside it, and
for zero, the result is T<undef>, or the error T<sbMathErrDiv> when the option
T<RaiseMatherror> asks for it, the same rule as R<ASIN>.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<ASECANT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<SECANT>, R<ACOS>, R<ACOSECANT>
*/
COMMAND(ASECANT)
#if NOTIMP_ASECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 == 0.0 || ( dop1 > -1.0 && dop1 < 1.0 ) ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( acos(1.0/dop1) )

#endif
END


/**ACOSECANT
=section math
=display ACOSECANT()

Calculates the arcus cosecant of the argument, the inverse of R<COSECANT>, and
returns it in radians, between -PI/2 and +PI/2. The domain is T<|x|>=1>: outside
it, and for zero, the result is T<undef>, or the error T<sbMathErrDiv> when the
option T<RaiseMatherror> asks for it, the same rule as R<ASIN>.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<ACOSECANT(undef)> is T<undef> or raises an error if the option
T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<COSECANT>, R<ASIN>, R<ASECANT>
*/
COMMAND(ACOSECANT)
#if NOTIMP_ACOSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 == 0.0 || ( dop1 > -1.0 && dop1 < 1.0 ) ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( asin(1.0/dop1) )

#endif
END


/**AHSIN
=section math
=display AHSIN()

Calculates the area hyperbolic sine of the argument, the inverse of R<HSIN>:
T<AHSIN(x)> is the number T<y> for which T<HSIN(y)> is T<x>. It is defined
everywhere. Like R<HSIN> it works on plain real numbers, not on angles.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHSIN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HSIN>, R<AHCOS>, R<AHTAN>
*/
COMMAND(AHSIN)
#if NOTIMP_AHSIN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( asinh(GETDOUBLEVALUE(Op1)) )

#endif
END


/**AHCOS
=section math
=display AHCOS()

Calculates the area hyperbolic cosine of the argument, the inverse of R<HCOS>.
The domain is every argument greater than or equal to 1: below it the result is
T<undef>, or the error T<sbMathErrDiv> when the option T<RaiseMatherror> asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHCOS(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HCOS>, R<AHSIN>, R<AHCTAN>
*/
COMMAND(AHCOS)
#if NOTIMP_AHCOS
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 < 1.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( acosh(dop1) )

#endif
END


/**AHTAN
=section math
=display AHTAN()

Calculates the area hyperbolic tangent of the argument, the inverse of R<HTAN>.
The domain is every argument strictly between -1 and 1: outside it, and at the
two ends, the result is T<undef>, or the error T<sbMathErrDiv> when the option
T<RaiseMatherror> asks for it, because the tangent tends to infinity there.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HTAN>, R<AHSIN>, R<AHCOS>
*/
COMMAND(AHTAN)
#if NOTIMP_AHTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 <= -1.0 || dop1 >= 1.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( atanh(dop1) )

#endif
END


/**AHSECANT
=section math
=display AHSECANT()

Calculates the area hyperbolic secant of the argument, the inverse of
R<HSECANT>: T<AHSECANT(x)> is T<ACOSH(1/x)>. The domain is every argument above
zero and at most 1: outside it the result is T<undef>, or the error
T<sbMathErrDiv> when the option T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHSECANT(undef)> is T<undef> or raises an error if the option
T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<HSECANT>, R<AHCOS>, R<AHCOSECANT>
*/
COMMAND(AHSECANT)
#if NOTIMP_AHSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 <= 0.0 || dop1 > 1.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( acosh(1.0/dop1) )

#endif
END


/**AHCOSECANT
=section math
=display AHCOSECANT()

Calculates the area hyperbolic cosecant of the argument, the inverse of
R<HCOSECANT>: T<AHCOSECANT(x)> is T<ASINH(1/x)>. It is defined for every
non-zero argument; for zero the result is T<undef>, or the error
T<sbMathErrDiv> when the option T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHCOSECANT(undef)> is T<undef> or raises an error if the option
T<RaiseMatherror> is set in bit T<sbMathErrUndef>.

See also R<HCOSECANT>, R<AHSIN>, R<AHSECANT>
*/
COMMAND(AHCOSECANT)
#if NOTIMP_AHCOSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( asinh(1.0/dop1) )

#endif
END


/**AHCTAN
=section math
=display AHCTAN()

Calculates the area hyperbolic cotangent of the argument, the inverse of
R<HCTAN>: T<AHCTAN(x)> is T<AHTAN(1/x)>. The domain is every argument whose
absolute value is greater than 1: outside it, and at the two ends, the result is
T<undef>, or the error T<sbMathErrDiv> when the option T<RaiseMatherror> asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<AHCTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HCTAN>, R<AHTAN>, R<AHCOSECANT>
*/
COMMAND(AHCTAN)
#if NOTIMP_AHCTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  if( dop1 >= -1.0 && dop1 <= 1.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( atanh(1.0/dop1) )

#endif
END


/**HSIN
=section math
=display HSIN()

Calculates the hyperbolic sine of the argument: T<HSIN(x)> is
T<(EXP(x)-EXP(-x))/2>. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HSIN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<HCOS>, R<HTAN>, R<HCOSECANT>
*/
COMMAND(HSIN)
#if NOTIMP_HSIN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( sinh(GETDOUBLEVALUE(Op1)) )

#endif
END


/**HCOS
=section math
=display HCOS()

Calculates the hyperbolic cosine of the argument: T<HCOS(x)> is
T<(EXP(x)+EXP(-x))/2>, always at least 1. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HCOS(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<HSIN>, R<HTAN>, R<HSECANT>
*/
COMMAND(HCOS)
#if NOTIMP_HCOS
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( cosh(GETDOUBLEVALUE(Op1)) )

#endif
END


/**HTAN
=section math
=display HTAN()

Calculates the hyperbolic tangent of the argument: T<HTAN(x)> is
T<HSIN(x)/HCOS(x)>, between -1 and +1. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<HSIN>, R<HCOS>, R<HCTAN>
*/
COMMAND(HTAN)
#if NOTIMP_HTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( tanh(GETDOUBLEVALUE(Op1)) )

#endif
END


/**HCTAN
=section math
=display HCTAN()

Calculates the hyperbolic cotangent of the argument, the inverse of R<HTAN>:
T<HCTAN(x)> is T<HCOS(x)/HSIN(x)>. At zero the hyperbolic sine is exactly zero,
so T<HCTAN(0)> is T<undef>, or the error T<sbMathErrDiv> when the option
T<RaiseMatherror> asks for it. Everywhere else the result is above 1, or below
-1 for a negative argument.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HCTAN(undef)> is T<undef> or raises an error if the option T<RaiseMatherror> is
set in bit T<sbMathErrUndef>.

See also R<HTAN>, R<HSECANT>
*/
COMMAND(HCTAN)
#if NOTIMP_HCTAN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dTanh,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  dTanh = tanh(dop1);
  if( dTanh == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( 1.0/dTanh )

#endif
END


/**HSECANT
=section math
=display HSECANT()

Calculates the hyperbolic secant of the argument, the inverse of R<HCOS>:
T<HSECANT(x)> is T<1/HCOS(x)>. The hyperbolic cosine is never zero, so the
function is defined everywhere and returns a value between 0 and 1.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HSECANT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HCOS>, R<HCOSECANT>
*/
COMMAND(HSECANT)
#if NOTIMP_HSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dResult;

  GET_ONE_ARG

  RETURN_DOUBLE_VALUE_OR_LONG( 1.0/cosh(GETDOUBLEVALUE(Op1)) )

#endif
END


/**HCOSECANT
=section math
=display HCOSECANT()

Calculates the hyperbolic cosecant of the argument, the inverse of R<HSIN>:
T<HCOSECANT(x)> is T<1/HSIN(x)>. At zero the hyperbolic sine is exactly zero, so
T<HCOSECANT(0)> is T<undef>, or the error T<sbMathErrDiv> when the option
T<RaiseMatherror> asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

T<HCOSECANT(undef)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<HSIN>, R<HSECANT>
*/
COMMAND(HCOSECANT)
#if NOTIMP_HCOSECANT
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  double dop1,dSinh,dResult;

  GET_ONE_ARG

  dop1 = GETDOUBLEVALUE(Op1);
  dSinh = sinh(dop1);
  if( dSinh == 0.0 ){
    RETURNUNDEF
    }

  RETURN_DOUBLE_VALUE_OR_LONG( 1.0/dSinh )

#endif
END

/**MAX
=section math
=display MAX()

Selects and returns the greatest of one or more arguments. The comparison is
numeric: a string argument is converted to a number first. The winning argument
keeps its type, so T<MAX(1,2)> is an integer while T<MAX(1,2.5)> is a real.
When several arguments are equal the first one wins.

T<MAX(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<MIN>, R<IMAX>
*/
COMMAND(MAX)
#if NOTIMP_MAX
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  long lBest = 0;
  double dBest = 0.0,dValue;
  int iBestIsLong = 0,iFirst = 1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  while( nItem ){
    Op1 = EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    dValue = GETDOUBLEVALUE(Op1);
    if( iFirst || dValue > dBest ){
      dBest = dValue;
      lBest = GETLONGVALUE(Op1);
      iBestIsLong = ISINTEGER(Op1);
      iFirst = 0;
      }
    nItem = CDR(nItem);
    }

  if( iBestIsLong ){
    RETURN_LONG_VALUE( lBest )
    }
  RETURN_DOUBLE_VALUE( dBest )
#endif
END

/**MIN
=section math
=display MIN()

Selects and returns the smallest of one or more arguments. The comparison is
numeric: a string argument is converted to a number first. The winning argument
keeps its type, so T<MIN(1,2)> is an integer while T<MIN(1,0.5)> is a real.
When several arguments are equal the first one wins.

T<MIN(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<MAX>, R<IMIN>
*/
COMMAND(MIN)
#if NOTIMP_MIN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  long lBest = 0;
  double dBest = 0.0,dValue;
  int iBestIsLong = 0,iFirst = 1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  while( nItem ){
    Op1 = EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    dValue = GETDOUBLEVALUE(Op1);
    if( iFirst || dValue < dBest ){
      dBest = dValue;
      lBest = GETLONGVALUE(Op1);
      iBestIsLong = ISINTEGER(Op1);
      iFirst = 0;
      }
    nItem = CDR(nItem);
    }

  if( iBestIsLong ){
    RETURN_LONG_VALUE( lBest )
    }
  RETURN_DOUBLE_VALUE( dBest )
#endif
END

/**IMAX
=section math
=display IMAX()

Selects the greatest of one or more arguments, comparing them as R<MAX> does,
and returns the position of the winner instead of its value: T<IMAX(5,3,9)> is 2.

Positions start at zero, like the first index of an array. When several
arguments are equal the first one wins, so T<IMAX(3,3)> is 0.

T<IMAX(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<MAX>, R<IMIN>
*/
COMMAND(IMAX)
#if NOTIMP_IMAX
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  long lBestIndex = 0,lIndex = 0;
  double dBest = 0.0,dValue;
  int iFirst = 1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  while( nItem ){
    Op1 = EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    dValue = GETDOUBLEVALUE(Op1);
    if( iFirst || dValue > dBest ){
      dBest = dValue;
      lBestIndex = lIndex;
      iFirst = 0;
      }
    lIndex ++;
    nItem = CDR(nItem);
    }

  RETURN_LONG_VALUE( lBestIndex )
#endif
END

/**IMIN
=section math
=display IMIN()

Selects the smallest of one or more arguments, comparing them as R<MIN> does,
and returns the position of the winner instead of its value: T<IMIN(5,3,9)> is 1.

Positions start at zero, like the first index of an array. When several
arguments are equal the first one wins, so T<IMIN(3,3)> is 0.

T<IMIN(undef,...)> is T<undef> or raises an error if the option T<RaiseMatherror>
is set in bit T<sbMathErrUndef>.

See also R<MIN>, R<IMAX>
*/
COMMAND(IMIN)
#if NOTIMP_IMIN
NOTIMPLEMENTED;
#else

  NODE nItem;
  VARIABLE Op1;
  long lBestIndex = 0,lIndex = 0;
  double dBest = 0.0,dValue;
  int iFirst = 1;

  /* this is an operator and not a command, therefore we do not have our own mortal list */
  USE_CALLER_MORTALS;

  nItem = PARAMETERLIST;
  while( nItem ){
    Op1 = EVALUATEEXPRESSION(CAR(nItem));
    NONULOP(Op1)
    dValue = GETDOUBLEVALUE(Op1);
    if( iFirst || dValue < dBest ){
      dBest = dValue;
      lBestIndex = lIndex;
      iFirst = 0;
      }
    lIndex ++;
    nItem = CDR(nItem);
    }

  RETURN_LONG_VALUE( lBestIndex )
#endif
END

