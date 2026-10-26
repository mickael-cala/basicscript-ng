/*if.c

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

#include "../command.h"

/*POD
=H 'if' conditional statement
CUT*/

/* Convert a variable to 0 or 1 for SLIF, IF, ELSIF commands */
static int IsTrue(VARIABLE v){

  if( memory_IsUndef(v) )return 0;/* undef is false*/
  switch( TYPE(v) ){
      case VTYPE_LONG:
        if( LONGVALUE(v) )return 1; else return 0;
      case VTYPE_DOUBLE:
        if(  DOUBLEVALUE(v) )return 1; else return 0;
      case VTYPE_STRING:
        if( * STRINGVALUE(v) == (char)0 )return 0; else return 1;
      case VTYPE_ARRAY:
        return 0;
      }
  return 0;
  }


/*POD
=section SLIF
=H Single line 'IF' conditional statement

This command is the single-line form of R<IF> and is emitted only by the
syntax analyser; it is not written in the source under this name. It has the
same arguments and the same jump machinery as the block T<IF>: after the
condition comes the label of the branch that follows the current one. The work
is done by R<COMMAND(IF)>.

CUT*/
COMMAND(SLIF)/* Single line IF */
#if NOTIMP_SLIF
NOTIMPLEMENTED;
#else
  NODE nItem;
  NODE nGoForward;
  NODE nCode;
  VARIABLE ItemResult;

  while(1){
    nItem = PARAMETERNODE;
    ItemResult = EVALUATEEXPRESSION(nItem);
    ASSERTOKE;
    NEXTPARAMETER;
    /* le ELSE, le ELSEIF ou la commande qui suit la branche */
    nGoForward = PARAMETERNODE;
    nCode = nGoForward ? CAR(nGoForward) : 0;

    /* Seuls le ELSE et le ELSEIF de cet IF d'une ligne sont des marqueurs.
       Le ELSE, le ELSEIF et le ENDIF d'un IF de bloc englobant sont de
       simples commandes sur ce chemin : y sauter les laisse faire leur
       travail. */
    switch( nCode ? OPCODE(nCode) : 0 ){
      case CMD_ELSE_SL:
        /* aller a la commande de la branche ELSE */
        nGoForward = CDR(nGoForward);
        nCode = nGoForward ? CAR(nGoForward) : 0;
        break;
      case CMD_ELSIF_SL:
        /* evaluer ce ELSEIF */
        break;
      default:;
      }

    if( IsTrue(ItemResult) ){
      SETPROGRAMCOUNTER(pEo->CommandArray[pEo->ProgramCounter-1].Parameter.NodeList.rest);
      RETURN;
      }

    if( nCode && OPCODE(nCode) == CMD_ELSIF_SL ){
      pEo->ProgramCounter = nGoForward;
      _ActualNode = PROGRAMCOUNTER;
      }else{
      SETPROGRAMCOUNTER(nGoForward);
      RETURN;
      }
    }
#endif
END


