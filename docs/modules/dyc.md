<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `dyc`

Source : `src/extensions/dyc/interface.c`

## DYC("format", arguments)

```
import dyc.sbm
print dyc::dyc("ms,i,USER32.DLL,MessageBox,PZZL",0,"test message","title",3)
```

This function calls an arbitrary function from an arbitrary dll. The first argument to the function has to be a format string
and the rest of the arguments are the arguments for the function to be called. The format string has to specify the calling
convention of the function, the return value, the name of the DLL and the function to call and the argument types. These have
to be specified one after the other separated by commas. The format string should not contain space.

The format string has the following format:

`"Xc,Xr,DllName,FunctionName,Xargs"`

where

- `Xc` specifies the calling convention
- `Xr` specifies the return value
- `DllName` is the name of the DLL
- `FunctionName` is the name of the function
- `Xargs` specifies the arguments

When the function is called the arguments are converted from their BASIC value and the function is called according to the
format specification. However note that a misformed format string can cause access violation in the program and thus stopping
the process. Therefore it is recommended that you fully debug your code and the way you use this function. It may be a wise
idea not to install this module on a server where different programmers can develop their programs and run in shared process
n multiple threads. For example a hosted web server running the Eszter SB Application Engine can be stopped by a BASIC program
using this external module.

In the following I describe the format string specifiers.

- `Xc` CALLING CONVENTION

The calling convention can be one, two or at most three characters. The character `m` or `M` means that the code was compiled
using Microsoft compiler. This is the default behavour, thus there is no need to specify this. The opposite is `b` or `B`
meaning that the code was compiled using Borland compiler. The difference between these two compilers is how the return value
is passed back to the caller. You should not use both `b` and `m` at a time. Actually `m` will be ignored.

The calling convention can also be `s` or `S` meaning standard callign convention or `c` or `C` meaning language C
calling convention. Only one of them is to be used in a function call. If you are callign some function from a Windows system
DLL then it is certainly `s`. If you do not know which to use write a small test program and experiment.

The difference between standard and C calling convention is the order of the arguments placed on the stack and also who the
responsible is to clean the arguments from the stack (the called function or the calling code).

Finally you can specify `4` or `8` to specify that the function is returning a four or eight-byte floating point number.
Although this is a kind of return value specification, it is stated here, because this affects the calling convention. These
values are returned not in a memory place from the function but rather in the co-processor register and function `dyc` has
to know to fetch them from there rather than expection the function to return a four or eight-byte memory chunk.

- `Xr` RETURN VALUE

The return value should be specified using a single character. This can be:

`i` or `I` `int`, `l` or `L` `long`, `p` or `P` pointer, `f` or `F` `float`,
`d` or `D` for `double` or `v` or `V` for `__int64`.

The int and long types are converted to a BASIC integer number, which is stored as a long in ScriptBasic. float and double values
are returned as real number, which is stored as double in ScriptBasic. A pointer value is converted to long and is returned in an
integer value. An `__int64` value is returned as an 8 byte string copiing the bytes of the original `__int64` value to the bytes
of the BASIC string.

- `DllName`

This parameter has to specify the name of the DLL. This name will be used to load the DLL calling the system function `LoadLibrary`.
This means that the name can but also may not include the full path to the file. In the latter case the system function will search
the path for the DLL as specified int he Microsoft provided documentation for the function `LoadLibrary`.

When a function from a certain DLL is called first the module loads the DLL and when the BASIC program finishes and the module is
unloaded it unloads all DLLs it loaded. Any DLL by the module will only be loaded once. Whent he module is used in a multi-thread
environment the interpreter threads load and unload the DLLs independently. If you do not understand what it means then just ignore
this explanation: nothing to worry about.

- `FunctionName`

The name of the function to be called from the certain DLL. If the function is not present in the DLL then the program tries to
use the function with the original name with an 'A' appended to it. Many system functions happen to have this format in the
Windows librares.

- `Xargs` argument types

This parameter should specify the arguments. It has to have as many character as many arguments there are. Each character should specify
exactly one argument and will control how the actual BASIC arguments are converted to their native format. For each argument one of the
following characters can be used:

- `1`
- `2`
- `4`
- `8` specifies that the argument is an arbitrary 1-, 2-, 4- or 8-byte argument. The BASIC argument should be string value and
should have at least as many characters as needed (1, 2, 4 or 8 as specified).

It is possible to use undefined, integer or real value for 1-, 2- or 4-byte values. In this case the value will be converted to integer and
the bytes of the value will be used as argument. In case of 8-byte argument the BASIC argument is converted to string.

- `c` specifies that the argument is a single character. If the BASIC argument is a string then the first character of the string is
used. If the argument is a real number or an integer number then the value will be used as ASCII code. If the argument is `undef` or if the
string has no characters in it then the value will be zero.

- `s` specifies that the argument is a short(2-byte) value. The BASIC argument is converted to an integervalue if needed and
truncated to two bytes if needed.

- `f` specifies that the argument is a float value. The BASIC argument is converted to a real value and its precision is decreased
from double to float.
- `h`
- `p`
- `l` specifies that the argument is a handle, pointer or long. In these cases the BASIC argument is converted to an integer value if
needed.

- `z` specifies that the argument is a pointer that should point to a zero terminated string. The BASIC argument is converted to string and
a pointer to the start of the string is passed as actual argument. Note that BASIC strings are not zero terminated and the function `dyc`
does not append the terminating zero character to the string. Thus you have to append a zero character to the BASIC string before you
pass it as zero terminated string. For example:

```
import dyc.sbm

a$ = "message"
a$ &= " text"

REM Make the string zero character terminated
a$ &= chr$(0)

print dyc::dyc("ms,i,USER32.DLL,MessageBox,PZZL",0,a$,"title",3)
```

On the other hand you can safely use string constants as argument like in the example above `"title"` because string constants in ScriptBasic
contain an extra zero character following their normal characters.

- `d` specifies that the argument is a double value. The BASIC argument is converted to a real value if needed, which is stored in BASIC
internally as double and is passed to the function.

Note that this is a wise idea to write a wrapper function in BASIC that gets the arguments performs some checks if needed and calls the
module `dyc` instead of putting the dyc call into the main BASIC code.
