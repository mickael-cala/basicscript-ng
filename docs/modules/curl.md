<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `curl`

Source : `src/extensions/curl/interface.c`

The CURL module provides easy and still powerful functions to perform various network
oriented actions that are based on URL to access data. For example CURL can perform
getting data via HTTP, HTTPS from web pages, POST data to web pages, transer file to
and from servers via FTP and so on.

To use the module the program has to import the BASIC "header" file named `curl.sbm`. This
should be done using the command import:

```
import curl.sbm
```

Note that there are no double quotes around the file name to include the definition of the
C implemented curl functions from the module interface header files include directory.

There are only a few functions defined in this file. The first function that an application
has to call is `curl::init()` to get a handle to a connection. A single program can use
several connections simultaneous, tough currently there is no possibility to download large
files asynchronous.

Follwing the initialization the program has to set several options calling `curl::option`, and then
should call the function `curl::perform` to actually perform the download/upload or other network
action. When a connection is not used anymore the function `curl::finish` can be called, though this
function is executed automatically for each connection when the interpreter exists.

```
import curl.sbm
ON ERROR GOTO CURL_ERROR
CURL = curl::init()
curl::option CURL,"URL","http://scriptbasic.com/html/index.html"
curl::option CURL,"FILE","C:\\ScriptBasicMirror\\html\\index.html"
curl::perform CURL
curl::finish
STOP
CURL_ERROR:
PRINT "Some error happened while trying to download ScriptBasic home page. The error message is:\n"
PRINT curl::error()
STOP
```

For more information on other functions read the appropriate chapters.

The name CURL is made of the name of the programming language C in which CURL is implemented
and from the word URL. This text that you are reading is the documentation of the ScriptBasic
implementation of CURL. This implementation includes an interface module that containc C coded
BASIC callable function that map most (or some) of the CURL library functions. In other words
using this program yu can access most (or some) of the functions that the CURL library provides.

Altough the CURL library is quite flexible, there are some options that are altered in this
module to ease the use for BASIC programmers. For example the option POSTFIELDS accepts zero
terminated string by default, because 1.) POST data usually does not contain zero character and 2.)
this is the easy way for C programmers. On the other hand there is a possibility, though more complex
to pass string and length to the option. Because all ScriptBasic strings are arbitrary binary data and
not zero character terminated strings this second method is followed.

Some of the options on the C level accept `long` numbers from a finite set and CURL C header files define
`enum` types for the purpose. In these cases the ScriptBasic interface requires strings being specified
by the BASIC program as option and convert to `long` as appropriate to CURL.

The option names in the original CURL interface are defined in `enum` and thus options are defined
as `long` constants. The BASIC interface works with strings, and converts the names specified as
strings to their appropriate value. This is (I assume) simpler for the BASIC programmers and on the other
hand induces only slight overhead as compared to network transfer times.

## curl::init

Call this function before any other curl action. This function will return a handle
to the curl session. This handle should be used as first argument to any further curl calls.

Usage:
```
CURL = curl::init()
```

The handle that the function returns is a small integer. However the program should not alter this value in any
way. The good practice is to store the value in a variable and use it as it is without caring about the real
value or type of the content.

If the initialization of the connection can not be performed `MEMORY_LOW` error occurs.

## curl::option

Call this function to set the various options of the actual curl session.

Usage:
```
curl::option CURL, "option" [, value]
```

The various options that the current version of the module supports are listed in the following sections.
Note that the documentation is mainly copied from the original CURL lib documentation.

Some of the options require a string as a parameter. The strings usually should not contain zero character.
The exception is the option `POSTFIELDS`, which is handled in a special way to allow the programmer to send
arbitrary binary data as HTTP POST parameter.

The option names in the section titles in this document are followed by one of the words *string*, *integer* and *flag*.

- 
Options followed by the word *string* need string argument. If the option value is not specified zero length
string is used. Because this is nonsense in most cases, you better use a third string argument for these options.
- 
Options followed by the word *flag* are yes/no type options. Presenting them without value means switching the
option on. To switch such an option off the programmer has to explicitely specify zero value as third argument.
In other words omitting the option value in such a case is the same as specifying `1` or `TRUE` or any other
non-zero value. To switch off a *flag* option you can specify the value as `FALSE` (which is zero in ScriptBasic)
for more readability.
```
curl::option CURL,"VERBOSE"
curl::option CURL,"VERBOSE",TRUE
curl::option CURL,"VERBOSE",1
```
are the same as well as the oposit
```
curl::option CURL,"VERBOSE",FALSE
curl::option CURL,"VERBOSE",0
```
are the same.
- 
Options followed by the word *integer* accept integer value. If the value is missing for such an option zero is used.
For example:
```
curl::option CURL,"RESUME_FROM"
curl::option CURL,"RESUME_FROM",0
```
are the same.

