<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Manuel du langage

Reference des commandes et fonctions du noyau, extraite des
commentaires de `src/commands/`. Le texte est celui des sources,
en anglais ; les documents de `docs/` sont en francais.

## Sommaire

- [array](#array) — 4 entrees
- [autre](#autre) — 2 entrees
- [error](#error) — 4 entrees
- [errord](#errord) — 1 entree
- [file](#file) — 35 entrees
- [loop](#loop) — 6 entrees
- [math](#math) — 57 entrees
- [misc](#misc) — 35 entrees
- [process](#process) — 4 entrees
- [string](#string) — 36 entrees
- [test](#test) — 7 entrees
- [time](#time) — 21 entrees

## array

### DIM

Declares an array of known size. All the elements from index zero up to the
declared index are `undef`; the array exists immediately, so
`UBOUND(variable)` reports the declared index before any element is written.

```
DIM a[10]
print ubound(a) & " " & lbound(a)      ' 10 0
print isundef(a[7])                     ' -1
```

Each index between brackets adds a dimension, and each dimension is allocated
in every element of the previous one. `DIM m[2,3]` therefore builds a table
where `m[0]` to `m[2]` are arrays of indices zero to three, which is what the
access `m[i,j]` expects.

The size expressions are evaluated once, when the command runs. An undefined,
negative or non numeric size is an error (`&H7` or `&H8`), and the variable
keeps its former value.

`DIM` replaces the former value of the variable: it never preserves the
elements of an array already there. It also does not turn the array into a
fixed size one; accessing an index above the declared one still extends the
array as it does everywhere in the language, so `DIM` fixes the initial
extent, not a boundary.

The form without brackets, `DIM a`, and an access nested in the left value,
`DIM a[1][2]`, are refused during the analysis (`&H77`), because this
command declares whole arrays only.

### ISARRAY()

This function can be used to determine whether a variable holds array value or ordinary value. If the variable passed as argument to the function is an array then the function returns `true`, otherwise the function returns `false`.

See also `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### LBOUND()

This function can be used to determine the lowest occupied index of an array. Note that arrays are increased in addressable indices automatically, thus it is not an error to use a lower index that the value returned by the function `LBOUND`. On the other hand all the element having index lower than the returned value are `undef`.

The argument of this function has to be an array. If the argument is an ordinary value, or a variable that is not an array the value returned by the function will be `undef`.

`LBOUND(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `UBOUND`.

### UBOUND()

This function can be used to determine the highest occupied index of an array. Note that arrays are increased in addressable indices automatically, thus it is not an error to use a higher index that the value returned by the function `UBOUND`. On the other hand all the element having index larger than the returned value are `undef`.

The argument of this function has to be an array. If the argument is an ordinary value, or a variable that is not an array the value returned by the function will be `undef`.

`UBOUND(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `LBOUND`.

## autre

### LOCK REGION

Lock a region of a file. The region starts with the record `start` and ends with the record `end` including both end positions. The length of a record in the file is given when the file is opened using the statement `OPEN`.

The mode can be `read`, `write` and `release`. The command works similar as whole file locking, thus it is recommended that you read the differences of the operating systems handling locking in the section of file locking for the command `LOCK`.

### OPEN DIRECTORY

Open a directory to retrieve the list of files.

- `dir_name` is the name of the directory.
- `pattern` is a wild card pattern to filter the file list.
- `option` is an integer value that can be composed AND-ing some of the following values
- `SbCollectDirectories`	Collect the directory names as well as file names into the file list.
- `SbCollectDots`	Collect the virtual . and .. directory names into the list.
- `SbCollectRecursively`	Collect the files from the directory and from all the directories below.
- `SbCollectFullPath`	The list will contain the full path to the file names. This means that the file names returned by the function `NextFile` will contain the directory path specified in the open directory statement and therefore can be used as argument to file handling commands and functions.
- `SbCollectFiles`	Collect the files. This is the default behavior.
- `SbSortBySize`	The files will be sorted by file size.
- `SbSortByCreateTime`	The files will be sorted by creation time.
- `SbSortByAccessTime`	The files will be sorted by access time.
- `SbSortByModifyTime`	The files will be sorted by modify time.
- `SbSortByName`	The files will be sorted by name. The name used for sorting is the bare file name without any path.
- `SbSortByPath`	The files will be sorted by name including the path. The path is the relative to the directory, which is currently opened. This sorting option is different from the value `sbSortByName` only when the value `sbCollectRecursively` is also used.
- `SbSortAscending`	Sort the file names in ascending order. This is the default behavior.
- `SbSortDescending`	Sort the file names in descending order.
- `SbSortByNone`	Do not sort. Specify this value if you do not need sorting. In this case directory opening can be much faster especially for large directories.
- `dn` is the directory number used in later references to the opened directory.

Note that this command can execute for a long time and consuming a lot of memory especially when directory listing is requested recursively. When the command is executed it collects the names of the files in the directory or directories as requested and builds up an internal list of the file names in the memory. The command `NEXTFILE` uses the list to retrieve the next file name from the list.

This implies to facts:

- The function `NEXTFILE` will not ever return a file name that the file was created after, and did not exist when the command `OPEN DIRECTORY` was executed.
- Using `CLOSEDIR` after the list of the files is not needed as soon as possible is a good idea.

Using a directory number that was already used and not released calling `CLOSEDIR` raises an error.

If the list of the files in the directory can not be collected the command raises error.

See also `CLOSEDIR` and `NEXTFILE`.

## error

### ERROR() or ERROR n

The keyword `ERROR` can be used as a command as well as a built-in function. When used as a function it returns the error code that last happened. The error codes are defined in the header file `error.bas` that can be included with the command `import`. The error code is a vital value when an error happens and is captured by some code defined after the label referenced by the command `ON ERROR GOTO`. This helps the programmer to ensure that the error was really the one that the error handling code can handle and not something else.

If the keyword is used as a command then it has to be followed by some numeric value. The command does not ever return but generates an error of the code given by the argument.

Take care when composing the expression following the keyword `ERROR`. It may happen that the expression itself can not be evaluated due to some error conditions during the evaluation of the expression. The best practice is to use a constant expression using the symbolic constants defined in the include file `error.bas`.

Note that the codes defined in the include file are version dependant.

If an error is not captured by any `ON ERROR GOTO` specified error handler then the interpreter stops. The command line variation passes the error code to the executing environment as exit code. In other word you can exit from a BASIC program

### ON ERROR GOTO

Set the entry point of the error handling routine. If the argument is `NULL` then the error handling is switched off.

### ON ERROR RESUME

Setting `ON ERROR RESUME` will try to continue execution on the label or on the next statement when an error occurs without any error handling code.

See also `ONERRORGOTO`, `RESUME` and `ERROR`.

### RESUME [ label | next ]

Resume the program execution after handling the error. `RESUME` without argument tries to execute the same line again that caused the error. `RESUME NEXT` tries to continue execution after the line that caused the error. `RESUME label` tries to continue execution at the specified label.

See also `ONERRORGOTO`, `ONERRORRESUME` and `ERROR`.

## errord

### ERROR$() or ERROR$(n)

Returns the English sentence describing the last error if the argument is not defined or returns the English sentence describing the error for the error code `n`.

If the error code `n` provided as argument is negative or is above all possible errors then the result of the function is `undef`.

## file

### BINMODE [ # fn ] | input | output

Set an opened file handling to binary mode.

The argument is either a file number with which the file was opened or one of keywords `input` and `output`. In the latter case the standard input or output is set.

See also `TEXTMODE`

### CLOSE DIRECTORY

Close an opened directory and release all memory that was used by the file list.

See also `OPENDIR`.

### CLOSE [ # ] fn

Close a previously successfully opened file. The argument of the command is the file number that was used in the command `OPEN` to open the file.

If the file number is not associated with a successfully opened file then error is raised.

### CRYPT()

This function returns the encoded DES digest of the string using the salt
as it is used to encrypt passwords under UNIX.

Note that only the first 8 characters of the string are taken into account.

### DELETE file/directory_name

This command deletes a file or an **empty** directory. You can not delete a directory which contains files or subdirectories.

If the file or the directory can not be deleted an error is raised. This may happen for example if the program trying to delete the file or directory does not have enough permission.

See `DELTREE` for a more powerful and dangerous delete.

### DELTREE file/directory_name

Delete a file or a directory. You can use this command to delete a file the same way as you do use the command `DELETE`. The difference between the two commands `DLETE` and `DELTREE` comes into place when the program deletes directories.

This command, `DELTREE` forcefully tries to delete a directory even if the directory is not empty. If the directory is not empty then the command tries to delete the files in the directory and the subdirectories recursively.

If the file or the directory cannot be deleted then the command raises error. However even in this case some of the files and subdirectories may already been deleted.

### EOD()

Checks if there is still some file names in the directory opened for reading using the directory number `dn`.

See also `NEXTFILE`.

### EOF(n)

This function accepts one parameter, an opened file number. The return value is `true` if and only if the reading has reached the end of the file.

### FILEACCESSTIME()

Get the time the file was accessed last time.

This file time is measured in number of seconds since January 1, 1970 00:00. Note that the different file systems store the file time with different precision. Also FAT stores the file time in local time while NTFS for example stores the file time as GMT. This function returns the value rounded to whole seconds as returned by the operating system. Some of the file systems may not store all three file time types:

- the time when the file was created,
- last time the file was modified and
- last time the file was accessed

Trying to get a time not defined by the file system will result `undef`.

### FILECOPY filename,filename

Copy a file. The first file is the existing one, the second is the name of the new file. If the destination file already exists then the command overwrites the file. If the destination file is to be created in a directory that does not exist yet then the directory is created automatically.

In the current version of the command you can not use wild characters to specify more than one file to copy, and you can not concatenate files using this command. You also have to specify the full file name as destination file and this is an error to specify only a directory where to copy the file.

Later versions of this command may implement these features.

If the program can not open the source file to read or the destination file can not be created then the command raises error.

### FILECREATETIME()

Get the time the file was modified last time. See also the comments on the function `FTACCESS`.

### FILEEXISTS()

Returns `true` if the named file exists.

### FILELEN()

Get the length of a named file. If the length of the file can not be determined (for example the file does not exists, or the process running the code does not have permission to read the file) then the return value is `undef`.

This function can be used instead of `LOC` when the file is not opened by the BASIC program.

### FILEMODIFYTIME()

Get the time the file was modified last time. See also the comments on the function `FTACCESS`.

### FILEOWNER(FileName)

This function returns the name of the owner of a file as a string. If the file does not exist or for some other reason the owner of the file can not be determined then the function returns `undef`.

### FREEFILE()

This function returns a free file number, which is currently not associated with any opened file. If there is no such file number it returns `undef`.

The returned value can be used in a consecutive `OPEN` statement to specify a file number. Another way to get a free file number is to set a variable to hold the integer value zero and use the variable as file number in the statement `OPEN`. For more information on this method see the documentation of the statement `OPEN`.

### INPUT()

This function reads records from an opened file.

Arguments:

- `n` the first argument is the number of records to read. The size of the record in terms of bytes is defined as the `LEN` parameter when the file was opened. If this was missing the function reads `n` bytes from the file or socket.
- `fn` the second parameter is the file number associated to the opened file by the command `OPEN`. If this parameter is missing the function reads from the standard input.

The function tries but not necessarily reads `n` records from the file. To get the actual number of bytes (and not records!) read from the file you can use the function `LEN` on the result string.

Note that some Basic languages allow the form

```
a = INPUT(20,#1)
```

however this extra `#` is not allowed in ScriptBasic. The character `#` is an operator in ScriptBasic defined as no-operation and therefore you can use this form. On the other hand operators like `#` are reserved for the external modules and some external module may redefine this operator. Programs using such modules may end up in trouble when using the format above therefore it is recommended not to use the above format.

### ISDIRECTORY()

Returns `true` if the named file is a directory and `false` if the file is NOT a directory.

### ISFILE()

Returns `true` if the named file is a regular file and `false` if it is a directory.

### LINE INPUT

Read a line from a file or from the standard input.

The syntax of the command is
```
LINE INPUT [# i , ] variable
```

The parameter `i` is the file number used in the open statement. If this is not specified the standard input is read.

The `variable` will hold a single line from the file read containing the possible new line character terminating the line. If the last line of a file is not terminated by a new line character then the `variable` will not contain any new line character. Thus this command does return only the characters that are really in the file and does not append extra new line character at the end of the last line if that lacks it.

On the other hand you should not rely on the missing new line character from the end of the last line because it may and usually it happens to be there. Use rather the function `EOF` to determine if a file reading has reached the end of the file or not.

See also `CHOMP`

You can also read from sockets using this command but you should be careful because data in a socket comes from programs generated on the fly. This means that the socket pipe may not contain the line terminating new line and not finished as well unlike a file. Therefore the command may start infinitely long when trying to read from a socket until the application on the other end of the line sends a new line character or closes the socket. When you read from a file this may not happen.

### LOC()

Return current file pointer position of the opened file. The argument of the
function is the file number that was used by the statement `OPEN` opening the
file.

This function is the counter part of the statement `SEEK` that sets the file
pointer position.

The file position is counted in record size. This means that the file pointer stands after the record returned by the function. This is not necessarily stands right after the record at the start of the next record actually. It may happen that the file pointer stands somewhere in the middle of the next record. Therefore the command

```
SEEK fn,LOC(fn)
```

may alter the actual file position and can be used to set the file pointer to a safe record boundary position.

If there was no record size defined when the file was opened the location is counted in bytes. In this case the returned value precisely defines where the file pointer is.

### LOCK

Lock a file or release a lock on a file. The `mode` parameter can be `read`, `write` or `release`.

When a file is locked to `read` no other program is allowed to write the file. This ensures that the program reading the file gets consistent data from the file. If a program locks a file to read using the lock value `read` other programs may also get the `read` lock, but no program can get the `write` lock. This means that any program trying to write the file and issuing the command `LOCK` with the parameter `write` will stop and wait until all read locks are released.

When a program write locks a file no other program can read the file or write the file.

Note that the different operating systems and therefore ScriptBasic running on different operating systems implement file lock in different ways. UNIX operating systems implement so called advisory locking, while Windows NT implements mandatory lock.

This means that a program under UNIX can write a file while another program has a read or write lock on the file if the other program is not good behaving and does not ask for a write lock. Therefore this command under UNIX does not guarantee that any other program is not accessing the file simultaneously.

Contrary Windows NT does lock the file in a hard way, and this means that no other process can access the file in prohibited way while the file is locked.

This different behavior usually does not make harm, but in some rare cases knowing it may help in debugging some problems. Generally you should not have a headache because of this.

You should use this command to synchronize the BASIC programs running parallel and accessing the same file.

You can also use the command `LOCK REGION` to lock a part of the file while leaving other parts of the file accessible to other programs.

If you heavily use record oriented files and file locks you may consider using some data base module to store the data in database instead of plain files.

### LOF()

This function returns the length of an opened file in number of records. The argument of the function has to be the file number that was used by the statement `OPEN` to open the file.

The actual number of records is calculated using the record size specified when the command `OPEN` was used. The returned number is the number of records that fit in the file. If the file is longer containing a fractional record at the end the fractional record is not counted.

If there was no record length specified when the file was opened the length of the file is returned in number of bytes. In this case fractional record has no meaning.

### MKDIR directory_name

This command creates a new directory. If it is needed then the command attempts to create all directories automatically that are needed to create the final directory. For example if you want to create `public_html/cgi-bin` but the directory `public_html` does not exist then the command

```
MKDIR "public_html/cgi-bin"
```

will first create the directory `public_html` and then `cgi-bin` under that directory.

If the directory can not be created for some reason an error is raised.

This is not an error if the directory does already exist.

You need not call this function when you want to create a file using the command `OPEN`. The command `OPEN` automatically creates the needed directory when a file is opened to be written.

The created directory can be erased calling the command `DELETE` or calling the dangerous command `DELTREE`.

### NAME filename,filename

Rename a file. The first file is the existing one, the second is the new name of the file. You can not move filed from one disk to another using this command. This command merely renames a single file. Also you can not use wild characters in the source or destination file name.

If you can not rename a file for some reason, you can try to use the command `FileCopy` and then delete the old file. This is successful in some of the cases when `NAME` fails, but it is a slower method.

If the file can not be renamed then the command raises error.

### NEXTFILE()

Retrieve the next file name from an opened directory list. If there is no more file names it returns `undef`.

See also `OPENDIR` and `CLOSEDIR`.

### OPEN file_name FOR mode AS [ # ] i [ LEN=record_length ]

Open or create and open a file. The syntax of the line is

```
OPEN file_name FOR mode AS [ # ] i [ LEN=record_length ]
```

The parameters:
- `file_name` if the name of the file to be opened. If the mode allows the file to be written
the file is created if it did not existed before. If needed, directory is created for the file.

- `mode` is the mode the file is opened. It can be:
- `input` open the file for reading. In this mode the file is opened in read only mode and can not be altered using the file number associated with the open file. Using any function or command that tries to write the file will result in error. In this mode the file has to exist already to open successfully. If the file to be opened for `input` does not exist the command `OPEN` raises an error.
- `output` open the file for writing. If the file existed it's content is deleted first and a freshly opened empty file is ready to accept commands and functions to write into the file. When a file is opened this way no function or command trying to read from the file can be used using the file number associated with the file. The file is opened in ASCII mode but the handling mode can be changed to binary any time.
- `append` open a possibly existing file and write after the current content. The same conditions apply as in the mode `output`, thus you can not read the file, only write. The file is opened in ASCII mode but the handling mode can be changed to binary any time.
- `random` open the file for reading and writing (textual mode). When you open a file using this mode the file can be written and the content of the existing file can be read. The file pointer can be moved back and forth any time using the command `SEEK` and thus quite complex file handling functions can be implemented. If the file did not exist it is created.
- `binary` open the file for reading and writing (binary mode). This mode is the same as `random` with the exception that the file is opened in binary mode.
- `socket` open a socket. In this case the file name is NOT a file name, but rather an Internet machine name and a port separated by colon, like `www.digital.com:80` You should not specify any method, like `http://` in front of the machine name, as this command opens a TCP socket to the machine's port and the protocol has to be implemented by the BASIC program.

- `#i` is the file number. After the file has been opened this number has to be used in later file handling functions and commands, like `CLOSE` to refer to the file. The `#` character is optional and is allowed for compatibility with other BASIC languages. The number can be between 1 and 512.
This number is quite big for most of the applications and provides compatibility with VisualBasic.

- `record_length` is optional and specify the length of a record in the file. The default record length is 1 byte. File pointer setting commands usually work on records, thus `SEEK`, `TRUNCATE` and other commands and functions accept arguments or return values as number of records. The actual record length is not recorded anywhere thus the BASIC program has to remember the actual length of a record in a file. This is not a BASIC program error to open a file with a different record size than it was created, although this may certainly be a programming error.

If the file number is specified as a variable and the variable value is set to integer zero then the command will automatically find a usable file number and set the variable to hold that value. Using any other expression of value integer zero is an error.

### PRINT [ # fn , ] print_list

This command prints the elements of the `print_list`. The argument `print_list` is a comma separated list of expressions. The expressions are evaluated one after the other and are printed to the standard output or to the file.

The command prints the `print_list` to an opened file given by the file number `fn`. If `fn` (along with the `#` character) is not specified the command prints to the standard output. The file has to be opened to some "output" mode otherwise the command fails to print anything into the file. The command can also print into an opened socket (a file opened for mode socket). If the file is not opened then the expressions in the list `print_list` are not evaluated and the command actually does nothing. If the file is opened, but not for a kind of "output" mode then the expressions in the `print_list` are evaluated but the printing does not happen. Neither printing to a non-opened file number nor printing to a file opened for some read-only mode generates error.

If there is no `print_list` specified the command prints a new line. In other words if the keyword `PRINT` stands on the command with the optional `#` and the file number but without anything to print then the command will print a new line character.

Note that unlike other BASIC implementations the command `PRINT` does not accept print list formatters, like `AT` or semicolons and does not tabify the output. The arguments are printed to the file or to the standard output one after the other without any intrinsic space or tab added. Also the print statements does not print a new line at the end of the print list unless the new line character is explicitly defined or if there is no print list at all following the command.

### RESET

This command closes all files opened by the current BASIC program. This command usually exists in most BASIC implementation. There is no need to close a file before a BASIC program finishes, because the interpreter automatically closes all files that were opened by the program.

### RESET DIRECTORY

Reset the directory file name list and start from the first file name when the next call to `NEXTFILE` is performed.

See also `OPENDIR`, `CLOSEDIR`, `NEXTFILE`, `EOD`.

### REWIND [ # ]fn

Positions the file cursor to the start of the file.
This is the same as `SEEK fn,0` or `SEEK #fn,0`

The argument to the statement is the file number used in the `OPEN` statement to open the file. The character `#` is optional and can only be used for compatibility reasons.

### SEEK fn,position

Go to a specified position in an open file. You can use this command to position
the file pointer to a specific position. The next read or write operation
performed on the file will be performed on that very position that was set
using the command `SEEK`. The first argument is the file number that was
used in the statement `OPEN` to open the file. The second argument is the
position where the file pointer is to be set.

The position is counted from the start of the file
counting records. The actual file pointer will be set **after** the record
`position`. This means that if for example you want to set the file pointer
To the start of the file then you have to `SEEK fn,0`. This will set the
File pointer before the first record.

If there was no record length specified when the file was
opened the counting takes bytes. There is no special "record" structure of
a file as it is usually under UNIX or Windows NT. The record is merely the
number of bytes treated as a single unit specified during file opening.

### SET FILE

Set some of the parameters of a file. The parameter can be:

- `owner` set the owner of the file. This operation requires `root` permission on UNIX or `Administrator` privileges on Windows NT. The value should be the string representation of the UNIX user or the Windows NT domain user.
- `createtime`
- `modifytime`
- `accesstime`
- Set the time of the file. The value should be the file time in seconds since January 1,1970. 00:00GMT.

If the command can not be executed an error is raised. Note that setting the file owner also depends on the file system. For example FAT file system does not store the owner of a file and thus can not be set.

Also setting the file time on some file system may be unsuccessful for values that are successful under other file systems. This is because different file systems store the file times using different possible start and end dates and resolution. For example you can set a file to hold the creation time to be January 1, 1970 0:00 under NTFS, but not under FAT.

The different file systems store the file times with different precision. Thus the actual time set will be the closest time not later than the specified in the command argument. For this reason the values returned by the functions `File***Time` may not be the same that was specified in the `SET FILE` command argument.

### TEXTMODE [ # fn] | input | output

Set an opened file handling to text mode.

The argument is either a file number with which the file was opened or one of keywords `input` and `output`. In the latter case the standard input or output is set.

See also `BINMODE`

### TRUNCATE fn,new_length

Truncate an opened file to the specified size. The first argument
Has to be the file number used in the `OPEN` statement opening the
file. The second argument is the number of records to be in the file after
it is truncated.

The size of a record has to be specified when the file is opened. If the size
Of a record is not specified in number of bytes then the command `TRUNCATE`
Does truncate the file to the number of specified bytes instead of records.
(In other words the record length is one byte.)

When the file is actually shorter than the length specified by the command argument the command `TRUNCATE` automatically extends the file padding with bytes containing the value 0.

## loop

### DO

This command is a looping construct that repeats commands so long as long the condition following the keyword `UNTIL` becomes `true` or the condition following the keyword `WHILE` becomes `false`.

The format of the command is
```
DO
 ...
 commands to repeat
 ...
LOOP WHILE expression
```

or

```
DO
 ...
 commands to repeat
 ...
LOOP UNTIL expression
```

The condition expression is evaluated every time after the loop commands were executed. This means that the loop body is executed at least once.

A `DO`/`LOOP` construct should be closed with a `LOOP UNTIL` or with a `LOOP WHILE` command but not with both.

The `DO`/`LOOP UNTIL` is practically equivalent to the `REPEAT`/`UNTIL` construct.

See also `WHILE`, `DOUNTIL`, `DOWHILE`, `REPEAT`, `DO` and `FOR`.

### DO UNTIL condition

This command implements a looping construct that loops the code between the line `DO UNTIL` and `LOOP` util the expression following the keywords on the loop starting line becomes `true`.

```
DO UNTIL expression
 ...
 commands to repeat
 ...
LOOP
```

The expression is evaluated when the looping starts and each time the loop is restarted. It means that the code between the `DO UNTIL` and `LOOP` lines may be skipped totally if the expression evaluates to some `TRUE` value during the first evaluation before the execution starts the loop.

This command is practically equivalent to the construct

```
WHILE NOT expression
...
 commands to repeat
 ...
WEND
```

You can and you also should use the construct that creates more readable code.

See also `WHILE`, `DOUNTIL`, `DOWHILE`, `REPEAT`, `DO` and `FOR`.

### DO WHILE condition

This command implements a looping construct that loops the code between the line `DO WHILE` and `LOOP` util the expression following the keywords on the loop starting line becomes `false`.

Practically this command is same as the command `WHILE` with a different syntax to be compatible with different BASIC implementations.

```
do while
 ...
loop
```

You can use the construct that creates more readable code.

See also `WHILE`, `DOUNTIL`, `DOWHILE`, `REPEAT`, `DO` and `FOR`.

### FOR var=exp_start TO exp_stop [ STEP exp_step ]

Implements a FOR loop. The variable `var` gets the value of the start expression `exp_start`, and after each execution of the loop body it is incremented or decrement by the value `exp_step` until it reaches the stop value `exp_stop`.

```
FOR var= exp_start TO exp_stop [ STEP exp_step]
 ...
 commands to repeat
 ...
NEXT var
```

The `STEP` part of the command is optional. If this part is missing then the default value to increment the variable is 1.

If
- the expression `exp_start` is larger than the expression `exp_stop` and `exp_step` is positive or if
- the expression `exp_start` is smaller than the expression `exp_stop` and `exp_step` is negative
then the loop body is not executed even once and the variable retains its old value.

When the loop is executed at least once the variable gets the values one after the other and after the loop exists the loop variable holds the last value for which the loop already did not execute. Thus

```
for h= 1 to 3
next
print h
stop
```

prints `4`.

The expression `exp_start` is evaluated only once when the loop starts. The other two expressions `exp_stop` and `exp_step` are evaluated before each loop. Thus

```
j = 1
k = 10
for h= 1 to k step j
print h,"\n"
j += 1
k -= 1
next
print k," ",j,"\n"
stop
```

will print

```
1
3
6
7 4
```

To get into more details the following example loop

```
STEP_v = 5
for z= 1 to 10 step STEP_v
 print z,"\n"
 STEP_v -= 10
next z
```

executes only once. This is because the step value changes its sign during the evaluation and the new value being negative commands the loop to terminate as the loop variable altered value is smaller then the end value. In other words the comparison also depends on the actual value of the step expression.

These are not only the expressions that are evaluated before each loop, but the variable as well. If the loop variable is a simple variable then this has not too much effect. However if the loop variable is an array member then this really has to be taken into account. For example:

```
for j=1 to 9
  A[j] = 0
next

j = 1
for A[j]= 1 to 9

 for k=1 to 9
  print A[k]
 next k
 print

 j += 1
 if j > 9 then STOP

next
```

prints

```
100000000
110000000
111000000
111100000
111110000
111111000
111111100
111111110
111111111
```

so you can see that the loop takes, evaluates, compares and increments the actual array element as the variable `j` in the sample code above is incremented.

The loop variable or some other left value has to stand between the keyword `FOR` and the sign `=` on the start line of the loop but this is optional following the keyword `NEXT`. ScriptBasic optionally allow you to write the variable name after the keyword `NEXT` but the interpreter does not check if the symbol is a variable of the loop. The role of this symbol is merely documentation of the BASIC code. However, you can not write an array element following the keyword `NEXT`, only a simple variable name.

If the expression `exp_step` is zero then the loop variable is not altered and the loop is re-executed with the same loop variable value. This way you can easily get into infinite loop.

These are fine tuning details of the command `FOR` that you may need to be aware when you read some tricky code. On the other hand you should never create any code that depends on these features. The loop variable is recommended to be a simple variable and the expressions in the loop head should evaluate the same for each execution of the loop. If you need something more special that may depend on some of the features discussed above then you have to consider using some other looping construct to get more readable code.

### REPEAT

This command implements a loop which is repeated so long as long the expression standing after the loop closing line `UNTIL` becomes `true`. The loop starts with a line containing the keyword `REPEAT` and finishes with the line `UNTIL expression`.

```
repeat
  ...
  commands to repeat
  ...
until expression
```

The expression is evaluated each time after the loop is executed. This means that the commands inside the loop are executed at least once.

This kind of loop syntax is not usual in BASIC dialects but can be found in languages like PASCAL. Implementing this loop in ScriptBasic helps those programmers, who have PASCAL experience.

See also `WHILE`, `DOUNTIL`, `DOWHILE`, `REPEAT`, `DO` and `FOR`.

### WHILE condition

Implements the 'while' loop as it is usually done in most basic implementations. The loop starts with the command `while` and finished with the line containing the keyword `wend`. The keyword `while` is followed by an expression and the loop is executes so long as long the expression is evaluated `true`.

```
while expression
  ...
  commands to repeat
  ...
wend
```

The expression is evaluated when the looping starts and each time the loop is restarted. It means that the code between the `while` and `wend` lines may be skipped totally if the expression evaluates to some `false` value during the first evaluation before the execution starts the loop.

In case some condition makes it necessary to exit the loop from its middle then the command `GOTO` can be used.

ScriptBasic implements several looping constructs to be compatible with different BASIC language dialects. Some constructs are even totally interchangeable to let programmers with different BASIC experience use the one that fit they the best. See also `WHILE`, `DOUNTIL`, `DOWHILE`, `REPEAT`, `DO` and `FOR`.

## math

### ABS()

Returns the absolute value of the argument. If the argument is a string then it first converts it to integer or real value. The return value is integer or real value depending on the argument.

`ABS(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### ACOS()

Calculates the arcus cosine of the argument, which is the inverse of the function `COS`. If the argument is not between (-1.0,+1.0) the return value is `undef`.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`ACOS(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### ACOSECANT()

Calculates the arcus cosecant of the argument, the inverse of `COSECANT`, and
returns it in radians, between -PI/2 and +PI/2. The domain is `|x|`=1>: outside
it, and for zero, the result is `undef`, or the error `sbMathErrDiv` when the
option `RaiseMatherror` asks for it, the same rule as `ASIN`.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`ACOSECANT(undef)` is `undef` or raises an error if the option
`RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `COSECANT`, `ASIN`, `ASECANT`

### ACTAN()

Calculates the arcus cotangent of the argument, the inverse of `COTAN`, and
returns it in radians. The branch chosen is the continuous one, between 0 and PI:
`ACTAN(1)` is PI/4, `ACTAN(0)` is PI/2 and `ACTAN(-1)` is 3*PI/4. It is
`PI/2-ATN(x)`, which stays on the same branch on both sides of zero, unlike
`ATN(1/x)`.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`ACTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `ATN`, `COTAN`

### AHCOS()

Calculates the area hyperbolic cosine of the argument, the inverse of `HCOS`.
The domain is every argument greater than or equal to 1: below it the result is
`undef`, or the error `sbMathErrDiv` when the option `RaiseMatherror` asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHCOS(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HCOS`, `AHSIN`, `AHCTAN`

### AHCOSECANT()

Calculates the area hyperbolic cosecant of the argument, the inverse of
`HCOSECANT`: `AHCOSECANT(x)` is `ASINH(1/x)`. It is defined for every
non-zero argument; for zero the result is `undef`, or the error
`sbMathErrDiv` when the option `RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHCOSECANT(undef)` is `undef` or raises an error if the option
`RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `HCOSECANT`, `AHSIN`, `AHSECANT`

### AHCTAN()

Calculates the area hyperbolic cotangent of the argument, the inverse of
`HCTAN`: `AHCTAN(x)` is `AHTAN(1/x)`. The domain is every argument whose
absolute value is greater than 1: outside it, and at the two ends, the result is
`undef`, or the error `sbMathErrDiv` when the option `RaiseMatherror` asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHCTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HCTAN`, `AHTAN`, `AHCOSECANT`

### AHSECANT()

Calculates the area hyperbolic secant of the argument, the inverse of
`HSECANT`: `AHSECANT(x)` is `ACOSH(1/x)`. The domain is every argument above
zero and at most 1: outside it the result is `undef`, or the error
`sbMathErrDiv` when the option `RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHSECANT(undef)` is `undef` or raises an error if the option
`RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `HSECANT`, `AHCOS`, `AHCOSECANT`

### AHSIN()

Calculates the area hyperbolic sine of the argument, the inverse of `HSIN`:
`AHSIN(x)` is the number `y` for which `HSIN(y)` is `x`. It is defined
everywhere. Like `HSIN` it works on plain real numbers, not on angles.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHSIN(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HSIN`, `AHCOS`, `AHTAN`

### AHTAN()

Calculates the area hyperbolic tangent of the argument, the inverse of `HTAN`.
The domain is every argument strictly between -1 and 1: outside it, and at the
two ends, the result is `undef`, or the error `sbMathErrDiv` when the option
`RaiseMatherror` asks for it, because the tangent tends to infinity there.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`AHTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HTAN`, `AHSIN`, `AHCOS`

### ASECANT()

Calculates the arcus secant of the argument, the inverse of `SECANT`, and
returns it in radians, between 0 and PI. The domain is `|x|`=1>: outside it, and
for zero, the result is `undef`, or the error `sbMathErrDiv` when the option
`RaiseMatherror` asks for it, the same rule as `ASIN`.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`ASECANT(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `SECANT`, `ACOS`, `ACOSECANT`

### ASIN()

Calculates the arcus sine of the argument, which is the inverse of the function `SIN`. If the argument is not between (-1.0,+1.0) the return value is `undef`.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`ASIN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### ATAN()

Calcule l'arc tangent de l'argument, en radians. Si le resultat tient dans un
entier, il est rendu en entier ; sinon en reel.

`ATAN(undef)` vaut `undef`, ou leve une erreur si l'option
`RaiseMatherror` contient le bit `sbMathErrUndef`.

### ATN()

Calcule l'arc tangent de l'argument, en radians. C'est l'alias historique de
`ATAN`. Si le resultat tient dans un entier, il est rendu en entier ; sinon
en reel.

`ATN(undef)` vaut `undef`, ou leve une erreur si l'option
`RaiseMatherror` contient le bit `sbMathErrUndef`.

### CINT()

This function is the same as `INT` and is present in ScriptBasic to be more
compatible with other BASIC language variants.

### COS()

Calculates the cosine of the argument.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`COS(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### COSECANT()

Calculates the cosecant of the argument, given in radians: `COSECANT(x)` is
`1/SIN(x)`, the inverse of `SIN`. Where the sine is exactly zero, at
`COSECANT(0)` for instance, the result is `undef`, or the error
`sbMathErrDiv` when the option `RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`COSECANT(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `SIN`, `SECANT`, `ACOSECANT`

### COTAN()

Calculates the cotangent of the argument, given in radians: `COTAN(x)` is
`COS(x)/SIN(x)`, the inverse of `TAN`. Where the sine is exactly zero, at
`COTAN(0)` for instance, the result is `undef`, or the error `sbMathErrDiv`
when the option `RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`COTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `TAN`, `COTAN2`, `SECANT`, `COSECANT`

### COTAN2()

Calculates the cotangent of the ratio of its two arguments: `COTAN2(x,y)` is
`COTAN(x/y)`. A zero divisor, or a ratio whose sine is exactly zero, yields
`undef`, or the error `sbMathErrDiv` when the option `RaiseMatherror` asks
for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`COTAN2(undef,...)` is `undef` or raises an error if the option
`RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `COTAN`, `TAN2`

### DEG2RAD()

Converts an angle from degrees to radians: `DEG2RAD(180)` is PI within the
precision of the machine, and `DEG2RAD(90)` is PI/2. The conversion is the plain
multiplication by `PI/180`, so it is exact only as far as the real arithmetic of
the machine is; round the result with `ROUND` when a whole number is expected.

`DEG2RAD(undef)` is `undef`.

See also `RAD2DEG`, `PI`, `SIN`

### EVEN()

Return `true` if the argument is an even number. `EVEN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `ODD`.

### EXP()

Calculates the x-th exponent of `e`. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`EXP(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### FALSE

This built-in constant is implemented as an argument less function. Returns the value `false`.

### FIX()

This function returns the integral part of the argument. The return value of the function is integer with the exception that `FIX(undef)` may return `undef`.

`FIX(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

The difference between `INT` and `FIX` is that `INT` truncates down while `FIX` truncates towards zero. The two functions are identical for positive numbers. In case of negative arguments `INT` will give a smaller number if the argument is not integer. For example:

```
  int(-3.3) = -4
  fix(-3.3) = -3
```

See `INT`.

### FRAC()

The function returns the fractional part of the
argument. This function always returns a double
except that `FRAC(undef)` may return `undef`.
`FRAC(undef)` is `undef` or raises an error if
the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

Negative arguments return negative value (or zero if the argument is
a negative integer), positive arguments result positive values
(or zero if the argument is integer).

### GCD()

Calculates the greatest common divisor of two or more integer arguments. Each
argument is converted to long (a real value is truncated toward zero), its sign
is ignored, and the result is the greatest non negative integer that divides all
of them. `GCD(0,0)` is 0.

`GCD(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `LCM`

### HCOS()

Calculates the hyperbolic cosine of the argument: `HCOS(x)` is
`(EXP(x)+EXP(-x))/2`, always at least 1. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HCOS(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `HSIN`, `HTAN`, `HSECANT`

### HCOSECANT()

Calculates the hyperbolic cosecant of the argument, the inverse of `HSIN`:
`HCOSECANT(x)` is `1/HSIN(x)`. At zero the hyperbolic sine is exactly zero, so
`HCOSECANT(0)` is `undef`, or the error `sbMathErrDiv` when the option
`RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HCOSECANT(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HSIN`, `HSECANT`

### HCTAN()

Calculates the hyperbolic cotangent of the argument, the inverse of `HTAN`:
`HCTAN(x)` is `HCOS(x)/HSIN(x)`. At zero the hyperbolic sine is exactly zero,
so `HCTAN(0)` is `undef`, or the error `sbMathErrDiv` when the option
`RaiseMatherror` asks for it. Everywhere else the result is above 1, or below
-1 for a negative argument.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HCTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `HTAN`, `HSECANT`

### HSECANT()

Calculates the hyperbolic secant of the argument, the inverse of `HCOS`:
`HSECANT(x)` is `1/HCOS(x)`. The hyperbolic cosine is never zero, so the
function is defined everywhere and returns a value between 0 and 1.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HSECANT(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `HCOS`, `HCOSECANT`

### HSIN()

Calculates the hyperbolic sine of the argument: `HSIN(x)` is
`(EXP(x)-EXP(-x))/2`. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HSIN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `HCOS`, `HTAN`, `HCOSECANT`

### HTAN()

Calculates the hyperbolic tangent of the argument: `HTAN(x)` is
`HSIN(x)/HCOS(x)`, between -1 and +1. It is defined everywhere.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`HTAN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `HSIN`, `HCOS`, `HCTAN`

### IMAX()

Selects the greatest of one or more arguments, comparing them as `MAX` does,
and returns the position of the winner instead of its value: `IMAX(5,3,9)` is 2.

Positions start at zero, like the first index of an array. When several
arguments are equal the first one wins, so `IMAX(3,3)` is 0.

`IMAX(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `MAX`, `IMIN`

### IMIN()

Selects the smallest of one or more arguments, comparing them as `MIN` does,
and returns the position of the winner instead of its value: `IMIN(5,3,9)` is 1.

Positions start at zero, like the first index of an array. When several
arguments are equal the first one wins, so `IMIN(3,3)` is 0.

`IMIN(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `MIN`, `IMAX`

### INT()

This function returns the integral part of the argument.
`INT(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.
Other than this the function returns integer value.

The difference between `INT` and `FIX` is that `INT` truncates down
while `FIX` truncates towards zero. The two functions are identical for
positive numbers. In case of negative arguments `INT` will give a smaller
number if the argument is not integer. For example:

```
  int(-3.3) = -4
  fix(-3.3) = -3
```

See `FIX`.

### LCM()

Calculates the least common multiple of two or more integer arguments. Each
argument is converted to long (a real value is truncated toward zero) and its
sign is ignored; the result is never negative. `LCM(0,n)` is 0.

The multiple of several arguments is usually larger than a long, therefore the
calculation is carried in real arithmetic and the result is returned as a real
value whenever it does not fit in a long. That arithmetic is exact up to 2^53;
beyond it the result is an approximation, as everywhere else in the language.

`LCM(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `GCD`

### LOG()

Calculates the natural log of the argument. If the argument is zero or less than zero the result is `undef`.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`LOG(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### LOG10()

Calculates the log of the argument. If the argument is zero or less than zero the result is `undef`

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`LOG10(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### MAX()

Selects and returns the greatest of one or more arguments. The comparison is
numeric: a string argument is converted to a number first. The winning argument
keeps its type, so `MAX(1,2)` is an integer while `MAX(1,2.5)` is a real.
When several arguments are equal the first one wins.

`MAX(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `MIN`, `IMAX`

### MAXINT

This built-in constant is implemented as an argument less function. Returns the maximal number that can be stored as an integer value.

### MIN()

Selects and returns the smallest of one or more arguments. The comparison is
numeric: a string argument is converted to a number first. The winning argument
keeps its type, so `MIN(1,2)` is an integer while `MIN(1,0.5)` is a real.
When several arguments are equal the first one wins.

`MIN(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `MAX`, `IMIN`

### MININT

This built-in constant is implemented as an argument less function. Returns the minimal ("maximal negative") number that can be stored as an integer value.

### ODD()

Return `true` if the argument is an odd number. `ODD(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

See also `EVEN`

### PI

This built-in constant is implemented as an argument less function. Returns the approximate value of the constant PI which is the ratio of the circumference of a circle to its diameter.

### POW()

Calculates the x-th exponent of 10. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`POW(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### RAD2DEG()

Converts an angle from radians to degrees: `RAD2DEG(PI())` is 180 within the
precision of the machine, and `RAD2DEG(PI()/2)` is 90. The conversion is the
plain multiplication by `180/PI`, so it is exact only as far as the real
arithmetic of the machine is; round the result with `ROUND` when a whole number
is expected.

`RAD2DEG(undef)` is `undef`.

See also `DEG2RAD`, `PI`, `TAN`

### RANDOMIZE

Seed the random number generator. If the command is presented without argument the random number generator is seed with the actual time. If argument is provided the random number generator is seed with the argument following the keyword `RANDOMIZE`.

### RND()

Returns a random number as generated by the C function `rand()`. Note that this random number generator usually provided by the libraries implemented for the C compiler or the operating system is not the best quality ones. If you need really good random number generator then you have to use some other libraries that implement reliable RND functions.

### ROUND()

This function rounds its argument. The first argument is the number to round, and the optional second argument specifies the number of fractional digits to round to.

The function rounds to integer value if the second argument is missing.

The return value is long if the number of decimal places to keep is zero, otherwise the return value is double.

Negative value for the number of decimal places results rounding to integer value.

`ROUND(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

Also `ROUND(val,undef)` is equivalent to `ROUND(value)`.

See also `INT`, `FRAC` and `FIX`

### SECANT()

Calculates the secant of the argument, given in radians: `SECANT(x)` is
`1/COS(x)`, the inverse of `COS`. Where the cosine is exactly zero the result
is `undef`, or the error `sbMathErrDiv` when the option `RaiseMatherror` asks
for it. As for `TAN`, the machine value of PI/2 does not make the cosine exactly
zero, so the pole needs an argument the machine represents as such; `SECANT(0)`
is 1.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`SECANT(undef)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `COS`, `COSECANT`, `ASECANT`

### SGN()

Calculates the signum of the argument. This is -1 for negative numbers, +1 for positive numbers and 0 for zero. The return value is always long except that  `SGN(undef)` may return `undef`. If the argument is a string it is converted to long or double.

`SGN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### SIN()

Calculates the sine of the argument. If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`SIN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

### SQRT()

Calculates the square root of the argument. The name is SQRT, as in every other
language; the old SQR spelling is gone.

If the result is within the range of an integer value on the actual architecture then the result is returned as an integer, otherwise it is returned as a real value.

`SQRT(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

If the argument is a negative number the result of the function is `undef` or the function raises error if the option `RaiseMathError` has the bit `sbMathErrDiv` set.

If the square root of the argument is an integer number then the function returns an integer number. In other cases the returned value is real even if the argument itself is integer.

### TAN()

Calculates the tangent of the argument, given in radians: `TAN(x)` is
`SIN(x)/COS(x)`. The poles at odd multiples of PI/2 are not detected, because
the machine value of PI/2 does not make the cosine exactly zero: the result is a
very large real, not `undef`.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`TAN(undef)` is `undef` or raises an error if the option `RaiseMatherror` is
set in bit `sbMathErrUndef`.

See also `ATN`, `TAN2`, `COTAN`

### TAN2()

Calculates the tangent of the ratio of its two arguments: `TAN2(x,y)` is
`TAN(x/y)`. A zero divisor yields `undef`, or the error `sbMathErrDiv` when
the option `RaiseMatherror` asks for it.

If the result is within the range of an integer value then it is returned as an
integer, otherwise as a real value.

`TAN2(undef,...)` is `undef` or raises an error if the option `RaiseMatherror`
is set in bit `sbMathErrUndef`.

See also `TAN`, `COTAN2`

### TRUE

This built-in constant is implemented as an argument less function. Returns the value `true`.

### VAL()

Converts a string to numeric value. If the string is integer it returns an integer value. If the string contains a number presentation which is a float number the returned value is real. In case the argument is already numeric no conversion is done.

`VAL(undef)` is `undef` or raises an error if the option `RaiseMatherror` is set in bit `sbMathErrUndef`.

## misc

### ADDRESS()

Return the entry point of a function or subroutine. The returned value is to be used solely in a corresponding `ICALL`.

The returned value is an integer value that is the internal node number of the compiled code where the function starts. The different node numbers are in complex relation with each other and simple rules can not be applied. In other words playing around with the value returned by the function `ADDRESS` and then using it in an `ICALL` may result interpreter crash raising internal error.

Note that in the argument of the function `ADDRESS` the function name has to include the `()` characters. The function is NOT called by the code when the function `ADDRESS` is used. On the other hand forgetting the opening and closing parentheses will result erroneous value unusable by `ICALL`.

### CALL subroutine

Use this command to call a subroutine. Subroutines can be called just writing the name of the already defined subroutine and the arguments. However in situation when the code calls a function that has not yet been defined the interpreter knows that the command is a subroutine call from the keyword `CALL`.

To be safe you can use the keyword before any subroutine call even if the subroutine is already defined.

Subroutines and functions can be called the same way. ScriptBasic does not make real distinction between subroutines and functions. However it is recommended that functions be used as functions using the return value and code segments not returning any value are implemented and called as subroutine.

### CHDIR directory

Change the current working directory (CWD). This command accepts one argument, the directory which has to be the CWD after the command is executed. If the CWD can not be changed to that directory then an error is raised.

Pay careful attention when you use this command in your code. Note that there is only one CWD for each process and not one for each thread. When an application embeds the BASIC interpreter in a multi-thread environment, like in the Eszter SB Application Engine this command may alter the CWD for all the threads.

For this reason the Eszter SB Application Engine switches off this command, raising error if ever a program executed in the engine calls this command whatever argument is given.

Thus usually BASIC programs should avoid calling this command unless the programmer is certain that the BASIC program will only be executed in a single thread environment (command line).

### COMMAND()

This function returns the command line arguments of the program in a single string. This does not include the name of the interpreter and the name of the BASIC program, only the arguments that are to be passed to the BASIC program. For example the program started as

```
# scriba test_command.sb arg1 arg2  arg3
```

will see `"arg1 arg2 arg3"` string as the return value of the function `COMMAND()`.

### CURDIR()

This function does not accept argument and returns the current working directory as a string.

### DECLARE COMMAND

Declare a command implemented in an external library. Note that library and the command implemented in it should be specifically written for ScriptBasic. This command is not able to declare and call just any library function.

The arguments are similar to that of the command `DECLARE SUB` and the same restrictions and conditions apply. Look at the documentation of the command `DECLARESUB`.

DEVELOPER DETAILS!

This command is used to start an external command defined in a dynamic load library. Please read the details of the command `DECLARE SUB` if you did not do up to know before reading the details of this command.

The major difference between an external function and an external command is that arguments are passed after they are evaluated to external functions, while arguments are not evaluated for external commands.

External commands are a bit more tedious to implement. On the other hand external commands have more freedom handling their arguments. A command is allowed to evaluate or not to evaluate some of its arguments. It can examine the structure of the expression passed as argument and evaluate it partially at its wish.

From the BASIC programmer point of view external commands are called the same way as user defined functions or external functions.

### DECLARE SUB

Declare a function implemented in an external library. Note that library and the function implemented in it should be specifically written for ScriptBasic. This command is not able to declare and call just any library function.

The command has three arguments. The argument `function` is a symbol (a case insensitive string without any space, containing only alphanumeric characters and without double quotes) , the name if the function as it is going to be used in the BASIC program. The BASIC programmer is free to choose this function name. The function later can be used as if it was a BASIC user defined function.

The argument `cfun` has to be the name of the function as it is defined in the library. This has to be a constant string value. You can not use any expression here but a constant string. This is usually the same as the name of the interface function in the C source file. The programmer writing the module in C has to know this name. If for some reason you do not happen to know it, but you need to declare the function you may be lucky using the name that stands in the C source file between parentheses after the macro `besFUNCTION`.

The last argument is the name of the library. This also has to be a constant string value. You can not use any expression here but a constant string. This argument has to specify the name of the library file without the extension and the path where the library is located. The extension will automatically be appended to the file name and the path will automatically be prepended to it. The actual extension and the path to be searched for the library is defined in the ScriptBasic configuration file.

You can also specify the full file name containing the full path to the library as well as the file extension. In this case the ScriptBasic configuration file data for the module path and extension is ignored.

DEVELOPER DETAILS!

This command is used to start an external function defined in a dynamic load library.

The dynamically loaded modules are implemented in ScriptBasic via an ordinary command that has the syntax:

```
'declare' 'sub' * function 'alias' string 'lib' string nl
```

For the compiler it generates a user defined function, which is defined on the line that contains the declare statement. The execution system will call itself recursively to execute lines starting at the line where the `declare` statement is. The command implemented in this file is executed and unlike the `FUNCTION` or `SUB` it immediately tells the execution system to return after the line was executed.

This command first checks if the line was already executed. On the first execution it loads the module and gets the address of the function named in the alias string. This entry point is stored in a `struct` and next time the function is called by the basic pointer it does not need to search for the function and for the module. If a function of an already loaded module is called the program does not reload the module. The program maintains a linked list with the names of the loaded modules and loads modules only when they are first referenced.

The modules are loaded using the operating system `dll` loading function `dlopen` or `LoadLibrary`. These functions search several locations for libraries in case the library is specified without absolute path name.
The module loader can be fouled if the same library is defined with full path and with single name in the basic code.

For example, if the commands

```
declare sub fun2 alias "mefun" lib "libobas"
declare sub fun3 alias "youfun" lib "/usr/lib/scriba/libobas.so"
```

refer to the same module, the code implemented here thinks that they are different libraries.

When the module is loaded the code tries to get the function named `versmodu` and calls it. This function gets three arguments. The first argument is the interface version that ScriptBasic supports for the external modules.
The second argument is a pointer to a ZCHAR terminated string that contains the variation of the calling interpreter. Note that this has to be an 8-character (+1 ZCHAR) string. The third argument is a pointer to a `NULL` initialized `void *` pointer, which is the module pointer. This pointer is stored by ScriptBasic, and it is guaranteed not been modified. The modules can store "global" variable information using this pointer. Usually this pointer is not used in this function, especially because there are no "safe" memory allocation functions available at this point of execution.

The module should examine the version it gets and it should decide if the interface version is OK for the module. If the interface version is not known the function should return 0 and ScriptBasic will interpret this value as a failure to load the module. If the module does not know if the interface version is good or not it can return the interface version that it can handle. In this case it is the duty of the interpreter to decide if the interface version can be provided or not. ScriptBasic will examine the version and in case it can not handle the version it will generate an error.

This methodology will allow either ScriptBasic to revert its functionality to earlier interface versions in case the higher version interface not only extends the lower version but is incompatible with the former version. On the other hand a module designed for a higher version may be loaded and executed by a lower version of ScriptBasic in case the module is ready to use the lower interface version.

If the function `versmodu` can not be found in the DLL then ScriptBasic assumes that the module is ready to accept the current interface version. However such a module should not generally be written.

Note that the version we are talking about here is neither the version of ScriptBasic nor the version of the module. This is the version of the interface between the module and ScriptBasic.

After the version negotiation has been successfully done ScriptBasic tries to get the address of the function named `bootmodu`. If this function exists it is started. This function can perform all the initializations needed for the module. This function gets all the parameters that a usual module implemented function except that there are no parameters and the function should not try to set a result value, because both of these arguments are `NULL`.

A module function (including `bootmodu`) is called with four arguments. The four arguments are four pointers.

```
int my_module_function(pSupportTable pSt,
                       void **ppModuleInternal,
                       pFixSizeMemoryObject pParameters,   // NULL for bootmodu
                       pFixSizeMemoryObject *pReturnValue) // NULL for bootmodu
```

The parameter `pSt` points to a `struct` that holds the function pointers that the function can call to reach ScriptBasic functionality. These functions are needed to access the arguments and to return a value.

The parameter `ppModuleInternal` is a pointer pointing to a `NULL` initialized pointer. This pointer belongs to the module. ScriptBasic guarantees that the value is not modified during the execution (unless the module itself modifies it). This pointer can be used to remember the address space allocated by the module for itself. To store permanent values to remember the state of the module you can either use static variables or this pointer. However using this pointer and allocating memory to store the values is the preferred method. The reason for preferring this method is that global variables are global in the whole process. On the other hand ScriptBasic may run in several threads in a single process executing several different basic programs. This `ppModuleInternal` pointer will be unique for each thread.

Here is the point to discuss a bit the role of `bootmodu`. Why not to use `DllMain` under Windows NT? The reason is in the possibility of threaded execution. `DllMain` is executed when the process loads the `dll`. `bootmodu` is executed when the basic executor loads the module. If there are more threads running then `DllMain` is not started again. Use `DllMain` or a similar function under UNIX if you want to initialize some process level data. Use `bootmodu` to initialize basic program specific data.

The parameter `pParameters` is a ScriptBasic array containing the values of the arguments. There is no run-time checking about the number of arguments. This is up to the module function.

The last parameter is a pointer to a ScriptBasic variable. The initial value of the pointed pointer is `NULL`, meaning `undef` return value. The final return value should be allocated using the macros defined in `basext.h` and this pointer should be set to point to the value. Note however that `bootmodu` and `finimodu` should not to try to dereference this variable, because for both of them the value is `NULL`.

For further information on how to write module extension functions read the on-line documentation of `basext.c` in this source documentation.

A module is unloaded when the basic program execution is finished. There is no basic code to unload a module. (Why?)

Before the module is unloaded calling one of the operating system functions `dlclose` or `FreeLibrary` the program calls the module function `finimodu` if it exists. This function gets all the four pointers (the last two are `NULL`s) and it can perform all the tasks that the module has to do clean up.

Note however that the module need not release the memory it allocated using the `besALLOCATE` macro defined in the file `basext.h` (declared in `basext.h`). The memory is going to be released afterwards by ScriptBasic automatically.

You can have a look at the source code of modules provided by ScriptBasic, for example MySQL module, Berkeley DB module, HASH module, MT module and so on.

### END

End of the program. Stops program execution.

You should usually use this command to signal the end of a program. Although you can use `STOP` and `END` interchangeably this is the convention in BASIC programs to use the command `END` to note the end of the program and `STOP` to stop the program execution based on some extra condition inside the code.

### ENVIRON()

This function returns the value of an environment variable. Environment variables are string values associated to names that are provided by the executing environment for the programs. The executing environment is usually the operating system, but it can also be the Web server in CGI programs that alters the environment variables provided by the surrounding operating system specifying extra values.

This function can be used to get the string of an environment variable in case the program knows the name of the variable or to list all the environment variables one by one.

If the environment variable name is known then the name as a string has to be passed to this function as argument. In this case the return value is the value of the environment variable.

If the program wants to list all the environment variables the argument to the function `ENVIRON` can be an integer number `n`. In this case the function returns a string containing the name and the value joined by a `=` sign of the `n`-th environment variable. The numbering starts with `n=0`.

If the argument value is integer and is out of the range of the possible environment variable ordinal numbers (negative or larger or equal to the number of the available environment variables) then the function returns `undef`.

If the argument to the function is `undef` then the function also returns the `undef` value.

Note that ScriptBasic provides an easy way for the embedding applications to redefine the underlying function that returns the environment variable. Thus an embedding application can "fool" a BASIC program providing its own environment variable. For example the Eszter SB Application Engine provides an alternative environment variable reading function and thus BASIC applications can read the environment using the function `ENVIRON` as if the program was running in a real CGI environment.

The following sample code prints all environment variables:

```
i=0
do
  e$ = environ(i)
  if IsDefined(e$) then
    print e$
    print
  endif
  i = i + 1
loop while IsDefined(e$)
```

### EXIT

Leave the innermost loop and continue after it. `EXIT FOR`, `EXIT WHILE`,
`EXIT DO` and `EXIT REPEAT` leave the innermost loop of that kind, which is
what you need when loops are nested and only one of them has to be left.

`EXIT` does what a jump to the statement after the loop does: the analyser
resolves the destination when it compiles the closing keyword, so there is no
label to name and no way to jump anywhere else. Inside a subroutine the
statement only sees the loops of that subroutine.

`EXIT` without a loop is refused at analysis, with the error "erroneous
nesting". Use `EXITSUB` or `EXITFUNC` to return from a subroutine or a
function.

### EXIT FUNCTION

This function stops the execution of a function and the execution gets back to the point from where the function was called. Executing this command has the same effect as if the execution has reached the end of a function.

### EXIT SUB

This function stops the execution of a subroutine and the execution gets back to the point from where the subroutine was called. Executing this command has the same effect as if the execution has reached the end of a subroutine.

Same as `EXITFUNC`

### FUNCTION fun()

This command should be used to define a function. A function is a piece of code that can be called by the BASIC program from the main part or from a function or subroutine.

```
FUNCTION fun(a,b,c)
...
fun = returnvalue
...
END FUNCTION
```

The end of the function is defined by the line containing the keywords `END FUNCTION`.

The function declaration can be placed anywhere in the code before or after the function is used. Because the functions just as well as the variables are typeless there is no such thing as function prototype and thus functions can not be declared beforehand.

You can call a function in your BASIC code that is not defined yet. The syntax analyzer seeing the `function_name()` construct will see that this is a function call and will generate the appropriate code to call the function that is defined later. If the function is not defined until the end of the code then the interpreter will generate error before the code is started.

Functions just as well as subroutines can have argument and local variables. When the function or subroutine is called the actual values are evaluated and placed in the argument variables. It is not an error when calling a function to specify less or more number of actual arguments than the declared arguments.

If the actual arguments are too few the rest of the arguments will become `undef` before the function starts. If there are too many arguments when calling a function or subroutine the extra arguments are evaluated and then dropped (ignored).

ScriptBasic allows recursive functions thus a function can call itself, but a function or subroutine is not allowed to be declared inside of another subroutine or function.

Functions differ from subroutines in the fact functions return value while subroutines do not. However in ScriptBasic this is not a strict rule. Subroutines declared with `SUB` instead of `FUNCTION` are allowed to return a value and been used just as if it was declared with the keyword `FUNCTION`.

To return a value from a function the name of the function can be used just like a local variable. The assignment assigning a value to the function will be returned from the function. If there are more than one assignments to the function name then the last assignment executed will prevail. On the other hand you can not use the function name as a local variable and retrieve the last assigned value from the function name. Using the function name inside the function in an expression will result recursive call or will be used as a global or local variable.

If the actual value of an argument is left value then the reference to the actual argument is passed to the function or the subroutine instead of the value. In other cases the value of the expression standing in the position of the argument when calling the function is passed.

Passing a variable by reference means altering the argument variable also alters the variable that was passed to the function.

To force a variable passed by value rather than reference the operator `ByVal` can be used. When `ByVal` is used as a numeric operator in an expression acts as an identity operator. It means that the value of the expression is the same as the value standing on the right of the operator `ByVal`. However this is already an expression and not a variable and as such it can not be passed by reference only the value.

The keyword `ByVal` can also be used as a command listing all the argument variables after the keyword on a line:

```
function myfunc(a,b,c,d)
ByVal a,b,c
...
```

In this case `ByVal` acts as a command and replaces the references by copies of the values. After executing this command the argument values can be altered without affecting the variables that stand in the argument's place where the function is called.

Although the command `ByVal` looks like a declaration of arguments to be passed by value instead of reference; this is not a declaration but it rather a command that has to be executed.

### GOTO label

Go to a label and continue program execution at that label. Labels are local within functions and subroutines. You can not jump into a subroutine or jump out of it.

Use of GOTO is usually discouraged and is against structural programming. Whenever you feel the need to use the `GOTO` statement (except `ON ERROR GOTO`) thin it twice whether there is a better solution without utilizing the statement `GOTO`.

Typical use of the `GOTO` statement to jump out of some error condition to the error handling code or jumping out of some loop on some condition.

### Gosub commands

#### Gosub commands

This is the good old way implementation of the BASIC `GOSUB` command. The command `GOSUB` works similar to the command `GOTO` with the exception that the next return command will drive the interpreter to the line following the line with the `GOSUB`.

You can only call a code segment that is inside the actual code environment. In other words if the `GOSUB` is in a function or subroutine then the label referenced by the `GOSUB` should also be in the same function or subroutine. Similarly any `GOSUB` in the main code should reference a label, which is also in the main code.

To return from the code fragment called by the command `GOSUB` the command `RETURN` should be used. Note that this will not break the execution of a function or a subroutine. The execution will continue on the command line following the `GOSUB` line.

`GOSUB` commands can follow each other, ScriptBasic will build up a stack of `GOSUB` calls and will return to the appropriate command line following the matching `GOSUB` command.

When a subroutine or function contains `GOSUB` commands and the function or subroutine is finished so that one or more executed `GOSUB` command remains without executed `RETURN` then the `GOSUB`/`RATURN` stack is cleared. This is not an error.

See also `RETURN`.

### HOSTNAME()

This function accepts no argument and returns the host name of the machine executing the BASIC program. This host name is the TCP/IP network host name of the machine.

### ICALL

ICALL is implicit call. The first argument of an `ICALL` command or `ICALL` function should be the integer value returned by the function `ADDRESS` as the address of a user defined function.

The rest of the arguments are the arguments to be passed to the function to be called. The return value if the function `ICALL` is the value of the implicitly called function.

Whenever you call a function or subroutine you have to know the name of the subroutine or function. In some situation programmers want to call a function without knowing the name of the function. For example you want to write a sorting subroutine that sorts general elements and the caller should provide a subroutine that makes the comparison. This way the sorting algorithm can be implemented only once and need not be rewritten each time a new type of data is to be sorted. The sorting subroutine gets the comparing function as an argument and calls the function indirectly. ScriptBasic can not pass functions as arguments to other functions, but it can pass integer numbers. The function
`ADDRESS` can be used to convert a function into integer. The result of the built-in function `ADDRESS` is an integer number, which is associated inside the basic code with the function. You can pass this value to the `ICALL` command or function as first argument. The `ICALL` command is the command for indirect subroutine call. The call

```
ICALL Address(MySubroutine()),arg1,arg2,arg3
```

is equivalent to

```
CALL MySubroutine( arg1,arg2,arg3)
```

If you call a function that has return value use can use the `ICALL` function instead of the `ICALL` statement:

```
A = ICALL(Address(MyFunction()),arg1,arg2,arg3)
```

is equivalent to

```
A = MyFunction(arg1,arg2,arg3)
```

The real usage of the function Address and icall can be seen in the following example:

```
sub MySort(sfun,q)
local ThereWasNoChange,SwapVar
repeat
 ThereWasNoChange = 1
 for i=lbound(q) to ubound(q)-1

  if  icall(sfun,q[i],q[i+1]) > 0 then
   ThereWasNoChange = 0
   SwapVar = q[i]
   q[i] = q[i+1]
   q[i+1] = SwapVar
  endif

 next i
until ThereWasNoChange

end sub

function IntegerCompare(a,b)
  if a < b then
   cmp = -1
  elseif a = b then
   cmp = 0
  else
   cmp = 1
  endif
end function

h[0] = 2
h[1] = 7
h[2] = 1

MySort address(IntegerCompare()) , h

for i=lbound(h) to ubound(h)
 print h[i],"\n"
next i
```

Note that the argument of the function Address is a function call. ScriptBasic allows variables and functions to share the same name. Address is a built-in function just as any other built in function, and therefore the expression

```
Address(MySub) B<THIS IS WRONG!>
```

Is syntactically correct. The only problem is that it tries to calculate the address of the variable `MySub`, which it can not and results a run-time error.
Instead you have to write

```
Address( MySub() )
```

using the parentheses. In this situation the function or subroutine
`MySub()` will not be invoked. The parentheses tell the compiler that this is a function and not a variable.

### IF condition THEN

Conditional execution. There are two different ways to use this command: single
line `IF` and multi line `IF`.

A single line `IF` has the form

```
IF condition THEN command
```

Since 2026-10 the single line form accepts a `ELSE` part and a chain of
`ELSEIF` branches, each branch being one command on the same physical line:

```
IF condition THEN command ELSEIF condition THEN command ELSE command
```

Each branch may hold exactly one command; a command that opens a construct of
its own (`IF`, `FOR`, `WHILE`, `SUB`, `FUNCTION`, `REPEAT` and the
declarations) is refused during analysis, because the branch would run past the
end of the line. Use the multi line form for those.

The multi line `IF` should not contain any command directly after the keyword `THEN`. It should have the format:

```
IF condition THEN
  commands
ELSE
  commands
END IF
```

The `ELSE` part of the command is optional, thus the command can have the format

```
IF condition THEN
  commands
END IF
```

as well. To be very precise the full syntax of the multi-line `IF` command is:

```
IF condition THEN
 commands
[ ELSEIF
  commands
  ... ]
[ ELSE
  commands ]
END IF | ENDIF
```

You can use as many `ELSEIF` branches as you like and at most one `ELSE` branch,
in both forms.

Only the single keyword `ELSEIF` is accepted: the two-word form `ELSE IF` and
the variants `ELSIF` and `ELIF` were removed from the language. `END IF` in
two words and the single keyword `ENDIF` remain equivalent.

### LET

Assign a value to a variable.

On the left side of the `=` a variable or some other ScriptBasic left value has to stand. On the right side an expression should be used. First the left value is evaluated and then the expression. Finally the left value's old value is replaced by the result of the expression.

The left value standing on the left side of the `=` can be a local or global variable, array element or associative array element.

### LETC

Append a string to a variable.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v & expression`. Using this short format is more readable in some cases and generates more efficient code. However note that this kind of assignment operation is a C language like operator and is not common in BASIC programs.

### LETD

Divide a variable by an expression.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v / expression`. Using this short format is more readable in some cases and generates more efficient code. However note that this kind of assignment operation is a C language like operator and is not common in BASIC programs.

### LETI

Integer divide a variable by a value.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v \ expression`.
Using this short format is more readable in some cases and generates more efficient code.
However note that this kind of assignment operation is a C language like operator and is not common in BASIC programs.

### LETM

This command subtracts a value from a variable.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v - expression`. Using this short format is more readable in
some cases and generates more efficient code. However note that this kind of assignment operation is a C language
like operator and is not common in BASIC programs.

### LETP

Add a value to a variable.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v + expression`. Using this short format is more readable in some cases and generates more efficient code. However note that this kind of assignment operation is a C language like operator and is not common in BASIC programs.

### LETS

Multiply a variable with a value.

The variable can be a global or local variable, array element or associative array element.

You can use this command as a shorthand for `v =  v * expression`. Using this short format is more readable in some cases and generates more efficient code. However note that this kind of assignment operation is a C language like operator and is not common in BASIC programs.

### OPTION symbol value

Set the integer value of an option. The option can be any string without the double quote. Option names
are case insensitive in ScriptBasic.

This command has no other effect than storing the integer value in the option symbol table. The commands
or extenal modules may access the values and may change their behavior accoring to the actual
values associated with option symbols.

You can retrieve the actual value of an option symbol using the function `OPTIONF`

### OPTION()

Retrieve the actual value of an option symbol as an integer or `undef` if the option was not set.
Unlike in the command `OPTION` the argument of this function should be double quoted.

### POP

Pop off one value from the GOSUB/RETURN stack. After this command a `RETURN` will return to one
level higher and to the place where it was called from.
For more information see the documentation of the command `GOSUB` and `RETURN`.

### REF

Assign a variable to reference another variable. Following this command altering one of the variables alters both variables. In other words this command can be used to define a kind of alias to a variable. The mechanism is the same as local variable of a function is an alias of a variable passed to the function as actual argument. The difference is that this reference is not automatically released when some function returns, but rather it is alive so long as long the referencing variable is not undefined saying `undef variable` in a command.

To have an alias to a variable is not something of a great value though. It becomes a real player when the 'variable' is not just an ordinary 'named' variable but rather part of an array (or associative array). Using this mechanisms the programmer can build up arbitrary complex memory structures without caring such complex things as pointers for example in C. This is a simple BASIC way of building up complex memory structures.

### RETURN

Return from a subroutine started with `GOSUB`. For more information see the documentation of the command `GOSUB`.

### SLEEP

Suspend the execution of the interpreter (process or thread) for `n` seconds.

Whenever the program has to wait for a few seconds it is a good idea to execute this command.
Older BASIC programs originally designed for old personal computers like Atari, Amiga, ZX Spectrum
intend to use empty loop to wait time to elapse. On modern computers this is a bad idea and should not be done.

If you execute an empty loop to wait you consume CPU. Because the program does not access any resource to wait for it
actually consumes all the CPU time slots that are available. This means that the computer slows down, does not respond to user actions timely.

Different computers run with different speed and an empty loop consuming 20sec on one machine may
run 2 minutes on the other or just 10 millisec. You can not reliably tell how much
time there will be during the empty loop runs.

When you execute `SLEEP n` the operating system is called telling it that the
code does not need the CPU for `n` seconds. During this time the program is
suspended and the operating system executes other programs as needed. The code is
guaranteed to return from the function `SLEEP` not sooner than `n` seconds, but
usually it does return before the second `n+1` starts.

### STOP

This command stops program execution. There is no possibility to restart program execution after this command was executed.

See also `END`.

### SUB fun()

This command should be used to define a subroutine. A subroutine is a piece of code that can be called by the BASIC program from the main part or from a function or subroutine.

```
SUB sub(a,b,c)
...
END SUB
```

The end of the subroutine is defined by the line containing the keywords `END SUB`.

Note that functions and subroutines are not really different in ScriptBasic. ScriptBasic allows you to return a value from a subroutine and to call a function using the command `CALL`. It is just a convention to have separately `SUB` and `FUNCTION` declarations.

For detailed information please read the documentation of the command `FUNCTION`

### SWAP

Planned command.

This command swaps two variables.

### UNDEF

Sets the value of a variable (or some other ScriptBasic left value) to be undefined. This command can also be used to release the memory that was occupied by an array when the variable holding the array is set to `undef`.

When this command is used as a function (with or without, but usually without parentheses), it simply returns the value `undef`.

Note that when this command is called in a function then the local variable is undefined and the caller variable passed by reference is not changed. Therefore

```

sub xx(a)
 undef a
end sub

q = 1
xx q
print q
```

will print 1 and not `undef`.

On the other hand

```

sub xx(a)
 a = undef
end sub

q = 1
xx q
print q
```

does print `undef`.

## process

### EXECUTE()

This function should be used to start an external program and wait for it to finish.

The first argument of the function is the executable command line to start.
The second argument is the number of seconds that the BASIC program should wait for
the external program to finish. If the external program finishes during this period
the function returns and the return value is the exit code of the external program.
If the argument specifying how many seconds the BASIC program has to wait is `-1`
then the BASIC program will wait infinitely.

If the program does not finish during the specified period then the function alters the
third argument, which has to be a variable and raises error. In this case the argument `pid_v` will
hold the PID of the external program. This value can be used in the error handling code to terminate
the external program.

The function can be used to start a program synchronous and asynchronous mode as well. When the timeout value passed to the function is zero the function starts the new process, but does not wait it to finish, but raises an error. In this case the BASIC program can catch this error using the `ON ERROR GOTO` structure and get the pid of the started process from the variable `pid_v`. In this case the function does not "return" any value because a BASIC error happened. For example:

```
ON ERROR GOTO NoError
a = EXECUTE("ls",0,PID)
NoError:
print "The program 'ls' is running under the pid: ",PID,"\n"
```

If the argument `time_out` is `-1` the function will wait for the subprocess to finish whatever long it takes to run. For example:

```
a = EXECUTE("ls",-1,PID)
print "ls was executed and the exit code was:",a
```

Note that the string passed as first argument containing the executable program name and the arguments (the command line) should not contain zero character (a character with ASCII code 0) for security reasons. If the command line string contains zero character an error is raised.

### KILL()

This function kills (terminates) a process given by the `pid` and returns true if the process was successfully killed. Otherwise it returns false.

Programs usually want to kill other processes that were started by themselves (by the program I mean) and do not stop. For example you can start an external program using the BASIC command `EXECUTE` to run up to a certain time. If the program does not finish its work and does not stop during this time then that program that started it can assume that the external program failed and got into an infinite loop. To stop this external program the BASIC program should use the function `KILL`.

The BASIC program however can try to kill just any process that runs on the system not only those that were started by the program. It can be successful if the program has the certain permissions to kill the given process.

You can use this function along with the functions `SYSTEM` and `EXECUTE`. You can list the processes currently running on an NT box using some of the functions of the module `NT`.

### SYSTEM()

This function should be used to start an external program in a separate process in asynchronous mode.
In other words you can start a process and let it run by itself and not wait for the process to finish.
After starting the new process the BASIC program goes on parallel with the started external program.

The return value of the function is the PID of the newly created process.

If the program specified by the argument can not be started then the return value is zero. Under UNIX
the program may return a valid PID even in this case. This is because UNIX first makes a copy of the
process that wants to start another and then replaces the new process image with the program image to be
started. In this case the new process is created and the command `SYSTEM` has no information on the fact
that the new process was not able to replace the executable image of itself. In this case, however, the
child process has a very short life.

### WAITPID()

This function should be used to test for the existence of a process.

The return value of the function is 0 if the process is still running.
If the process has exited (or failed in some way) the return value is `1` and
the exit code of the process is stored in `ExitCode`.

## string

### &amp;

This operator concatenates two strings. The resulting string will contain the characters of the string standing on the left side of the operator followed by the characters of the string standing on the right hand side of the operator. The ScriptBasic interpreter automatically allocates the resulting string.

### ASC()

Returns the ASCII code of the first character of the argument string.

### BIN()

Converts the argument to a string of binary digits, most significant bit first,
without a prefix and without a leading zero: `BIN(5)` is `"101"` and
`BIN(0)` is `"0"`. Like `HEX` and `OCT` it works on the unsigned long bit
pattern of the value, so a negative argument yields the full width of a long.

`BIN(undef)` is `undef`.

See also `HEX`, `OCT`

### CHOMP()

Remove the trailing new line from the space. If the last character of the string is not
new line then the original stringis returned. This function is useful to remove the trailing
new line character when reading a line from a file using the command `LINEINPUT`

### CHR()

Return a one character string containing a character of ASCII code `code`.

### CONF()

This function can be used to retrieve ScriptBasic configuration parameters. This is rarely needed by general programmers.
This is needed only for scripts that maintain the ScriptBasic setup, for example install a new module copying the files
to the appropriate location.

The argument `"conf.key"` should be the configuration key string. If this key is not on the top level
then the levels should be separated using the dot chatacter, like `conf("preproc.internal.dbg")` to get the
debugger DLL or SO file.

The return value of the function is the integer, real or string value of the configuration value. If the key is not
defined or if the system manager set the key to be hidden (see later) then the function will raise an error

`(0): error &H8:The argument passed to a module function is out of the accepted range.`

Some of the configuration values are not meant to be readable for the BASIC programs for security reasons. A typical
example is the database connection password. The system manager can insert extra "dummy" configuration keys that will
prevent the BASIC program to get the actual value of the configuration key. The extra configuration key has to have
the same name as the key to be hidden with a `$` sign prepended to it.

For example the MySQL connection named `test`
has the connection password under the key `mysql.connections.test.password`. If the key in the compiled configuration
file `mysql.connections.test.$password` exists then the BASIC function `conf()` will result error. The value of this
extra key is not taken into account.

The system manager can configure whole configuration branches to be hidden from the BASIC programs. For example the
configuration key `mysql.connections.$test` defined with any value will prevent access of BASIC programs to any
argument of the connection named `test`. Similarly the key `mysql.$connections` will prevent access to any configuration
value of any MySQL connections if defined and finally the key `$mysql` will stop BASIC programs to discover any MySQL
configuration information if defined.

The current implementation does not examine the actual value of the extra security key. However later implementations
may alter the behaviour of this function based on the value of the key. To remain compatible with later versions it is
recommended that the extra security key is configured to have the value `1`.

### FORMAT()

The function format accepts variable number of arguments. The first argument is a format string and the
rest of the arguments are used to create the result string according to the format string. This way the function
`format` is like the C function `sprintf`.

The format string can contain normal characters and control substrings.

The control substring have the form `%[flags][width][.precision]type`. It follows the general `sprintf` format
except that type prefixes are not required or allowed and type can only be "dioxXueEfgGsc". The `*`
for width and precision is supported.

An alternate format BASIC-like for numbers has the form `%~format~` where `format` can be:

# Digit or space

0 Digit or zero

^ Stores a number in exponential format.
Unlike QB's USING format this is a place-holder like the #.

. The position of the decimal point.

, Separator.

- Stores minus if the number is negative.

+ Stores the sign of the number.

Acknowledgement: the function `format` was implemented by Paulo Soares <psoares@consiste.pt>

### HEX()

Take the argument as a long value and convert it to a string that represents the value in hexadecimal form.
The hexadecimal form will contain upper case alpha character if there is any alpha character in the
hexadecimal representation of the number.

### INSTR()

This function can be used to search a sub-string in a string.
The first argument is the string we are searching in.
The second argument is the string that we actually want to find in the
first argument. The third optional argument is the position where the
search is to be started. If this argu-ment is missing the search starts
with the first character position of the string.
The function returns the position where the sub-string
can be found in the first string. If the searched sub-string
is not found in the string then the return value is undef.

See `INSTRREV`

### INSTRREV()

This function can be used to search a sub-string in a string in reverse
order starting from the end of the string. The first argument is the
string we are searching in. The second argument is the string that
we actually want to find in the first argument. The third optional argument
is the position where the search is to be started. If this argument is
missing the search starts with the last character position of the string.
The function returns the position where the sub-string can be found in the
first string. If the searched sub-string is not found in the string then the
return value is undef.

See `INSTR`

### ISSTRING()

This function can be used to determine whether an expression is string or some other type of value. If the argument is a string then the function returns `true`, otherwise the function returns `false`.

See also `ISARRAY`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### JOIN()

Join the argument strings using the first argument as a joiner string.
This function can be used to join several strings together.
The first argument of the function is the string used to join the rest
of the arguments. The rest of the argument are joined together, but also elements on
an array can be joined together. See the example:

```
for i=1 to 8
 q[i] = I
next
print join("|",q)
print
print join("/",1,2,3,4,5,6,7,8)
print
print join(" j-s ",q,2,3,4,5,6,7,8)
print
print join("/",1)
print
```

will print

```
1|2|3|4|5|6|7|8
1/2/3/4/5/6/7/8
1 j-s 2 j-s 3 j-s 4 j-s 5 j-s 6 j-s 7 j-s 8
1
```

The first join joins the elements of the array. The second join joins the arguments of the
function. The third example also joins the arguments although the second argument is an array.
Because there are more arguments each of them is treated as single value and are joined.
Whenever an array is used in place of a single value, the first element of the array is taken.
In this example this is 1. The last join is a special one. In this case the join string is not
used, because there is only one argument after the join string. Because this argument is not
an array there are no elements of it to join.

### JOKER()

Return the actual match for the n-th joker character from the last executed `LIKE` operator.

When a `LIKE` operator is executed ScriptBasic stores the actual strings that
matched the joker and wild card characters from the pattern in an array. Using this
function the programcan access the actual value of the n-th string that was matched
against the n-th joker or wild card character. For example:

```
Const nl="\n"
if "file.txt" like "*.*" then
  print "File=",joker(1)," extension=",joker(2),nl
else
  print "did not match"
endif
```

will print

```
File=file extension=txt
```

### LCASE()

Lowercase a string.

`LOWER` is a second name for this function.

### LEFT()

Creates the left of a string. The first argument is the string. The second argument is the
number of characters that should be put into the result. If this value is larger than the
number of characters in the string then all the string is returned.

`LEFT$` is a second name for this function.

See also `MID`, `RIGHT`

`left(x,y)` cuts out a substring of `y` characters from the left of the string `x`.
If the first argument is not defined the result is also `undef`. Otherwise the first
argument is converted to string and the second ar-gument is converted to integer value.

If the second argument is not defined or has negative value it is treated as numeric zero
and as such the result string will be empty string.

For compatibility reasons you can append a dollar (`$`) sign to the end of the function
identifier.

Example

```
a$ = _
"superqualifragilisticexpialidosys"
print "*",left(a$,undef),"*"
print "*",left(a$,7),"*"
print "*",left(a$,-6),"*"
print "*",left(a$,0),"*"
print left(undef,"66")
```

will print

```
**
*superqu*
**
**
undef
```

### LEN()

This function interprets its argument as a string and returns the length of the string. In ScriptBasic strings can hold any value thus the length of the string is the number of characters contained in the string containing any binary characters, even binary zero.

If the argument is not a string it is converted to string automatically and the length of the converted string is returned. The only exception is `undef` for which the result is also `undef`.

### LTRIM()

Remove the space from the left of the string.

`LTRIM$` is a second name for this function.

### MID()

Return a subpart of the string. The first argument is the string, the second argument is the start position.
The third argument is the length of the sub-part in terms of characters.
If this argument is missing then the
subpart lasts to the last character of the argument `string`.

`MID$` is a second name for this function.

See also `LEFT`, `RIGHT`.

`mid(x,y,[z])` cuts out a sub-string from the string `x`.
If the first argument of the function is undefined the result is `undef`.
Otherwise the first argument is converted to string and the second and third
arguments are converted to numeric value. The third argument is optional.

The second argument specifies the start position of the resulting
substring in the original string x; and the last argument specifies
the number of characters to take from the original string `x`.
If the third argument is missing the substring lasts from the start
position to the end of the string. If the second argu-ment is
not defined the start of the substring is at the start of the
original string. In other words if the second argument is missing
it is the same as value 1. If the second argument is zero or negative
it will specify the start position counting the
characters from the end of the string.

If the staring position `y` points beyond the end of the string the result
is empty string. If the length of the substring is larger than the number
of characters between the starting position and end of the original string then the
result will be the substring between the start position and the end of the original string.

If the length of the substring is negative the characters before the starting position
are taken. No more than the available characters can be taken in this
case either. In other words if the length is negative and is larger in
absolute value than the starting position the resulting sub-string is the character
between the position specified by the second argument and the start of the string.

Note that the order of the characters is never changed even if some position or length
parameters are negative.

For compatibility reasons you can append a dollar (`$`) sign to the end of the function
identifier.

Example:
```
a$ = "superqualifragilisticexpialidosys"
print mid(a$,undef)
print mid(a$,1,5)
print mid(a$,undef,6)
print mid(a$,6,5)
print mid(a$,"-3")
print "*",mid(a$,0),"*"
print mid(undef,"66")
print mid(a$,6,-3)
print mid(a$,6,3)
print mid(a$,-4,-3)
print mid(a$,-4,3)
```

will print

```
superqualifragilisticexpialidosys
super
superq
quali
sys
**
undef
erq
qua
ido
osy
```

### OCT()

Take the argument as a long value and convert it to a string that represents the value in octal form.

### PACK()

Pack list of arguments into a binary string.

The format strings can contain the packing control literals.
Each of these characters optionally take the next argument and
convert to the specific binary string format. The result is the
concatenated sum of these strings.

Some control characters do not take argument, but result a constant
string by their own.

- `SZ` the argument is stored as zero terminated string. If the argument
already contains zchar that is taken as termnator and the rest of the
string is ignored.

- `S1` the argument is stored as a string. One byte length and maximum 255
byte strings. If the argument longer than 255 bytes only the first 255 bytes
are used, and the rest is ignored.

- `S2` same as `S1` but with two bytes for the length.

- `S3` same as `S1` but with three bytes for the length.

- `S4` same as `S1` but with four bytes for the length.

- `S5..8` the same as `S1` but with 5..8 bytes for the length.

- `Zn` one or more zero characters, does not take argument. `n` can be
   1,2,3 ... positive numbers

- `In` integer number stored on n bytes. Low order byte first. If the number does not
   fit into n bytes the higher bytes are chopped. If the number is negative the
   high overflow bytes are filled with FF.

- `C` character (same as `I1`)

- `Un` same as `In` but for unsigned numbers.

- `An` store the argument as string on n bytes. If the argument is longer than
   n bytes only the first n bytes are stored. If the argument is shorter than
   n bytes the higher bytes are filled with space.

- `R`  a real number.

See also `UNPACK`

### REPLACE()

This function replaces one or more occurrences of a sub-string in a string.
`REPLACE(a,b,c)` searches the string `a` seeking for occurrences of sub-string `b`
and replaces each of them with the string `c`.

The fourth and fifth arguments are optional. The fourth argument specifies the number of
replaces to be performed. If this is missing or is `undef` then all occurrences of string
`b` will be replaced. The fifth argument may specify the start position of the operation.
For example the function call

```
REPLACE("alabama mama", "a","x",3,5)
```

will replace only three occurrences of string `"a"` starting at position 5.
The result is `"alabxmx mxma"`.

### RIGHT()

Creates the right of a string. The first argument is the string. The second argument is the
number of characters that should be put into the result. If this value is larger than the
number of characters in the string then all the string is returned.

`RIGHT$` is a second name for this function.

See also `MID`, `LEFT`.

`RIGHT(x,y)` cuts out a substring of `y` characters from the right of the string `x`.
If the first argument is not defined the result is also `undef`. Otherwise the first argument
is converted to string and the second argument is converted to integer value.

If the second argument is not defined or has negative value it is treated as numeric zero
and as such the result string will be empty string.

For compatibility reasons you can append a dollar (`$`) sign to the end of the function
identifier.

### RTRIM()

Remove the space from the right of the string.

`RTRIM$` is a second name for this function.

### SET [NO] JOKER

Set a joker character to match certain characters when using the `LIKE` operator. The joker character
`"c"` can be one of the following characters

```
*  #  $  @  ?  &  %  !  +  /  |  <  >
```

The string after the keyword `TO` should contain all the characters that the joker
character should match. To have the character to match only itself to be a normal character
say
```
SET NO JOKER "c"
```

See also `SETWILD`, `LIKE` (details), `JOKER`

### SET [NO] WILD

Set a wild character to match certain characters when using the `LIKE` operator. The wild character
`"c"` can be one of the following characters

```
*  #  $  @  ?  &  %  !  +  /  |  <  >
```

The string after the keyword `TO` should contain all the characters that the wild card
character should match. To have the character to match only itself to be a normal character
say
```
SET NO WILD "c"
```

See also `SETJOKER`, `LIKE`  (details), `JOKER`

### SPACE()

Return a string of length `n` containing spaces.

### SPLIT string BY string TO var_1,var_2,var_3,...,var_n

Takes the string and splits into the variables using the second string as delimiter.

### SPLITA string BY string TO array

Split a string into an array using the second string as delimiter.
If the string has zero length the array becomes undefined.
When the delimiter is zero length string each array element will contain a single
character of the string.

See also `SPLIT`

### SPLITAQ string BY string QUOTE string TO array

Split a string into an array using the second string as delimiter.
The delimited fields may optionally be quoted with the third string.
If the string to be split has zero length the array becomes undefined.
When the delimiter is a zero length string each array element will contain a
single character of the string.

Leading and trailing delimiters are accepted and return an empty element
in the array. For example :-

```
   SPLITAQ ",'A,B',C," BY "," QUOTE "'" TO Result
```
   will generate
```
                 Result[0] = ""
                 Result[1] = "A,B"
                 Result[2] = "C"
                 Result[3] = ""
```

Note that this kind of handling of trailing and leading empty elements is different
from the handling of the same by the command `SPLIT` and `SPLITA` which do ignore
those empty elements. This command is useful to handle lines exported as CSV from
Excel or similar application.

The QUOTE string is really a string and need not be a single character. If there is an
unmatched quote string in the string to be split then the rest of the string until its end
is considered quoted.

If there is an unmatched

See also `SPLITA`

This command was suggested and implemented by Andrew Kingwell
(`Andrew.Kingwell@idstelecom.co.uk`)

### STR()

Converts a number to string. This function is rarely needed, because conversion is done automatically.
Converts a number to string. This function is rarely needed, because conversion is done automatically.
However you may need

```
 STRING(13,STR(a))
```

to be sure that the value `a` is interpreted as string value.

### STRING()

Create a string of length `n` containing characters `code`. If `code` is a string then the
first character of the string is used to fill the result. Otherwise `code` is converted to
long and the ASCII code is used.

### STRREVERSE()

Return the reversed string (aka. all the characters in the string in reverse order).

### TRIM()

Remove the space from both ends of the string.

`TRIM$` is a second name for this function.

### UCASE()

Uppercase a string.

`UPPER` is a second name for this function.

### UNPACK

Unpack the binary string `string` using the format string into the variables. The format string
should have the same format as the format string the in the function `PACK`.

### string LIKE pattern

Compare a string against a pattern.

```
      string LIKE pattern
```

The pattern may contain joker characters and wild card characters.
Pattern matching in ScriptBasic is similar to the pattern matching that you get used to on
the UNIX or Windows NT command line. The operator like compares a string to a pattern.

```
string like pattern
```

Both string and pattern are expressions that should evaluate to a string. If the pattern matches
the string the result of the operator is true, otherwise the result is false.

The pattern may contain normal characters, wild card characters and joker characters.
The normal characters match themselves. The wild card characters match one or more characters from the
set they are for. The joker characters match one character from the set they stand for. For example:

```
Const nl="\n"
print "file.txt" like "*.txt",nl
print "file0.txt" like "*?.txt",nl
print "file.text" like "*.txt",nl
```

will print

```
-1
-1
0
```

The wild card character `*` matches a list of characters of any code. The joker character `?` matches
a single character of any code. In the first print statement the `*` character matches the string file
and `.txt` matches itself at the end of the string. In the second example `*` matches the string file
and the joker `?` matches the character `0`. The wild card character `*` is the most general wild card
character because it matches one or more of any character. There are other wild card characters.
The character `#` matches one or more digits, `$` matches one or more alphanumeric characters and
finally `@` matches one or more alpha characters (letters).

```
*	all characters
#	0123456789
$	0123456789abcdefghijklmnopqrstxyvwzABCDEFGHIJKLMNOPQRSTXYVWZ
@	abcdefghijklmnopqrstxyvwzABCDEFGHIJKLMNOPQRSTXYVWZ
```

A space in the pattern matches one or more white spaces, but the space is not a regular wild card character,
because it behaves a bit different.

Note that wild card character match ONE or more characters and not zero or more as in other systems. Joker
characters match exactly one character, and there is only one joker character by default, the
character `?`, which matches a single character of any code.

We can match a string to a pattern, but that is little use, unless we can tell what
substring the joker or wildcard characters matched. For the purpose the function joker is available. The argument
of this function is an integer number, n starting from 1 and the result is the substring that the last
pattern matching operator found to match the nth joker or wild card character. For example

```
Const nl="\n"
if "file.txt" like "*.*" then
  print "File=",joker(1)," extension=",joker(2),nl
else
  print "did not match"
endif
```

will print

```
File=file extension=txt
```

If the pattern did not match the string or the argument of the function joker is zero or negative, or is larger
than the serial number of the last joker or wild card character the result is `undef`.

Note that there is no separate function for the wild card character substrings and one for the joker characters.
The function joker serves all of them counting each from left to right. The function joker does not count,
nor return the spaces, because programs usually are not interested in the number of the spaces that separate
the lexical elements matched by the pattern.

Sometimes you want a wild card character or joker character to match only itself. For example you want to
match the string `"13*52"` to the pattern two numbers separated by a star. The problem is that the star
character is a wild card character and therefore `"#*#"` matches any string that starts and ends with a digit.
But that may not be a problem. A `*` character matches one or more characters, and therefore `"#*#"` will
indeed match `"13*52"`. The problem is, when we want to use the substrings.

```
Const nl="\n"
a="13*52" like "#*#"
print joker(1)," ",joker(3),nl
a="13*52" like "#~*#"
print joker(1)," ",joker(2),nl
```

will print

```
1 52
13 52
```

The first `#` character matches one character, the `*` character matches the substring `"3*"` and the
final `#` matches the number `52`.

The solution is the pattern escape character. The pattern escape character is the tilde
character: `~`. Any character following the `~` character is treated as normal character
and is matched only by itself. This is true for any normal character, for wild card
characters; joker characters; for the space and finally for the tilde character itself.
The space character following the tilde character matches exactly one space characters.

Pattern matching is not always as simple as it seems to be from the previous examples.
The pattern `"*.*"` matches files having extension and `joker(1)` and `joker(2)` can
be used to retrieve the file name and the extension. What about the file `sciba_source.tar.gz`?
Will it result

```
File=scriba_source.tar extension=gz
```

or

```
File=scriba_source extension=tar.gz
```

The correct result is the second. Wild card characters implemented in ScriptBasic are not greedy.
They eat up only as many characters as they need.

Up to now we were talking about wild card characters and the joker character defining what
matches what as final rule carved into stone. But these rules are only the default behavior of
these characters and the program can alter the set of characters that a joker or wild card character matches.

There are 13 characters that can play joker or wild card character role in pattern matching. These are:

```
*  #  $  @  ?  &  %  !  +  /  |  <  >
```

When the program starts only the first five characters have special meaning the others are normal characters. To change the role of a character the program has to execute a set joker or set wild command. The syntax of the commands are:

set joker expression to expression
set wild expression to expression

Both expressions should evaluate to string. The first character of the first string should be the joker or wild card character and the second string should contain all the characters that the joker or wild card character matches. The command set joker alters the behavior of the character to be a joker character matching a single character in the compared string. The command set wild alters the behavior of the character to be a wild card character matching one or more characters in the compared string. For ex-ample if you may want the & character to match all hexadecimal characters the program has to execute:

set wild "&" to "0123456789abcdefABCDEF"

If a character is currently a joker of wild card character you can alter it to be a normal character issuing one of the commands

set no joker expression
set no wild expression

where expression should evaluate to a string and the first character of the string should give the character to alter the behavior of.

The two commands are identical, you may always use one or the other; you can use set no joker for a character being currently wild card character and vice versa. You can execute the command even if the character is currently a normal character in the pattern matching game.

Using the commands now we can see that

## test

### ISDEFINED()

This function can be used to determine whether an expression is defined or undefined (aka `undef`). If the argument is a defined value then the function returns `true`, otherwise the function returns `false`.

The argument is evaluated exactly once, like any expression. When it is a
function call the function runs: if it returns `undef`, or no value at all,
the test answers `false` and never reads the missing value. A definition
test therefore cannot fail on the very thing it is asked to inspect, even
when the call is the inner argument of the test.

This function is the counter function of `ISUNDEF`.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### ISEMPTY()

This function can be used to determine whether an expression holds an empty string. Because programmers tend to use the value `undef` where empty string would be more precise the function returns `true` if the argument is `undef`. Precisely:

The function returns true if the argument is `undef` or a string containing zero characters. Otherwise the function returns `false`.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `TYPE`.

### ISINTEGER()

This function can be used to determine whether an expression is integer or some other type of value. If the argument is an integer then the function returns `true`, otherwise the function returns `false`.

See also `ISARRAY`, `ISSTRING`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### ISNUMERIC()

This function can be used to determine whether an expression is numeric (real or integer) or some other type of value. If the argument is a real or an integer then the function returns `true`, otherwise the function returns `false`.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### ISREAL()

This function can be used to determine whether an expression is real or some other type of value. If the argument is a real then the function returns `true`, otherwise the function returns `false`.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`, `TYPE`.

### ISUNDEF()

This function can be used to determine whether an expression is defined or undefined (aka `undef`). If the argument is a defined value then the function returns `false`, otherwise the function returns `true`.

This function is the counter function of `ISDEFINED`.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISEMPTY`, `TYPE`.

### TYPE()

This function can be used to determine the type of an expression.
The function returns a numeric value that describes the type of the argument. Although the numeric values are guaranteed to be the one defined here it is recommended that you use the predefined symbolic constant values to compare the return value of the function against. The function return value is the following

- `SbTypeUndef`   0 if the argument is `undef`.
- `SbTypeString`  1 if the argument is string.
- `SbTypeReal`    2 if the argument is real.
- `SbTypeInteger` 3 if the argument is integer.
- `SbTypeArray`   4 if the argument is an array.

See also `ISARRAY`, `ISSTRING`, `ISINTEGER`, `ISREAL`, `ISNUMERIC`, `ISDEFINED`, `ISUNDEF`, `ISEMPTY`.

## time

### AddDay()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the day by the second argument and returns the time value for the same hour and minute but some days later or sooner in case the second argument is negative.

This function is very simple from the arithmetic's point of view, because it simply adds 86400 times the second argument to the first argument and returns the result.

### AddHour()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the hours by the second argument and returns the time value for the same minute and seconds but some hours later or sooner in case the second argument is negative.

This function is very simple from the arithmetic's point of view, because it simply adds 3600 times the second argument to the first argument and returns the result.

### AddMinute()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the minutes by the second argument and returns the time value for the same seconds but some minutes later or sooner in case the second argument is negative.

This function is very simple from the arithmetic's point of view, because it simply adds 60 times the second argument to the first argument and returns the result.

### AddMonth()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the month by the second argument and returns the time value for the same day, hour and minute but some months later or sooner in case the second argument is negative.

If the resulting value is on a day that does not exist on the result month then the day part of the result is decreased. For example:

```
print FormatTime("MONTH DAY, YEAR",AddMonth(TimeValue(2000,03,31),1))
```

will print

```
April 30, 2000
```

### AddSecond()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the seconds by the second argument and returns the time value.

This function is the simplest from the arithmetic's point of view, because it simply adds the second argument to the first argument and returns the result.

### AddWeek()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the week by the second argument and returns the time value for the same hour and minute but some weeks later or sooner in case the second argument is negative.

This function is very simple from the arithmetic's point of view, because it simply adds 604800 times the second argument to the first argument and returns the result.

### AddYear()

This function takes two arguments. The first argument is a time value, the second is an integer value. The function increments the year of the time value by the second argument and returns the time value for the same month, day, hour and minute but some years later or sooner in case the second argument is negative.

This is a bit more complex than just adding 365*24*60*60 to the value, because leap-years are longer and in case you add several years to the time value you should consider adding these longer years extra days. This is calculated correct in this function.

If the original time value is February 29 on a leap-year and the resulting value is in a year, which is not leap year the function will return February 28.

Note that because of this correction using the function in a loop is not the same as using it once. For example:

```
print AddYear(TimeValue(2000,02,29),4),"\n"
print AddYear(AddYear(TimeValue(2000,02,29),2),2),"\n"
```

will print two different values.

### Day()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the day of the month (1 to 31) value of that time. If the argument is missing the function uses the actual local time to calculate the day of the month value. In other words it returns the day value of the actual date.

### FORMATDATE()

```
FormatDate("format",time)
```

Formats a time value (or date) according to the format string. The format string may contain placeholders. The first argument is the format string the second argument is the time value to convert. If the second argument is missing or `undef` then the local time is converted.

This function uses the first argument as a formatting string. This string may contain the following character strings:

```
     YEAR         four digit year
     YY           two digit year
     MON          three letter abbreviation of the month name
     MM           month
     0M           month with leading zero if needed
     *MONTH-NAME* name of the month
     DD           day of the month
     0D           day of the month with leading zero if needed
     WD           week day on a single digit starting with sunday=0
     WEEKDAY-NAME the name of the weekday
     WDN          three letter abbreviation fo the week day name
     HH           hours (24 hours notation)
     0H           hours with leading zero if needed (24 hours notation)
     hh           hours (12 hours notation)
     0h           hours with leading zero if needed (12 hours notation)
     mm           minutes
     0m           minutes with leading zero if needed
     am
     pm   is am or pm
```

Any other character in the format string will get into the result verbatim.

A `%` followed by a letter or a digit is not a pattern of this language; it
is rejected with `COMMAND_ERROR_INVALID_TIME_FORMAT` instead of being copied
into the result.

`FORMATTIME` is a second name for this same function; it is not a separate
command and takes the same patterns.

### GMTIMETOLOCALTIME()

This is the function originally documented as `GmToLocalTime()`; the name to
call is `GMTIMETOLOCALTIME()`.

This function accepts one argument that has to be the number of seconds elapsed since January 1, 1970 0:00 am in GMT. The function returns the same number of seconds in local time. In other words the function converts a GMT time value to local time value.

### GmTime()

This function returns the GMT time expressed as seconds since January 1, 1970, 00:00am. The function does not accept any argument. This function is similar to the function `NOW` but returns the GMT time instead of the actual local time.

### Hour()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the hour value of that time. If the argument is missing the function uses the actual local time.

### LOCALTIMETOGMTIME()

This is the function originally documented as `LocalToGmTime()`; the name to
call is `LOCALTIMETOGMTIME()`.

This function accepts one argument that has to be the number of seconds elapsed since January 1, 1970 0:00 am in local time. The function returns the same number of seconds in GMT. In other words the function converts a local time value to GMT time value.

### Minute()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the minute value of that time. If the argument is missing it uses the actual local time.

### Month()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the month (1 to 12) value of that time. If the argument is missing it uses the actual local time. In other words it returns the actual month in this latter case. The months are numbered so that January is 1 and December is 12.

### NOW()

This function returns the local time expressed as seconds since January 1, 1970, 00:00am. The function does not accept any argument. This function is similar to the function `GMTIME` but returns the local time instead of the actual GMT.

### Sec()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the seconds value of that time. If the argument is missing the function uses the actual local time.

### TimeValue()

This function gets zero or more, at most six arguments and interprets them as year, month, day, hour, minute and seconds and calculates the number of seconds elapsed since January 1, 1970 till the time specified. If some arguments are missing or `undef` the default values are the following:
- year = 1970
- month = January
- day = 1st
- hours = 0
- minutes = 0
- seconds = 0

### WeekDay()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the week day value of that time. If the argument is missing the function uses the actual local time. In other words it returns what day it is at the moment.

### Year()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the year value of that time. If the argument is missing it uses the actual local time to calculate the year value. In other words it returns the actual year.

### YearDay()

This function accepts one argument that should express the time in number of seconds since January 1, 1970 0:00 am and returns the year-day value of that time. This is actually the number of the day inside the year so that January 1st is #1 and December 31 is #365 (or 366 in leap years). If the argument is missing the function uses the actual local time.