/**IF
=section misc
=title IF condition THEN

Conditional execution. There are two different ways to use this command: single
line T<IF> and multi line T<IF>.

A single line T<IF> has the form

=verbatim
IF condition THEN command
=noverbatim

Since 2026-10 the single line form accepts a T<ELSE> part and a chain of
T<ELSEIF> branches, each branch being one command on the same physical line:

=verbatim
IF condition THEN command ELSEIF condition THEN command ELSE command
=noverbatim

Each branch may hold exactly one command; a command that opens a construct of
its own (T<IF>, T<FOR>, T<WHILE>, T<SUB>, T<FUNCTION>, T<REPEAT> and the
declarations) is refused during analysis, because the branch would run past the
end of the line. Use the multi line form for those.

The multi line T<IF> should not contain any command directly after the keyword T<THEN>. It should have the format:

=verbatim
IF condition THEN
  commands
ELSE
  commands
END IF
=noverbatim

The T<ELSE> part of the command is optional, thus the command can have the format

=verbatim
IF condition THEN
  commands
END IF
=noverbatim

as well. To be very precise the full syntax of the multi-line T<IF> command is:

=verbatim
IF condition THEN
 commands
[ ELSEIF
  commands
  ... ]
[ ELSE
  commands ]
END IF | ENDIF
=noverbatim

You can use as many T<ELSEIF> branches as you like and at most one T<ELSE> branch,
in both forms.

Only the single keyword T<ELSEIF> is accepted: the two-word form T<ELSE IF> and
the variants T<ELSIF> and T<ELIF> were removed from the language. T<END IF> in
two words and the single keyword T<ENDIF> remain equivalent.

*/
/*POD
=section IF
=H 'if' conditional statement

The IF conditional statement is implemented in a very special way. This is because IF can
be very compound on several lines. If an IF expression is false then the command checks
if there is any daingling ELSEIF and in case there is any it goes on and on until there
is any ELSEIF with a true expression or there is no more ELSEIF but ELSE or ENDIF.

CUT*/
COMMAND(IF)
#if NOTIMP_IF
NOTIMPLEMENTED;
#else
  NODE nItem;
  NODE nGoForward;
  NODE nCode;
  VARIABLE ItemResult;

  while(1){
    nItem = PARAMETERNODE;
    ItemResult = EVALUATEEXPRESSION(nItem);
    ASSERTOKE;
    NEXTPARAMETER;
    /* if the condition is FALSE then we step forward to the ENDIF/ELSIF or ELSE statement */
    nGoForward = PARAMETERNODE;
    nCode = CAR(nGoForward); /* the code node of the instruction */

    switch( OPCODE(nCode) ){
      case CMD_ELSE:
      case CMD_ENDIF:
        /* go AFTER the ELSE or ENDIF statement */
        nGoForward = CDR(nGoForward);
        break;
      case CMD_ELSIF:
        break;
      default:; /*TODO: report an internal error*/
      }

    /* If the expression is true then we have to continue with the next statement. This is
       usually the default, but here we have to explicitely set the program counter because
       in case we wen onto an ELSEIF the execution would start on the command after the
       IF statement. 

       If the expression is false then we either continue the execution after the ELSE or ENDIF
       statement or set the program counter to the ELSEIF statement and loop back to execute it
       if it were an IF statement.
    */
    if( IsTrue(ItemResult) ){
      SETPROGRAMCOUNTER(pEo->CommandArray[pEo->ProgramCounter-1].Parameter.NodeList.rest);
      RETURN;
      }else{
      if( OPCODE(nCode) == CMD_ELSIF ){
        pEo->ProgramCounter = nGoForward;
        _ActualNode=PROGRAMCOUNTER;
        }else{
        SETPROGRAMCOUNTER(nGoForward);
        RETURN;
        }
      }
    }
#endif
END

COMMAND(ELSE)
#if NOTIMP_ELSE
NOTIMPLEMENTED;
#else


  SETPROGRAMCOUNTER(PARAMETERNODE);

#endif
END

COMMAND(ENDIF)
#if NOTIMP_ENDIF
NOTIMPLEMENTED;
#else

  /* this command does not do anything */
#endif
END

/*POD
=section ELSIF
=H 'elseif' conditional statement

Note that this piece of code does nothing. The execution gets here only when a previous
IF or ELSEIF was true. In that case the expression has to be ignored and the ELSEIF treated
like if it was an ELSE.

The expression after an ELSEIF is checked by the preceeding IF command.

CUT*/
COMMAND(ELSIF)
#if NOTIMP_ELSIF
NOTIMPLEMENTED;
#else

  NEXTPARAMETER;/* skip the expression */
  SETPROGRAMCOUNTER(PARAMETERNODE);

#endif
END

/*POD
=section ELSIF
=H 'elseif' and 'else' of the single-line IF

These two opcodes are emitted by the analyser when the keyword is followed by a
command on the same line (R<ex_IsCommandELSE()> and R<ex_IsCommandELSIF()>).
They are the same commands as R<ELSE> and R<ELSIF>: reached after a true branch
they jump to the label left by the analyser, which is the next ELSE/ELSEIF of
the chain or the statement after it. The difference is only in the opcode, so
that the single-line IF can tell its own markers from the ELSE and ELSEIF of an
enclosing block IF, which are plain jump targets for it.

CUT*/
COMMAND(ELSE_SL)
#if NOTIMP_ELSE
NOTIMPLEMENTED;
#else
  IDENTICAL_COMMAND(ELSE)
#endif
END

COMMAND(ELSIF_SL)
#if NOTIMP_ELSIF
NOTIMPLEMENTED;
#else
  IDENTICAL_COMMAND(ELSIF)
#endif
END