The option names are implemented case sensitive in this module, thus you can not use `"verbose"`
instead of `"VERBOSE"`. Also the programmer should type the option names precisely, no mispelling is
tolerated by the program.

### BUFFER_SIZE integer

```
curl::option CurlHandle,"BUFFER_SIZE",1024
```

When a file is downloaded but not stored in a file the function `curl::perform(CURL)` returns the content
of the downloaded file as a huge string. During the download this string is stored ina temporary buffer. The
size of this buffer increases gradually as more and more bytes come. If there are 10 bytes coming at a time then
the buffer will grow only ten bytes. This also means a new buffer allocation and copying the content of the buffer,
which consumes system resources, especially for large files.

If you happen to know the estimated size of the file, you can set the initial size of the buffer to a huge value using this
option. For example if you know that the file is 1024 bytes, you can set this option as in the example above. In that case
when the first byte comes from the URL the 1024 byte length buffer is increased and when the consecutive bytes come there is
space to store them without reallocating the buffer.

You need not worry about using this buffer when you handle small files, like web pages. If you see performace or memory
shortage problems, then you may consider this option along with the option `FILE` that helps you store the downloaded file
on disk.

### CAINFO string

```
curl::option CurlHandle,"CAINFO","c:\\certs\mycert.pem"
```

Pass a string file naming holding the certificate to
verify the peer with. This only makes sense when used in combination with the
CURLOPT_SSL_VERIFYPEER option.

### COOKIE string

```
curl::option CurlHandle,"COOKIE","mycookie=is rather long"
```

Pass a string as parameter. It will be used to
set a cookie in the http request. The format of the string should be
`[NAME]=[CONTENTS];` Where `NAME` is the cookie name.

### COOKIEFILE string

```
curl::option CurlHandle,"COOKIEFILE","c:\\WINDOWS\\cookies.txt"
```

Pass a string as parameter. It should contain the
name of your file holding cookie data. The cookie data may be in Netscape /
Mozilla cookie data format or just regular HTTP-style headers dumped to a
file.

### CUSTOMREQUEST string

```
curl::option CurlHandle,"CUSTOMREQUEST","MOVE"
```

Pass a string as parameter. It will be user
instead of GET or HEAD when doing the HTTP request. This is useful for doing
DELETE or other more obscure HTTP requests. Don't do this at will, make sure
your server supports the command first.

### FILE

```
curl::option CurlHandle,"FILE","file_name"
```

Use this option to set the file name of the file where the result will be saved. When you set this option
the file is opened and truncated assumed that the program has appropriate privileges. Thus if there was a
file with the name the file will be overwrtitten even if the `curl::perform` function is not called. The file
is opened, when the option is set and kept opened so long as the connection is "finished" or another
`INFILE` or `FILE` option is specified for the connection.

If you donot specify any file to store the downloaded result, then the function `curl::perform(CURL)`
will return the file as a single huge string.

Comment on internals (if you do not understand what I am talking about, most probably you do not need to):

The underlying CURL library requests an opened file handle passed to the library and a function that
performs the file writing. The ScriptBasic interface gets the name of the file, opens the file, passes
the opened file pointer to the library and specifies the stadard `fwrite` function to write the file.

The pointer to the function `fwrite` is taken from the support table, thus is any preloaded module
altered this before setting this option the module function will be used.

The file is opened calling the system function `fopen` via the ScriptBasic support function calling stacks.
This means that if some module implements a hook function to control file access that will be taken into account
the same way as it is taken into accoung in the case of the BASIC command `OPEN`.

### FTPPORT string

```
curl::option CurlHandle,"FTPPORT","222"
```

Pass a string as parameter. It will be used to
get the IP address to use for the ftp PORT instruction. The PORT instruction
tells the remote server to connect to our specified IP address. The string may
be a plain IP address, a host name, an network interface name (under unix) or
just a '-' letter to let the library use your systems default IP address.

### HEADERFILE string

```
curl::option CurlHandle,"HEADERFILE","file_name"
```

Use this option to set the file name of the file where the header coming from the server will be saved.
When you set this option the file is opened and truncated assumed that the program has appropriate privileges.
Thus if there was a file with the name the file will be overwrtitten even if the `curl::perform` function is
not called. The file is opened, when the option is set and kept opened so long as the connection is "finished"
or another `HEADERFILE` option is specified for the connection.

Note: You need this option if you want to get the header and the body of a downloaded file into two separate
files. You can easily get the header and the body into a single file if you use the option `HEADER`. Altough
there is nothing that prevents, I see no real use of the two options aka `HEADERFILE` and flag `HEADER`) together.
But you are free to use them together.

