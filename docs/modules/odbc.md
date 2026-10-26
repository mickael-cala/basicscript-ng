<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `odbc`

Source : `src/extensions/odbc/interface.c`

ODBC Database Connection Module

This module implements ODBC interface to connect to relational databases. Using this module
you can use SQL RDBMS implementations from Windows/Unix/Linux platform independant of the
actual database system.

Though this module does not deliver all functions of the ODBC system it does implement
most of the functions that are needed to write SQL based applications. You can connect to
ODBC databases, disconnect, issue SQL commands and retrieve the data resulted by the queries.

To use this module from a BASIC program you have to include the file `odbc.bas` with the line

```
import odbc.bas
```

to have all the neccessary function defintiions. This file and the neccessary `odbc.dll` (or `odbc.so`)
are installed in the include and in the modules directories on Windows.

A typical BASIC application using this module first calls `Connect` or `RealConnect` to get a
connection to the database. Then the program calls `Query` to execute SQL queries, altering the
database and `FetchHash` and/or `FetchArray` to get the result of queries. Finally the program
optionally calls `Close` to close the connection.

(Calling `Close` is optional. `Close` is automatically called for each opened database connection
when the interpreter finishes.)

The underlying ODBC system layer implements connection pooling on process level. Thus there is
no need to use the ScriptBasic resource pooling module as it is done by the MySQL module. (By the
time I write this the ScriptBasic resource pool support module is still experimental, and so is the
MySQL module interface utilizing it.)

When the ODBC module is initialized the module requests the underlying ODBC layer to perform process
level connection pooling. The module also implements the multi-thread interface functions that allow
the module to remain in memory if used in multi-thread application.

The module raises module specific errors. For more information see `Error`.

## Error Codes

The module raises error in case when there is some problem with the underlying
database handling. The error codes the module may raise:

- 0x00081001 there is no such connection name
- 0x00081002 there is some problem with the connection name. This is an error in the configuration file.
- 0x00081003 the connection was refused by the database.
- 0x00081004 query execution error.
- 0x00081005 the program tries to call `FetchArray` or `Fetchhash` without a preceeding query execution.
- 0x00081006 the second argument of a fetch does not evaluate to left value.

## FetchArray

```
R = ODBC::FetchArray(DB,arr)
```

Use this function to fetch one row from the results after a successfully executed `SELECT` statement.
The first argument to the function is `DB` the database handle. The second argument should be a variable.
This variable will hold the array containing the data of the actual row. The array is indexed from zero to
`n-1` where `n` is the number of columns in the result set.

Use consecutive calls to this fucntion to get the rwos of the result one after the other. You can mix
consecutive calls to `FetchHash` and `FetchArray`.

The function returns `-1` (TRUE) if fetching a row of data was successful or `0` (FALSE) if there
are no more rows in the dataset. In the latter case `arr` will be `undef`.

See also `FetchHash`

This function calls the ODBC functions `SQLFetch`, `SQLDescribeCol`, `SQLGetData`.

## FetchHash

```
R = ODBC::FetchHash(DB,arr)
```

Use this function to fetch one row from the results after a successfully executed `SELECT` statement.
The first argument to the function is `DB` the database handle. The second argument should be a variable.
This variable will hold the hash-array containing the data of the actual row. The array is indexed from zero to
`2*n-1` where `n` is the number of columns in the result set. Every even element (starting with index zero) will
hold the name of a column and the next odd element of the array will hold the value of the column. This is
according to the storage strategy of ScriptBasic for hashes.

After the successful execution of this function you will be able to access the value of the column named `"column"`
with the syntax `arr{"column"}`. Note however that column names in SQL are usually case insensitive, but ScriptBasic
hash indexing is case sensitive.

Use consecutive calls to this fucntion to get the rows of the result one after the other. You can mix
consecutive calls to `FetchHash` and `FetchArray`.

The function returns `-1` (TRUE) if fetching a row of data was successful or `0` (FALSE) if there
are no more rows in the dataset. In the latter case `arr` will be `undef`.

See also `FetchArray`.

This function calls the ODBC functions `SQLFetch`, `SQLDescribeCol`, `SQLGetData`.

## AffectedRows

```
R = ODBC::AffectedRows(DB)
```

Use this function to get the number of rows affected by a previous `UPDATE`, `DELETE` or
`INSERT` command. The function returns undefined number in case the last executed was
none of the above command types.

The underlying ODBC function this function calls is `SQLRowCount`.

## Error

```
R = ODBC::Error(DB)
```

Use this function to get the txtual report of an error that has happened during an ODBC call.
Calling ScriptBasic ODBC module functions raise error if the underlying ODBC calls report some error.
This error can be captured using the ScriptBasic `ON ERROR GOTO` function. The error handling routine can call
this function to get the text of the error as reported by the ODBC driver.

The return value is the text of the error.

The underlying ODBC function this function calls is `SQLError`.

## Query

```
ODBC::Query DB, query
```

This function should be used to execute an SQL statement. The first argument
`DB` is the connection handle, the second argument `query` is the text of the
SQL query.

The function returns `undef`.

The function calls the underlying ODBC function `SQLExecDirect` and `SQLNumResultCols`.

## Close

```
ODBC::Close DB
```

Use this function to release connection. The connection may still be kept alive by the
underlying ODBC layer for later use.

The function returns `undef`.

The function calls the underlying ODBC function `SQLDisconnect` and `SQLFreeHandle`.

## RealConnect

```
DB = ODBC::RealConnect(DSN,User,Password)
```

Use this fucntion to connect to an ODBC data source. The arguments are the `DSN` data source name as
configured in the ODBC connection manager, the `User` user name, and the `Password` password.

The return value is the database handle to be used in consecutive calls to `Query`, `FetchArray`,
`FetchHash` and `Close`. Note that this handle is a handle that ScriptBasic uses internally and is
not the handle returned by the underlying ODBC functions. ScriptBasic, for security reasons keeps track of
the handles using handle tables and return small integer numbers to the BASIC program to be used as reference.

You should not use any of these handles to perform any calculation other than passing them to the other
functions to identify connections. You can, however freely copy these numbers from variable to the other,
passing to fucntions and so on as they are simply small integer numbers.

If the connection can not be established for some reason the function raises a module specific error.

See also `Connect`, `Error`.

The function calls the underlying ODBC function `SQLAllocHandle` and `SQLConnect`.

## Connect

```
DB = ODBC::Connect(connection_name)
```

Use this fucntion to connect to an ODBC data source. The argument is the name of the connection as
configured in the ScriptBasic configuration file.

Connection to an ODBC database usually requires a user name and password. Coding these into
script source files may not be secure and thus this module allows the system manager to
configure these codes in the ScriptBasic config file. This config file may not be readable by
ordinary user and is binary in the form as read by the ScriptBasic interpreter. This adds some
extra security.

Calling this function instead of `RealConnect` the programmer may reference a connection configured
in the ScriptBasic configuration file. The key with the name of the connection should be under the
key `odbc.connections`. For example:

```
odbc (
  connections (
    test (        ; the name of the connection
	  dsn "odbctest" ; data source name
	  user "" ; user for the connection, to test we use MS-Access thus there is no user or passwd
	  password "" ; password for the connection
	  )
    )
  )
```

When configuring such an ODBC connection do not forget to recompile the configuration file.

The name of the connection passed to the function should be the name used in the configuration file
to identify the connection. In the example above this is `test`. This name need not, howeve may be the same
as the data source name.

If the connection can not be established for some reason the function raises a module specific error.

See also `RealConnect`, `Error`.

The function calls the underlying ODBC function `SQLAllocHandle` and `SQLConnect`.
