<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `mt`

Source : `src/extensions/mt/interface.c`

## The module MT
Support for multi-thread implementation web applications.

This module delivers functions that can be used by ScriptBasic
programs executed by a multi-thread basic interpreter. This means
that the programs are executed in the same process one after the
other or in synchron. The typical example is the Eszter SB Engine
that is a standalone webserver execution BASIC programs to answer
http questions. Those programs may ask this module to store
session variables, wait for eachother.

## What are session and variables
Some description on what is a session and what are session and MT variables.

Formally a session is the series of consecutive execution of ScriptBasic scripts
that belong to the same calculation. Typically this is the series of web script
executions that serve the same client.

When a client meets the web server the first time the program creates a new session and
when the user goes on with the web application the same session information is used. The
session ID is usually sent to the client in HTTP cookie and when the browser sends
the cookie back the server application remembers that this client is the same as the one
before. What the application really remembers is that state of the transaction the user
performed in the session. The tipical example is the consumer basket in an eShop. The user
behind the browser puts several things into the basket while shopping and the application
has to remember what is in the basket.

Conventional CGI applications usually store the session information in plain text files
or in database. That is usually a slow solution unless you need to store these information
for a longer period. But session state is usually not stored for long time.

When a browser first comes to the site a new session is created calling `NewSessionId`. Later
when the user gets to the next page the session id is sent from the browser and the application
checks that the session really exists calling `CheckSessionId` and if it exists it tells the
MT extension module calling `SetSessionID` that the actual session served by the currently
running interpreter thread is this.

To store a value that belongs to that session the application can call `SetSessionVariable` and
to retrieve the value call `GetSessionVariable`. Session variables are kept in memory and can
be reached extremely fast. They are available until the session is deleted calling `DeleteSession`.

There can be not two interpreter threads running concurrently accessing the same session data.
Each session has its own variable set and in case a session has a variable named "BASKET" this
has nothing to do with the other sessions variable of the same name.

MT variables on the other hand are shared by all sessions. An MT variable has a name, and in case
a session modifies the MT variable named "BASKET" the other session looking at the MT variable "BASKET"
will see the changed value. MT variables are global among the sessions.

When accessing an MT variable the variable can not be accesses by other interpreter threads. Thus
there is no chance that an MT variable gets up messed. What is more a program may decide to lock an
MT variable preventing other programs to alter it or even to read its value until the variable
is unlocked. For this the program should call `LockWrite`, `LockRead` and to unlock `UnlockWrite`,
`UnlockRead`

## mt::SetSessionId "session id"

Set the session for the script. This function can and should be used to specify the session that the
program actions belong to. Before calling this function and specifying the session the program can not
access the session variables.

The session ID is usually given by the client program by some means. Typically web browser cookies hold
the session identifier.

## mt::CheckSessionId("sessionid")

Checks that a session already exists. The function returns `TRUE`
if the session id already exists.

This function can and should be used to check that a session is valid or not based
on its existence. If the session is not existent the program can still call the
function `SetSessionId` to set the session and the new session is autmatically created.

However calling this function is a good chanhe to check if any session initialization is
needed, for example password check.

## mt::SessionCount()

This function returns the number of the active sessions.

## mt::NewSessionId(["optional random string"])

This function creates a new session and returns the ID of the newly created
session and returns the id of the session.

If no argument is provided the function just count the session and
creates a 32 bytes session id using MD5.

If there is an optional argument that is also used to create the
MD5 session id. That may provide some more randimity from the
application level.

## mt::DeleteSession ["sessionid"]

Call this function to delete a session. The function deletes the session and releases
all memory that was allocated to store session variables.

If no argumentum is supplied then the function deletes the actual session.

## mt::GetSessionId()

This function returns the actual session id. This is usually known by
the program because this is the id that was set calling `SetSessionId` or
was created and returned by `NewSessionId`. However some programs may need
to call this function.

## mt::SetSessionVariabe "variablename",value

Call this function to set the value of a session variable. The first
argument has to be a string that names the session variable. The second
argument is the new value of the session variable.

## mt::GetSessionVariabe("variablename")

Get the value of a session variable. The argument has to be a string that
identifies the session variable.

The value of a session variable can be set using `SetSessionVariable`.

## mt::SetVariable "variablename",value

Set the value of an MT variable. The first argument has to be a string that
identifies the variable. The second argument is the new value of the
variable.

MT variables are global, all running scripts like the same values.

## mt::GetVariable("variablename")

Get Mt variable. This function gets the value of an Mt variable.

## mt::LockWrite "variablename"

Lock mt variable for write protection. While the variable is locked no programs can access it, not even
the one that locked the variable.