Comment on internals (if you do not understand what I am talking about, most probably you do not need to):

The underlying CURL library requests an opened file handle passed to the library and a function that
performs the file writing. The ScriptBasic interface gets the name of the file, opens the file, passes
the opened file pointer to the library and specifies the stadard `fwrite` function to write the file.

The pointer to the function `fwrite` is taken from the support table, thus is any preloaded module
altered this before setting this option the module function will be used.

The file is opened calling the system function `fopen` via the ScriptBasic support function calling stacks.
This means that if some module implements a hook function to control file access that will be taken into account
the same way as it is taken into accoung in the case of the BASIC command `OPEN`.

### INFILE string

```
curl::option CurlHandle,"INFILE","file_name"
```

Use this option to set the file name of the file where the source is for file upload. When you set this option
the file is opened for reading, thus the file should exist and should be readable for the program. The file
is opened, when the option is set and kept opened so long as the connection is "finished" or another
`INFILE` or `FILE` option is specified for the connection.

You cannot use the options `INTEXT` and `INFILE` at the same time for the same connection. If you use both
the latter specified will be used.

Comment on internals (if you do not understand what I am talking about, most probably you do not need to):

The underlying CURL library requests an opened file handle passed to the library and a function that
performs the file writing. The ScriptBasic interface gets the name of the file, opens the file, passes
the opened file pointer to the library and specifies the stadard `fread` function to read the file.

The pointer to the function `fread` is taken from the support table, thus is any preloaded module
altered this before setting this option the module function will be used.

Curl optionally acepts the size of the file to report to the upload server. The implementation of this
option interfacing automatically gets the size of the file specified in the option and sets the underlying
option.

The file is opened calling the system function `fopen` via the ScriptBasic support function calling stacks.
This means that if some module implements a hook function to control file access that will be taken into account
the same way as it is taken into accoung in the case of the BASIC command `OPEN`.

### INTERFACE string

```
curl::option CurlHandle,"INTERFACE","16.193.68.55"
```

Pass a string as parameter. This set the interface name to use as outgoing
network interface. The name can be an interface name, an IP address or a host
name.

### INTEXT string

```
curl::option CurlHandle,"INTEXT","""This is the content of the file
that we will upload.
"""
```

Use this option to set the content of the file for any kind of upload. This option can be used to upload
small files via HTTP or FTP, which are generated on-the-fly by the BASIC program, and there is no need to
store the files locally.

You cannot use the options `INTEXT` and `INFILE` at the same time for the same connection. If you use both
the latter specified will be used.

### KRB4LEVEL string

```
curl::option CurlHandle,"KRB4LEVEL","confidential"
```

Pass a string as parameter. Set the krb4 security level, this also enables
krb4 awareness.  This is a string, 'clear', 'safe', 'confidential' or
'private'.  If the string is set but doesn't match one of these, 'private'
will be used. Set the string to NULL to disable kerberos4. The kerberos
support only works for FTP.

### PROXY string

```
curl::option CurlHandle,"PROXY","www.my-proxy.com:8080"
```

If you need libcurl to use a http proxy to access the outside world, set the
proxy string with this option. To specify port number in this string, append `:[port]` to
the end of the host name. The proxy string may be prefixed with
`[protocol]://` since any such prefix will be ignored.

### PROXYPORT integer

```
curl::option CurlHandle,"PROXYPORT",8080
```

Use this option to set the proxy port to use unless it is
specified in the proxy string `PROXY`.

### PROXYUSERPWD string

```
curl::option CurlHandle,"PROXYUSERPWD","verhas:m23kkdUT"
```

Pass a string as parameter, which should be `username:password` to use for
the connection to the HTTP proxy. If the password is left out, you will be
prompted for it.

### RANDOM_FILE string

```
curl::option CurlHandle,"RANDOM_FILE","c:\\WINNT4\pagefile.sys"
```

Pass a string to a zero terminated file name. The file will be used to read
from to seed the random engine for SSL. The more random the specified file is,
the more secure will the SSL connection become.

### RANGE string

```
curl::option CurlHandle,"RANGE","3321-3322"
```

Pass a string as parameter, which should contain the specified range you
want. It should be in the format "X-Y", where X or Y may be left out. HTTP
transfers also support several intervals, separated with commas as in
`"X-Y,N-M"`. Using this kind of multiple intervals will cause the
HTTP server to send the response document in pieces.

### REFERER string

```
curl::option CurlHandle,"REFERER","http://www.scriptbasic.com"
```

Pass a string as parameter. It will be used to
set the `Referer:` header in the http request sent to the remote server. This
can be used to fool servers or scripts.

### SSLCERT string

```
curl::option CurlHandle,"SSLCERT","???"
```

Pass a string as parameter. The string should be
the file name of your certficicate in PEM format.

