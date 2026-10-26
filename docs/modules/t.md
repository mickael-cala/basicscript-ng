<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `t`

Source : `src/extensions/t/interface.c`

## Tools module

This module contains handy functions. These are general, system-independant functions
that can be used to write effective programs, but are more complex or special
purpose to include them into the core language.

The functions include file handling, string handling, array to string conversion and
many more. See below:

## s = t::ArrayToString(Array)

Call this function to convert an array to a binary string. This string
can be saved to a file or stored in memory and the function `StringToArray`
can convert it back to array. Note that arrays and associative arrays
are not different in ScriptBasic.

Note that the include file `t.bas` also defines the `Array2String` name to the same
function.

Also note that though the name of the function is `ArrayToString` you can convert
any variable to binary string using this function. However this is most useful
when the argumentum is an array.

## s = t::ArrayToXML(Array)

Call this function to convert an array to a string containing the XML
representation of the array. This string can be saved to a file or stored in
memory and the function `XMLToArray` can convert it back to array.
Note that arrays and associative arrays are not different in ScriptBasic.

Note that the include file `t.bas` also defines the `Array2XML` name to
the same function.

Also note that though the name of the function is `ArrayToXML` you can convert
any variable to binary string using this function. However this is most useful
when the argumentum is an array.

The generated XML is not really for human reading. It is without any new
line characters. Because the strings can be binary they are also encoded with
all new line characters to hexa encoded XML character `0A`. If you want to
prettyprint the output you have to format it. The purpose of this function is
to provide a simple XML conversion routine to generate a format that can
be processed by other applications.

## t::StringToArray array,string

Call this subroutine to convert a string that was created as the result of
calling the function `ArrayToString`. Note that the include file `t.bas`
also defines the `String2Array` function name to get to the same function.

If the string is invalid the function raises `COMMAND_ERROR_ARGUMENT_RANGE`
error that can be captured using the command `ON ERROR GOTO`.

It is not an error if there are excess characters in the string after the
string that was originally created by the function `ArrayToSTring`

## t::SaveString file,string

Save a string to a file. This subroutine opens the file, creates it if it did
not exist and prints the binary string to the file and closes the file.
See also `loadstring`

## s = t::LoadString(file)

This function opens a file, reads its content into a new string variable and returns it.
See also `savestring`

## Calculate MD5
Calculate the MD5 value of a string

```
  q = t::MD5("string")
```

Note that the return value is a 16 byte binary strig thus do not try to print it
without first converting to some printable string.

## s = t::ArrayToStringMD5(Array)

This function is the same as `ArrayToString` with the extra feature that this function
appends the MD5 fingerprint of the created string to the result, generating 16 characters
longer result. This is a bit slower than `ArrayToString` and creates longer string, but also
ensures integrity and can eliminate file reading and write errors.

See also `StringToArrayMD5`.

Note that because `StringToArray` does not care excess characters at the end of the string
you can use `StringToArray` to convert a string back to array that was created using
`ArrayToStringMD5`.

## t::StringToArrayMD5 array,string

This function is the same as `StringToArray` with the extra functionality that this
function is the counterpart of the function `ArrayToStringMD5` and this function
does check MD5 integrity of the strig before starting to convert back.

## t::Exit(n)

This function calls the operating system function `exit()` to terminate an
application. A similar result can be reached in the command line version
of the interpreter when an error is not handled. The differences that the
programmer **has to be aware** before using this function are the following:

- This function exits the program, even if there are higher level error
handling routines defined.
- The interpreter finalization routines are not called. This may cause
problem in some applications, where resources need specific release.
- This function will stop the process and not the thread. It means that
calling this function from a multi-thread variation, like Eszter SB Application
Engin will stop all running threads, all interpreters and quits the
service.