Usually there is no need to alter the MT variables, as the variables are automatically locked while
their value is altered. Locking is needed when a program needs to alter more than one MT variable in
a coincise manner. Another possibility is when a program wants to alter a variable and the new value depends
on the previous value. As a simplest example is when aprogram wants to increment a variable.

In either case the programs have to have a pseudo MT variable, which is used in the locking and
unlocking functions. Usually the programs use these variables only to lock and to release
and do not care their values, albeit there is nothing that would prohibit using the values.

For example a program (X) wants to increment the MT variable "B" and it has to keep track of the
variable "BB" to ensure that "BB" is the double of "B".

In the meantime another (Y) program wants to decrement "BB" by two and set "B" to the half of "BB" to
keep the rule.

Program X:
```
mt::GetVariable "B",B
B = B+1
mt::SetVariable "B",B
mt::SetVariable "BB",2*B
```

Program Y:
```
mt::GetVariable "BB",BB
BB = BB-2
mt::SetVariable "BB",BB
mt::SetVariable "B",B \ 2
```

The lines of the two programs can execute in any mixed order.

See the following scenario:
- Program X examines B and sees that B is 3 (for example)
- Program Y examines BB and sees that BB is 6.
- Program X increments B to be 4.
- Program Y decrements BB to be 4.
- Program Y sets B to be the half of BB to 2.
- Program X sets the variable BB to the double of B, which is 8 for that program.

Instead a variable called "BLCK" has to be used (BLCK is just for example):

Program X:
```
mt::LockWrite "BLCK"
mt::GetVariable "B",B
B = B+1
mt::SetVariable "B",B
mt::SetVariable "BB",2*B
mt::UnlockWrite "BLCK"
```

Program Y:
```
mt::LockWrite "BLCK"
mt::GetVariable "BB",BB
BB = BB-2
mt::SetVariable "BB",BB
mt::SetVariable "B",B \ 2
mt::UnlockWrite "BLCK"
```

The locking at the start of the critical code segments prevents two programs to enter the critical part at the
same time.

## mt::LockRead "variablename"

Lock mt variable for read protection. Several concurrent programs can read lock a variable,
but so long as long there are read locks no program can write lock a variable.

The function will wait until it can lock the variable. A variable can not be read locked if
the variable is write locked or there is a write lock waiting for the variable.

## mt::UnlockWrite "variablename"

Unlock mt variable from write protection.

## mt::UnlockRead "variablename"
Unlock mt variable from read protection

## mt::SessionTimeout [sec]

Call this function to set session timeout (20 minutes in the example)
```
mt::SessionTimeout 20*60
```
or
```
mt::SessionTimeout
```
cancelling session timeout.

## mt::GetSessionTimeout ["sessionid"]

This function returns the time when the session times out. This value is
expressed as GMT seconds since January 1, 1970. 0:00.

If the argument is missing the actual argument is used.

## mt::GetSessionPingTime ["sessionid"]

This function returns the time when the session was last used (assigned to a script).
This value is expressed as GMT seconds since January 1, 1970. 0:00.

If the argument is missing the actual argument is used.

## mt::ListSessions

This subroutine collects the session ids.

```
  call mt::ListSessions Array,Sm,SM,Pm,PM,Tm,TM
```

The first argument to the subroutine should be a variable. This variable will loose its previous
value and gets a new array value containing the session strings.

If there is no more arguments to the subroutine call this array will contain all the
existing sessions in the MT module. However the programmer may define six optional
parameters that select only certain sessions that have time value parameters that
are between the specified values. These are

- `Sm` required minimal start time of the session
- `SM` required maximal start time of the session
- `Pm` required minimal ping time of the session
- `PM` required maximal ping time of the session
- `Tm` required minimal time-out time of the session
- `TM` required maximal time-out time of the session

These time values have to be expressed as GTM time stamps. If a value is zero or `undef`
the value in the constraint is ignored. Unspecified arguments are `undef` by ScriptBasic behaviour.

For example to get the list of all sessions that are older than an hour

```
call mt::ListSession Array,undef,GmTime()-60*60
```

To get all session IDs that timed out:

```
call mt::ListSession Array,undef,undef,undef,undef,undef,GmTime()
```

To get all sessions that are idle for more than ten minutes

```
call mt::ListSession Array,undef,undef,undef,GmTime()-600
```

Note that this subroutine is rarely used in "cgi" scripts. This
subroutine has to be used in scripts that are run by the Eszter
SB Engine asynchronously started using the configuration
key `httpd.run.[re]start`. Those scripts start before the
engine starts to listen on a port and run so long as long
the engine runs and are able to scan the sessions from time to
time and delete the old sessions freeing the memoryof the module
MT.