### SSLCERTPASSWD string

```
curl::option CurlHandle,"SSLCERTPASSWD","m23kkdUT"
```

Pass a string as parameter. It will be used as
the password required to use the `SSLCERT` certificate. If the password
is not supplied, you will be prompted for it.

### URL string

```
curl::option CurlHandle,"URL","http://curl.haxx.se"
```

The actual URL to deal with.

NOTE: this option is required to be set before `curl::perform()` is called.

### USERAGENT string

```
curl::option CurlHandle,"USERAGENT","CURL 7.9.5 with SB Interface; v10b29"
```

Pass a string as parameter. It will be used to
set the `User-Agent:` header in the http request sent to the remote server. This
can be used to fool servers or scripts.

### USERPWD string

```
curl::option CurlHandle,"USERPWD","verhas:m23kkdUT"
```

Pass a string as parameter, which should be `username:password` to use for
the connection. If the password is left out, you will be prompted for it.

### WRITEINFO string

```
curl::option CurlHandle,"WRITEINFO",""
```

Pass a string as parameter. It will be used to
report information after a successful request.

As report and progress callback is not implemented in ScriptBasic CURL module, there
is not much use of this option.

### EGDSOCKET string

```
curl::option CurlHandle,"EGDSOCKET","\\.\\edg"
```

Pass a string path name to the Entropy Gathering Daemon
socket. It will be used to seed the random engine for SSL.

### POSTFIELDS string

```
curl::option CurlHandle,"POSTFIELDS","name=Edmund+Nielsen+Bohr&address=Downing+street+11"
```

Pass a string as parameter, which should be the full data to post in a HTTP
post operation.

Note that CURL library also implements the option `CURLOPT_POSTFIELDSIZE` for C programmers.
That option is automatically called by the interface function, thus any binary BASIC string
can be used as post parameter.

### HTTPPROXYTUNNEL flag

```
curl::option CurlHandle,"HTTPPROXYTUNNEL"
```

Set the parameter to get the library to tunnel all non-HTTP
operations through the given HTTP proxy. Do note that there is a big
difference to use a proxy and to tunnel through it. If you don't know what
this means, you probably don't want this tunnel option.

### VERBOSE flag

```
curl::option CurlHandle,"VERBOSE"
```

Set the parameter to get the library to display a lot of verbose
information about its operations. Very useful for libcurl and/or protocl
debugging and understanding.

### NOPROGRESS flag

```
curl::option CurlHandle,"NOPROGRESS"
```

Setting the parameter tells the library to shut of the built-in progress meter
completely. (NOTE: future versions of the lib is likely to not have any
built-in progress meter at all).

### HEADER flag

```
curl::option CurlHandle,"HEADER"
```

Setting the parameter tells the library to include the header in the
output. This is only relevant for protocols that actually has a header
preceeding the data (like HTTP).

### NOBODY flag

```
curl::option CurlHandle,"NOBODY"
```

Setting the parameter tells the library to not include the body-part in the
output. This is only relevant for protocols that have a separate header and
body part.

### FAILONERROR flag

```
curl::option CurlHandle,"FAILONERROR"
```

Setting the parameter tells the library to fail silently if the HTTP code
returned is equal or larger than 300. The default action would be to return
the page normally, ignoring that code.

### UPLOAD flag

```
curl::option CurlHandle,"UPLOAD"
```

Setting the parameter tells the library to prepare for an upload. The
option `INFILE` is also interesting for uploads.

### POST flag

```
curl::option CurlHandle,"POST"
```

Setting the parameter tells the library to do a regular HTTP post. This is a
normal `application/x-www-form-urlencoded` kind, which is the most commonly used
one by HTML forms. See the option `POSTFIELDS` option for how to specify the
data to post.

### CRLF flag

```
curl::option CurlHandle,"CRLF"
```

Convert unix newlines to CRLF newlines on FTP uploads.

### FTPLISTONLY flag

```
curl::option CurlHandle,"FTPLISTONLY"
```

Setting the parameter tells the library to just list the names of an ftp
directory, instead of doing a full directory listin that would include file
sizes, dates etc.

### FTPAPPEND flag

```
curl::option CurlHandle,"FTPAPPEND"
```

Setting the parameter tells the library to append to the remote file instead of
overwrite it. This is only useful when uploading to a ftp site.

### NETRC flag

```
curl::option CurlHandle,"NETRC"
```

