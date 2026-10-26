<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `dbg`

Source : `src/extensions/dbg/interface.c`

## Print the value of a variable into a string

This function should be used to get the textual representation of a
ScriptBasic `VARIABLE`.

/*FUNCTION*/
int SPrintVariable(pDebuggerObject pDO,
                   VARIABLE v,
                   char *pszBuffer,
                   unsigned long *cbBuffer
  ){
/*noverbatim

- `pDO` is the debugger object
- `v` is the variable to print
- `pszBuffer` is pointer to the buffer that has to have at least
- `cbBuffer` number of bytes available

The function returns zero on success.

The function returns 1 if the buffer is not large enough. In this case the
number returned in `*cbBuffer` will be the size of the buffer needed. It may
happen in case the buffer is extremely short that even the returned size is not
enough. Choosing a buffer length of 80 bytes or so ensures that either the
result fits into the buffer or the returned number is large enough to hold
the result.

Note that the number can be extremely large in case the variable is a string. In
this case all the characters are copied into the result and non-printable characters
are converted to hex.

The buffer should be large enough to hold the "->->->->->...->" string representing the
references and the number or string.

## Print the value of a variable into a string

This fucntion prints a variable string representation into a buffer.
The name of the variable is given in the variable `pszName`.

The fucntion first searches the variable and then calls the function
`SPrintVariable` to print the value.

The fucntion first tries to locate the variable as local variable.
For this not the normal debug stack pointer is used, but rather the
`StackListPointer`. This allows the client to print local
variables levels higher than the bottom of the stack.

If the function succeeds finding the variable it returns the return value of the
function `SPrintVariable`. If the variable is not found it returns 2.

/*FUNCTION*/
int SPrintVarByName(pDebuggerObject pDO,
                    pExecuteObject pEo,
                    char *pszName,
                    char *pszBuffer,
                    unsigned long *cbBuffer
  ){
/*noverbatim

## Get the source line number for a given node

/*FUNCTION*/
long GetSourceLineNumber(pDebuggerObject pDO,
                         long PC
  ){
/*noverbatim
