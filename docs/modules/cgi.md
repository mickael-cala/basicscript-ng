<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `cgi`

Source : `src/extensions/cgi/interface.c`

## Handle CGI input and output

Functions that handle web input data for the standalone module running as CGI or
ISAPI or Eszter SB Engine variation.

The module CGI is implemented as an external module written in the language C. Whenever the
programmer wants to write a program using the language ScriptBasic that uses the CGI interface of the web
server he or she can use this module. Although this is not a must to use this module for CGI programming
it IS a wise choice.

The module comes from the developers of the language interpreter. Most of its code is implemented as C
code, executing fast and efficient. In its very first version of the module it already supports such
advanced features as file upload in a robust and secure way. It handles cookies, POST and GET operations
and will support other interfaces in the future like Apache Module interface or FastCGI in a compatible
manner.

The basic programs using the ISAPI interpreter should use the module the same way as CGI versions. In
other words if you write a CGI program it should work executed by the ISAPI version of the interpreter
without modification.
/**
## Acknowledgements

The core code of the CGI handling routines was not written just looking at the RFC. I have learned a lot
from other works. The most important to mention is the CGI.pm Perl module written by Lincoln D. Stein
(http://www-genome.wi.mit.edu/ftp/pub/software/WWW/cgi_docs.html) Another work was named
cgi-lib.c created by eekim@eekim.com. This piece of code gave me the last push to finish looking for
readily available free library to interface with ScriptBasic and write instead a new one.

The ISAPI interface was developed using the Microsoft Developer Network CD documentation and I really
learned a lot while testing the different uses of the function ReadClient until I could figure out that the
official Microsoft documentation is wrong.

It says: "If more than lpdwSize bytes are immediately available to be read, ReadClient will block until some amount
of data has been returned, and will return with less than the requested amount of data." In fact it has to be "If less
than lpdwSize bytes are immediately available to be read, ReadClient will block until some amount of data has been
returned, and will return with possibly less than the requested amount of data."

## Error Codes

## cgi::GetParam("name")

This function returns the value of the GET parameter named in the argument. If there are multiple GET
parameter with the same name the function returns the first one.

The CGI parameter names are case sensitive according to the CGI standard.

## cgi::PostParam("name")

This function returns the value of the POST parameter named in the argument. If there are multiple
POST parameter with the same name the function returns the first one.

The CGI parameter names are case sensitive according to the CGI standard.

## cgi::GetParamEx("name",q) cgi::PostParamEx("name",q)

```
q = undef
cgi::GetParamEx("param",q)
cgi::GetParamEx("param",q)

q = undef
cgi::PostParamEx("param",q)
cgi::PostParamEx("param",q)
```

These functions can be used to iteratively fetch all parameter values passed with the same name. While
the functions Param, GetParam and PostParam return the value of the first parameter of the name these
functions can be used to retrieve the second, third and so on parameters.

The first parameter is the name of the CGI variable.

The second argument is an iteration variable that the function uses internally. This argument is passed
by value and therefore it should be a variable to reach proper functioning. This variable should be undef
when first calling the function. Later the value of this variable is set to a string that represents
an internal value that the basic code SHOULD NOT alter. The value can be moved from one variable to another,
but should not be changed.

The function returns undef when there are no more CGI variables of the name and the iteration variable
is also set to hold the undef value.

The CGI parameter names are case sensitive according to the CGI standard.

## cgi::SaveFile("param","filename")
This function can be used to save an uploaded file into a file on disk. The first argument is the name of
the CGI parameter. The second argument is the desired file name. This file will be created and the content
of the uploaded data is written into it. If the file already existed it is going to be deleted.

Note that the first argument is the name of the CGI parameter and not the name of the file. This is the
string that appears in the tag name in the input field of type file. In the following example the argument
should be `"FILE-UPLOAD-NAME"`.

```
<FORM METHOD="POST" ACTION="echo.bas" ENCTYPE="multipart/form-data">
<INPUT TYPE="FILE" VALUE="FILE-UPLOAD-VALUE" NAME="FILE-UPLOAD-NAME">
<INPUT TYPE="SUBMIT" NAME="SUBMIT-BUTTON" VALUE="UPLOAD FILE">
</FORM>
```

Files are uploaded in binary format. This means that applications accepting text file uploads should take
care of the conversion. The current version of the CGI module does not support the conversion process.

The CGI parameter names are case sensitive according to the CGI standard.

## cgi::FileName("name")

This function returns the name of the uploaded file. This is the file name that the file has on the client
computer. Based on the client computer operating system and used browser this value may contain
spaces, may contain backslash as path separator character, may but need not contain the full path of the
file or even may be OpenVMS format file name specification. Applications using this function should be
prepared to handle the various client file formats.

## cgi::FileLength("param")

This function returns the length of the uploaded file. This is zero if the file field was not filled by the user
or if a zero length file was uploaded. Files are uploaded in binary format. The length is the length of the
binary data that may be more or less of the size of the final file if it is a text file converted to the natural
format of the operating system running the application.

## Environment functions
CGI programs gain a great wealth of information from environment variables. This data is available to
the ScriptBasic program via module functions. The CGI program is encouraged to use these functions
instead of the function environ(). The reason to use these functions is that later versions of the CGI
module may support ISAPI, NSAPI, FastCGI and other web server interface modes. Calling the function
environ() to get the value of these variables will not work in that case, while calling the functions
provided by the CGI module still works.

The value returned by any of these functions is string even when the value is numeric by its nature. This
is usually not an issue, because ScriptBasic automatically converts the values from numeric to string and
back.

## ServerSoftware
This function returns the string identifying the web server software. This is "Microsoft-IIS/4.0" under
Windows NT using the Microsoft web server. The ScriptBasic program usually does not need this function
only if there is some special feature that the web server software provides.

## cgi::ServerName

The internet name of the server machine running the web server.

## cgi::GatewayInterface
This is the name and version of the interface between the program and the web server. This is usually
"CGI/1.1"

## cgi::ServerProtocol
This is the protocol name and version that the server uses. This is usually "HTTP/1.1", but old servers
may still use HTTP/1.0.

## cgi::ServerPort
The internet port that the web server is listening. This value is numeric by its nature, but the functions
returns the string representation of the port number as decimal value. There can be more than one web
servers running on a machine all listening on different ip numbers and ports. The returned value is the
port number of the web server that started the CGI application.

## cgi::RequestMethod
This is the HTTP method the request uses. This can be `"GET"`, `"HEAD"` or `"POST"`. There are other methods that
web servers may support but there is no clear definition how these may interact with CGI programs.

Note that most CGI programs are not prepared to handle HEAD requests. Therefore this method is NOT allowed by default.
The allowed methods are `GET`, `POST` and the special type of `POST` that uploads one or more files. If
your program is prepared to handle `HEAD` requests then you can explicitely allow this method

```
option cgi$Method cgi::Get or cgi::Upload or cgi::Head
```

setting the appropriate value of the run-time option `cgi$Method`.

## cgi::PathInfo
This is the value CGI variable PATH_INFO. The web servers often use this variable in many different
ways. PATH_INFO and PATH_TRANSLATE variables may appear to work incorrectly in Internet
Information Server (IIS) version 4.0. These CGI environment variables return the physical path to the file that
was passed to the CGI application as part of the GET statement. Instead, IIS returns the path to the CGI
script.

## cgi::PathTranslated
This is the value of the CGI variable PATH_TRANSLATED. This is supposed to be the absolute path to the
application.

## cgi::ScriptName
This is value of the CGI variable SCRIPT_NAME. This is usually the name of the script. This may contain
full or partial path elements before the script name.

## cgi::QueryString
This is the query string of a GET request. This string usually appears after the ? mark on the URL and it
is automatically created when the request method is specified to be "GET" in a form. ScriptBasic
programs rarely need this value, as there are other more flexible and more powerful methods to handle CGI
input values.

## cgi::RemoteHost
This is the internet name of the remote client. If the name of the client can not be determined this
variable usually holds the ip number of the client. To have an ip name in this variable depends on the client
and also on the configuration of the web server. The http request does not include the name of the
client. It only holds the ip number, and the web server should issue a reverse lookup request toward the
DNS server to determine the ip name of the client. This is used sometimes for security reasons
disallowing all clients that have no ip name. On the other hand this is a slow process and may severely impact
the performance of the web server. Usually this reverse lookup is switched off.

## cgi::RemoteAddress
This is the ip address of the client machine that issued the http request. This can be the ip number of the
client or the ip number of the proxy server that the client uses.

## cgi::AuthType
This is the type of the authentication the web server performed when the CGI program was started. It is
undef if there was no user authentication. It is "Basic" if the web server used the basic authentication
and is "NTLM" if the authentication is Windows NT challenge response authentication. Other web servers
may use different authentication schemas and this variable may have other values.

## cgi::RemoteUser
This is the value of the CGI variable REMOTE_USER. This variable usually holds the user name supplied
during authentication. Note that this name may have the format DOMAIN\USER under Windows NT.

## cgi::RemoteIdent
This is the value of the CGI variable REMOTE_IDENT. This is rarely implemented.

## cgi::ContentType
This is the value of the CGI variable CONTENT_TYPE. This is undef in case of a GET request. Otherwise
this is the MIME type of the http request body. This is application/x-www-form-urlencoded for normal
form posting and is multipart/form-data in case of file upload.

## cgi::ContentLength
This is the value of the CGI variable CONTENT_LENGTH. This gives the length of the http request data
sent to the web server in the POST request.

## cgi::UserAgent
This is the value of the CGI variable USER_AGENT. The web browsers send their identification strings to
the server in the http request. This browser identification string can be retrieved using this function. This
string is "Mozilla/4.0 (compatible; MSIE 5.0; Windows NT)" for an Internet Explorer 5.0 under
Windows NT.

## cgi::RawCookie
The cookies that the client has sent in the http request. This string contains the cookies without any
processing in raw format. ScriptBasic programs usually do not need this function because there are more
powerful functions to handle cookies.

## cgi::Referer
The browser usually sends the URL of the referring page in a http header field to the server. Using this
function the CGI application can access this URL. Note that according to the http spelling Referer is
spelled with simple r and not Referrer.

This function was missing prior to version 3.0 of the module.

## cgi::Header

```
cgi::Header 200,"text/html"
```

This function is implemented in basic and the source of the function can be found in the file
`cgi.bas`. This function accepts two arguments. The first argument is the code of the state
of the http answer that the application is sending. This is usually `200` for normal answers.
The second argument is the mime type of the answer. This is `text/html` for HTML pages.

The state of the http answer should be defined differently using Microsoft IIS. This function
automatically takes care of this.

## cgi::SetCookie

```
cgi::SetCookie CookieName,CookieValue,CookieDomain,CookiePath,CookieExpires,CookieIsSecure
```

This function should be used to set a cookie. The arguments are the name and value of the cookie. These
arguments are needed. Other arguments may be missing or explicitly hold the undef value.

## cgi::FinishHeader

This function should be called after the last http answer header creating function. In the current
implementation this function only prints an empty line that separates the header from the body, but later
versions may do more actions.

Note that this function is implemented as BASIC code in the file `cgi.bas`, therefore you can easily read and
understand how it works without the need for reading and understanding any C code.

## Cookie("myCookie")

This function returns the value of the cookie named as the argument. This function handles the cookies
that the browser has sent in the http request and not the cookies that the application sends to the client.

## Template handling

CGI programs should output HTML text. Embedding this text into the code is a bad practice. In our
practice we usually use HTML templates that the program reads, modifies, inserts values and outputs. The
CGI module support the usage of such templates.

A template is an HTML text with parameters. The parameters are placed in HTML comments, therefore
you can easily edit these templates with the HTML editor you like. Each comment may contain a symbol
name. The program should specify the actual value for the symbol and the module reads the template
with the actual values replacing the comments. For example the template:

This is a template text with a
```
<!--alma-->
```
symbol.

will be presented as
```
This is a template text with a defined symbol.
```
assuming that the actual value of the symbol alma is the string "defined". If the value of the symbol is
not defined by the program the comment is replaced by an empty string.

To handle symbols, and templates there are several functions in ScriptBasic. You can define a symbol
calling the function cgi::SymbolName. To define the symbol alma you have to write:

```
cgi::SymbolName "alma" , "defined"
```

You can also tell the module that the actual string of the symbol can be found in a file, saying:

```
cgi::SymbolFile "symbol","file name"
```

To get the template file already with resolved symbol values you should say:

```
HtmlTemplate$ = cgi::GetHtmlTemplate("filename")
```

or if you want to hard wire the template text into the code:

```
HtmlTemplate$ = cgi::ResolveHtml("template text to be resolved")
```

When you are finished sending a resolved template to the client you may want to define other symbols,
but before doing that it is safe to undefine the symbols used by the previous template. You can do that
calling the function

```
cgi::ResetSymbols
```

Calling this function also releases the space occupied by the symbols and their values.
For more information see the sample code.

Note that modern approach to this issue is to generate XML format output from the program and use XSLT
transformation to create the desired XHTML output.

## Options that alter the behavour of the module CGI

Options can be set using the ScriptBasic command option. Each option has a 32bit value. The options
should be set before calling the very first function of this module.

These options are:

**Name:** `cgi$BufferIncrease`

**Default value:** 1024 byte

The size of the internal buffer that is used to hold the
header lines of a multipart upload. This is the initial size
of the buffer and this is the size of the automatic
increase whenever the buffer needs size increase.

**Name:** `cgi$BufferMax`

**Default value:** 10240 byte

This is the maximal size of the buffer. If the buffer gets
larger than this size the cgi handling process fails. This
helps you stop attacks that send endless header fields in
a multipart upload.

**Name:** `cgi$ContentMax`

**Default value:** 10Mbyte

The maximal size of the http request.

**Name:** `cgi$FileMax`

**Default value:** 10Mbyte

The maximal size of an uploaded file. If there are more
than one file files uploaded in a single http request their
total size should also be less than the configuration parameter cgi$ContentMax.

**Name:** `cgi$Method`

**Default value:** GET, POST, UPLOAD

The allowed configuration methods. To set the value of
this option you can use the constants defined in the file
cgi.bas. These are:
```
const None   = &H00000000
const Get    = &H00000001
const Post   = &H00000002
const Upload = &H00000006
const Put    = &H00000008
const Del    = &H00000010
const Copy   = &H00000020
const Move   = &H00000040
const Head   = &H00000080
```

If a client starts your CGI program with a method that is not allowed the
cgi modul will not handle that and stop with error.

You can allow more than one methods for the program. In that case the
different options should be given in an expression with biwise `OR`
connected. For example, if you want to allow `GET` and `POST`
operation handled by your CGI program, but do not want to allow upload
you can use the following code segment:

```
import cgi.bas
option cgi$Method cgi::Get or cgi::Post
```

When you want to allow upload you can write

```
import cgi.bas
option cgi$Method cgi::Upload
```

because `cgi::Upload` is a special kind of POST operation and therefore
the bits controlling the methods permissions are set according to this.