The parameter tells the library to scan your
`~/.netrc`
file to find user name and password for the remote site you are about to
access. Do note that curl does not verify that the file has the correct
properties set (as the standard unix ftp client does), and that only machine
name, user name and password is taken into account (init macros and similar
things aren't supported).

### FOLLOWLOCATION flag

```
curl::option CurlHandle,"FOLLOWLOCATION"
```

Setting the parameter tells the library to follow any Location: header that the
server sends as part of a HTTP header. NOTE that this means that the library
will resend the same request on the new location and follow new Location:
headers all the way until no more such headers are returned.

### TRANSFERTEXT flag

```
curl::option CurlHandle,"TRANSFERTEXT"
```

Setting the parameter tells the library to use ASCII mode for ftp transfers,
instead of the default binary transfer. For LDAP transfers it gets the data in
plain text instead of HTML and for win32 systems it does not set the stdout to
binary mode. This option can be useable when transfering text data between
system with different views on certain characters, such as newlines or
similar.

### CLOSEPOLICY

```
curl::option CurlHandle,"CLOSEPOLICY"
```

This option sets what policy libcurl should use when the
connection cache is filled and one of the open connections has to be closed to
make room for a new connection. This must be `OLD` or `FRESH`. If you specify `OLD`
as argument libcurl close the
oldest connection, the one that was created first among the ones in the
connection cache. If you specify `FRESH` libcurl close the
connection that was least recently used, that connection is also least likely
to be capable of re-use.

Example

```
curl::option CURL,"CLOSEPOLICY","OLD"
curl::option CURL,"CLOSEPOLICY","FRESH"
```

Note that the values are checked by the module case sensitive, thus you can not write `"old"`
or `"fresh"` or mixed case words. Also the module **does not tolerate** any other form of the words,
like `OLDEST` or `NEW`.

### PUT flag

```
curl::option CurlHandle,"PUT"
```

Setting the parameter tells the library to use HTTP PUT a file. The file to put
must be set with option `INFILE`.

### SSL_VERIFYPEER flag

```
curl::option CurlHandle,"SSL_VERIFYPEER"
```

Set the flag value to make curl verify the peer's
certificate. The certificate to verify against must be specified with the
option `CAINFO` option.

### FILETIME flag

```
curl::option CurlHandle,"FILETIME"
```

Pass a long. If it is a non-zero value, libcurl will attempt to get the
modification date of the remote document in this operation. This requires that
the remote server sends the time or replies to a time querying command. The
curl_easy_getinfo() function with the CURLINFO_FILETIME argument can be used
after a transfer to extract the received time (if any). (Added in 7.5)

### FRESH_CONNECT flag

```
curl::option CurlHandle,"FRESH_CONNECT"
```

Set the option to make the next transfer use a new connection by
force. If the connection cache is full before this connection, one of the
existinf connections will be closed as according to the set policy. This
option should be used with caution and only if you understand what it
does. Set to 0 to have libcurl attempt re-use of an existing connection.

### FORBID_REUSE flag

```
curl::option CurlHandle,"FORBID_REUSE"
```

Set the option to make the next transfer explicitly close the
connection when done. Normally, libcurl keep all connections alive when done
with one transfer in case there comes a succeeding one that can re-use them.
This option should be used with caution and only if you understand what it
does. Set to 0 to have libcurl keep the connection open for possibly later
re-use.

### HTTPGET flag

```
curl::option CurlHandle,"HTTPGET"
```

Set thisoption to force the HTTP request to get back
to `GET`. Only really usable if `POST`, `PUT` or a custom request have been used
previously using the same curl handle.

### TIMEOUT integer

```
' for example set the timeout to ten minutes
curl::option CurlHandle,"TIMEOUT",600
```

Pass an integer parameter containing the maximum time in seconds that you allow
the libcurl transfer operation to take. Normally, name lookups can take a
considerable time and limiting operations to less than a few minutes risk
aborting perfectly normal operations. This option will cause curl to use the
SIGALRM to enable timeouting system calls. **NOTE**
that this does not work in multi-threaded programs!

### LOW_SPEED_LIMIT integer

```
curl::option CurlHandle,"LOW_SPEED_LIMIT",100
```

Pass an integer as parameter. It contains the transfer speed in bytes per second
that the transfer should be below during `LOW_SPEED_TIME` seconds for
the library to consider it too slow and abort.

### LOW_SPEED_TIME integer

```
curl::option CurlHandle,"LOW_SPEED_TIME",60
```

Pass an integer as parameter. It contains the time in seconds that the transfer
should be below the `LOW_SPEED_LIMIT` for the library to consider it too
slow and abort.

### RESUME_FROM integer

```
curl::option CurlHandle,"RESUME_FROM",3321
```

Pass an integer as parameter. It contains the offset in number of bytes that you
want the transfer to start from.

### SSLVERSION integer

```
curl::option CurlHandle,"SSLVERSION",3
```

Pass an integer as parameter. Set what version of SSL to attempt to use, 2 or
3. By default, the SSL library will try to solve this by itself although some
servers make this difficult why you at times will have to use this option.

### TIMECONDITION string

```
curl::option CurlHandle,"TIMECONDITION","IFMODSINCE"
```

Pass a string as parameter. This defines how the `TIMEVALUE` time value is
treated. You can set this parameter to `IFMODSINCE` or
`IFUNMODSINCE`. This is aa HTTP-only feature.

Example

```
curl::option CURL,"TIMECONDITION","IFMODSINCE"
curl::option CURL,"TIMECONDITION","IFUNMODSINCE"
```

Note that the values are checked by the module case sensitive, thus you can not write `"ifmodsince"`
or `"ifmodsince"` or mixed case words.

### TIMEVALUE integer

```
curl::option CURL,"TIMECONDITION","IFMODSINCE"
curl::option CurlHandle,"TIMEVALUE",curl::getdate("2 days ago")
```

Pass an integer as parameter. This should be the time in seconds since 1 jan 1970,
and the time will be used as specified in the option `TIMECONDITION` or if that
isn't used, it will be `IFMODSINCE` by default. (In other words curl will fetch the
page only if that is newer than the specified time.)

To conveniently get such time-stamp values as accepted by this function as argument you
can use the ScriptBasic function `TIMEVALUE`.

### MAXREDIRS integer

```
curl::option CurlHandle,"MAXREDIRS",3
```

Pass an integer as parameter. The set number will be the redirection limit. If that many
redirections have been followed, the next redirect will cause an error. This
option only makes sense if the option `FOLLOWLOCATION` is used at the same
time.

### MAXCONNECTS integer

```
curl::option CurlHandle,"MAXCONNECTS",10
```

Pass an integer as parameter. The set number will be the persistant connection cache size. The
set amount will be the maximum amount of simultaneous connections that libcurl
may cache between file transfers. Default is 5, and there isn't much point in
changing this value unless you are perfectly aware of how this work and
changes libcurl's behaviour. Note: if you have already performed transfers
with this curl handle, setting a smaller `MAXCONNECTS` than before may cause
open connections to unnecessarily get closed.

### CONNECTTIMEOUT integer

```
curl::option CurlHandle,"CONNECTTIMEOUT",10
```

Pass an integer as parameter. It should contain the maximum time in seconds that you allow the
connection to the server to take.  This only limits the connection phase, once
it has connected, this option is of no more use. Set to zero to disable
connection timeout (it will then only timeout on the system's internal
timeouts). Specifying no value for this option is the same as specifying zero as all integer value options.
See also the `TIMEOUT` option.

**NOTE** that this does not work in multi-threaded programs!

### HTTPHEADER string

```
curl::option CurlHandle,"HTTPHEADER","Accept: image/gif"
```

Use this option to specify a header to be sent to the server. If you add a header that is
otherwise generated and used by libcurl internally, your added one will be used instead.
If you add a header with no contents as in `Accept:`, the internally used header will
just get disabled. Thus, using this option you can add new headers, replace
internal headers and remove internal headers.

You should call the function with this option for each extra header line that you want to add.

### NOHTTPHEADER flag

```
curl::option CurlHandle,"NOHTTPHEADER"
```

Use this option to delete all previous `HTTPHEADER` options. This may be needed when you use a single
CURL connection to download several pages from the same server and did not call `curl::finish` and
`curl::init()` again. Until you call `curl::finish` or specify this option all header lines remain effective.

After setting this option you can set various `HTTPHEADER` option strings to build up a new list.

### QUOTE string

```
curl::option CurlHandle,"QUOTE","pwd"
```

Use this option to set arbitrary FTP commands that will be passed to the server prior to
your ftp request. You can set more than one values for this option. The commands will be
executed in the order they are specified.

### NOQUOTE flag

```
curl::option CurlHandle,"NOQUOTE"
```

Use this option to delete all previously set `QUOTE` FTP command strings. Read the documentation of
the option `HTTPDEADER`.

### POSTQUOTE string

```
curl::option CurlHandle,"POSTQUOTE","site chmod 777 uploadedfile.txt"
```

Use this option to set arbitrary FTP commands that will be passed to the server after
your ftp request. You can set more than one values for this option. The commands will be
executed in the order they are specified.

### NOPOSTQUOTE flag

```
curl::option CurlHandle,"NOPOSTQUOTE"
```

Use this option to delete all previously set `POSTQUOTE` FTP command strings. Read the documentation of
the option `HTTPDEADER`.

### HTTPPOST string

```
curl::option CurlHandle,"HTTPPOST","name=Desade+Marquis"
```

Use this option to specify form data in case of complex HTTP POST operation. As third argument specify a string
as POST data. You can specify many post data calling the `curl::option` function with this option in successive calls.
The string argument should have the following format:

- `name=content`

This can be used to specify textual form data. This is like what the browser sends to the web server when an
<input type=text name="name" value="content"> is used.

- `name=@@filename`

This can be used to specify a file to upload. This is like what the browser sends to the web server when an
<input type=file name="name"> is used.

- `name=@@filename1,filename2,...`

Add a form field named `name` with the contents as read from the local files
named `filename1` and `filename2`. This is identical to the previous, except that
you get the contents of several files in one section.

- `name=@@filename;type=content-type`

Whenever you specify a file to read from, you can optionally specify the
content-type as well. The content-type is passed to the server together with
the contents of the file. The underlying CURL library will guess content-type for a
number of well-known extensions and otherwise it will set it to binary. You
can override the internal decision by using this option.

- `name=@@filename1,filename2,...;type=content-type`

When you specify several files to read the contents from, you can set the
content-type for all of them in the same way as with a single file.

### ERRORBUFFER not implemented

This option is implemented in the original CURL library, but is not implemented in this interface.
The ScriptBasic interface sets this option automatically and you can access the error message calling the
BASIC function `curl::error()`.

### Options not implemented
- `CURLOPT_STDERR` is used in the CURL library to specify an alternate strean instead of `stderr`
for erro output. This is not supported by the ScriptBasic interface.

- `CURLOPT_PROGRESSFUNCTION` is used in conjunction with `CURLOPT_PROGRESSDATA` to specify a progress
function that CURLIB library calls from time to time to allow progress indication for the user. This is
not implemented in the ScriptBasic interface.

- `CURLOPT_PASSWDFUNCTION` is used along with the option `CURLOPT_PASSWDDATA` to specify a function
that returns the password for connections that need password, but no password is specified. This is not
implemented in ScriptBasic.

## curl::perform

Call this function to perform the action that was initialized using the function
`curl::init()` and `curl::option()`

Usage:
```
curl::perform CURL
```

## curl::info

Call this function to get information on the performed action
after `curl::perform` was called

Usage:
```
INFO = curl::info(CURL,info)
```

The argument `info` should be string and can take one of the valueslisted in the following sections. The return
value is either a string, integer or real value according to the type of the requested information. The possible
string arguments should be used in upper case letters, and should be precisely typed, no abbreviations or alternate
forms of the words are recognized by the subroutine.

Note that the following sections were mainly compiled from the documentation of the original C language CURL package
documentation `curl_easy_getinfo.3` man page.

### EFFECTIVE_URL string
The function returns the string of the effective URL of the last transfer.

### HTTP_CODE integer
The function returns the integer status value of the lasttransfer.

### FILETIME integer
The function returns the remote time of the retrieved document. If the result is zero it means that the time
can not be determined for some reson. It may happen that the server hides the time, or does not support the
command that is used to retrieve the file time.

### TOTAL_TIME real
The function returns a real number; the total transaction time in seconds for the previous transfer. The returned
value is real and not integer because the fractional seconds are also taken into account.

### NAMELOOKUP_TIME real
The function returns a real number; the time, in seconds, it took from the start until the name resolving
was completed.

### CONNECT_TIME real
The function returns a real number;
the time, in seconds, it took from the
start until the connect to the remote host (or proxy) was completed.

### PRETRANSFER_TIME real
The function returns a real number;
the time, in seconds, it took from the
start until the file transfer is just about to begin. This includes all
pre-transfer commands and negotiations that are specific to the particular
protocol(s) involved.

### SIZE_UPLOAD real
The function returns a real number;
the total amount of bytes that were
uploaded.

I see no reason why this is a real number but the underlying CURL library returns a `double`.

### SIZE_DOWNLOAD real
The function returns a real number;
total amount of bytes that were
downloaded.

I see no reason why this is a real number but the underlying CURL library returns a `double`.

### SPEED_DOWNLOAD real
The function returns a real number;
the average download speed, in bytes/seconds, that curl
measured for the complete download.

### SPEED_UPLOAD real
The function returns a real number;
the average upload speed, in bytes/seconds, that curl
measured for the complete upload.

### HEADER_SIZE integer
The function returns an integer;
the toal number of bytes received in all headers during the transfer.

### REQUEST_SIZE integer
The function returns an integer;
the total number of bytes of the issued
requests. This is so far only for HTTP requests. Note that this may be more
than one request if `FOLLOWLOCATION` was set using `curl::option`.

### SSL_VERIFYRESULT integer
The function returns an integer;
the result of the certification verification that was requested (using `curl::option` with the option `SSL_VERIFYPEER`).

### CONTENT_LENGTH_DOWNLOAD real
The function returns a real number;
the content-length of the download. This
is the value read from the `Content-Length:` field.

I see no reason why this is a real number but the underlying CURL library returns a `double`.

### CONTENT_LENGTH_UPLOAD real
The function returns a real number;
the specified size of the upload.

## curl::finish

You can call this function to close the connection that was used by CURL and to release
all resources that were allocated to handle the connection. To call this function is not
a must as all the releasing tasks are automatically performed as soon as the interpreter
finishes execution the BASIC program. However it is a good practice to call this functions
especially if you want to access the file the perform routine downloaded.

Usage:
```
curl::finish CURL
```

This function closes the connection to the remote URL, closes the local file (if there was opened any), and
releases the memory that was used for the connection.

## curl::error

If any error has happened this function can be used to retrieve the error message that the underlying
CURL library supplied.

Usage:
```
print curl::error(CURL)
```

## curl::escape

This function will convert the given input string to an URL encoded string and
return that string. All input characters that are not a-z,
A-Z or 0-9 will be converted to their "URL escaped" version.

Usage:
```
URLencoded = curl::escape(URLnonencoded)
```

## curl::unescape

This function will convert the given URL encoded input string to a "plain
string" and return that string. All input characters that are encoded `%XX`
where `XX` is a to digit hexadecimal character will be converted to their
plain text version. All `+` characters that are after the (or a) `?` character
are converted to space.

Usage:
```
URLencoded = curl::escape(URLnonencoded)
```

This behaviour of this function makes it easy to unescape URLs that contain not
only the CGI GET parameter of the URL, but the whole URL. For exaple:

```
print curl::unescape("http://www.kuka+muka.com/mypage.asp?name=Linus+Nielsen")
```

will print

```
http://www.kuka+muka.com/mypage.asp?name=Linus Nielsen
```

which is much better than having the web server name converted to something containing a space.

## curl::getdate

This function returns the number of seconds since January 1st 1970, for the
date and time that the *datestring* parameter specifies.
Read further in the date string parser section below.

If the input string can not be parsed the function return zero.

The following is copied from the original `curl_getdate` manual page.

**PARSING DATES AND TIMES**

A `"date"` is a string, possibly empty, containing many items separated by
whitespace. The whitespace may be omitted when no ambiguity arises.  The
empty string means the beginning of today (i.e., midnight).  Order of the
items is immaterial.  A date string may contain many flavors of items:

- calendar date items
This can be specified in a number of different ways.
Including `1970-09-17`, `70-9-17`, `70-09-17`, `9/17/72`,
`24 September 1972`, `24 Sept 72`, `24 Sep 72`, `Sep 24, 1972`,
`24-sep-72`, `24sep72`.
The year can also be omitted, for example: `9/17` or `sep 17`.

- time of the day items
This string specifies the time on a given day. Syntax supported includes:
`18:19:0`, `18:19`, `6:19pm`, `18:19-0500`
(for specifying the time zone as well).

- time zone items
Specifies international time zone. There are a few acronyms supported, but in
general you should instead use the specific realtive time compared to
UTC. Supported formats include: `-1200`, `MST`, `+0100`.

- day of the week items
Specifies a day of the week. If this is mentioned alone it means that day of
the week in the future.

Days of the week may be spelled out in full: `Sunday`, `Monday`, etc or they
may be abbreviated to their first three letters, optionally followed by a
period.  The special abbreviations `Tues` for `Tuesday`, `Wednes` for
`Wednesday` and `Thur` or `Thurs` for `Thursday` are also allowed.

A number may precede a day of the week item to move forward supplementary
weeks.  It is best used in expression like `third monday`.  In this context,
`last DAY` or `next DAY` is also acceptable; they move one week before or
after the day that DAY by itself would represent.

- relative items
A relative item adjusts a date (or the current date if none) forward or
backward. Example syntax includes: `1 year`, `1 year ago`, `2 days`,
`4 weeks`.

The string `tomorrow` is worth one day in the future (equivalent to `day`),
the string `yesterday` is worth one day in the past (equivalent to `day ago`).

- pure numbers
If the decimal number is of the form `YYYYMMDD` and no other calendar date item
appears before it in the date string, then `YYYY` is read as the year, `MM` as the
month number and `DD` as the day of the month, for the specified calendar date.

**AUTHORS**

Originally written by Steven M. Bellovin `smb@research.att.com` while at the
University of North Carolina at Chapel Hill.  Later tweaked by a couple of
people on Usenet.  Completely overhauled by Rich $alz `rsalz@bbn.com` and Jim
Berets `jberets@bbn.com` in August, 1990.

## curl::version

Returns a human readable string with the version number of
libcurl and some of its important components (like OpenSSL
version) that were used to create the ScriptBasic module
`curl.dll` under Windows NT or `curl.so` under UNIX.

The module links the CURL library static thus you have to
recompile it or download a newer binary of the module if
there is a newer version of the CURL library even if you
have installed a newer version of the library itself.
