<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `win`

Source : `src/extensions/win/interface.c`

## The module win
Windows specific functions

This module implements some Win32 system calls that are not implemented in
the core of ScriptBasic but can be helpful for those who want to write system
maintenance scripts using ScriptBasic.

The reason that these functions are not implemented inside ScriptBasic is
that ScriptBasic itself is portable, and whenever a programmer writes
a program in pure ScriptBasic it should execute the same way under UNIX
as well as under Windows NT/Win98/W2K.

Programs using the module win however are Win32 specific and will not run
unaltered under UNIX.

## Handling the registry

These functions handle the registry.
The argument that specifies a registry element should start with the
name of the top level registry hives. These are:

- `HKEY_CLASSES_ROOT` or `HKCR`
- `HKEY_CURRENT_CONFIG` or `HKCC`
- `HKEY_CURRENT_USER` or `HKCU`
- `HKEY_LOCAL_MACHINE` or `HKLM`
- `HKEY_USERS` or `HKU`
- `HKEY_PERFORMANCE_DATA` or `HKPD`
The registry names in the documentation usually refer to the ScriptBasic configuration file location regis-try element. This is only an example, but you should not alter is from a program without good reason. Generally saying: be reasonable and think twice before altering the registry from a program.

## Read the registry

```
win::RegRead("HKLM\\Software\\ScriptBasic\\config")
```

This function reads a registry entry and returns the value of the entry or
undef if the entry does not ex-ists or if it is not readable by the
interpreter process/thread.

If the argument string specifies a key and the last character of the argument
is \ then the default value for the key is retrieved.  Note that
HKLM\Software\ScriptBasic can have a subkey named config as well
as a value named config. The string
`"HKLM\\Software\\ScriptBasic\\config"` refers to the value while
`"HKLM\\Software\\ScriptBasic\\config\\"` refers to the default value of
the subkey.

## Delete a registry value or key

```
win::RegDel "HKLM\\Software\\ScriptBasic\\config"
```

This function deletes a value from the registry or deletes a key from the registry.
If the argument specifies a registry key it should not have any subkeys. If the registry
value or key cannot be deleted for some reason the function raises error that
the programmer should capture using the `ON ERROR GOTO` statement.

## Write a registry value

```
win::RegWrite "HKLM\\Software\\ScriptBasic\\config",value
```

This function writes the value of a registry value. The key should already exist.
If the key is non-existent or the value is not alterable by the program the function
raises error that the programmer should capture using the `ON ERROR GOTO` statement.

## Display a message box

```
i = win::MsgBox("Text","Caption" [,"buttons","style",defbutton])
```

This function displays a Windows message box on the screen and waits for the user to press
one of the buttons of the message box. The function should get two mandatory and three
optional arguments.

The arguments:

- `"Text"` the first argument to the function is the text that appears on the text box.
- `"Caption"` the second argument to the function is the caption of the message
      box. This text appears in the top of the box.

- `"buttons"` This optional string specifies what buttons to diplay on the message box. If this
      argument is missing a single OK button will appear on the box. The valid strings for this argument
      are
- `"ARI"` Abort/Retry/Ignore
- `"OK"` OK
- `"O"` OK
- `"OC"` OK/Cancel
- `"RC"` Retry/Cancel
- `"YN"` Yes/No
- `"YNC"` Yes/No/Cancel
  You can also write the string in a long form for better readability:
- `"AbortRetryIgnore"`
- `"Ok"`
- `"OkCancel"`
- `"RetryCancel"`
- `"YesNo"`
- `"YesNoCancel"`
- `"style"` this optional argument specifies what type of icon should appear on the message box
      left to the text. There are four icons available:
- `"Exclamation"`
- `"Ex"`
- `"Warning"`
- `"W"` An exclamation-point icon appears in the message box.
- `"Information"`
- `"Info"`
- `"I"`
- `"Asterix"`
- `"A"`
- `"*"` An icon consisting of a lowercase letter i in a circle appears in the message box.
- `"Question"`
- `"Q"`
- `"?"` A question-mark icon appears in the message box.
- `"Stop"`
- `"S"`
- `"Error"`
- `"E"`
- `"Hand"`
- `"H"` A stop-sign icon appears in the message box.
  If this argument is missing or `undef` no icon appears left to the text.
- `defbutton` should be an integer value between 1 to 4, or at most the number of the
      buttons on the actual message box. This argument specifies , which button is the default
      button on the form. The default button is selected as default and in case the user pressed
      the Enter key instead of clickong on one of the buttons the default button is pressed.

The **return value** of the function is a one character string containing one of the characters
`A, C, I, N, O, R, Y` for the pressed buttons respectively: Abort, Cancel, Ignore, No, OK, Retry, Yes.

The parameters `"buttons"` and `"style"` are not case sensitive. The return value is uppercase
character.

The following example tests all the possible message boxes:

```
import win.bas

win::MsgBox """This program test the various text boxes.

In the coming message boxes press the various keys as requested by the text
and check on the console window that the program reported the correct
button you pressed. (Only initials are printed.)
""","Initial MsgBox"

splita " |ARI|O|RC|YN|YNC" BY "|" TO buttons
splita " |W|I|Q|S" BY "|" TO marks

for j=lbound(marks) to ubound(marks)

for i=lbound(buttons) to ubound(buttons)

for d=1 to len(buttons[i])
print i," ",buttons[i]," ",j," ",marks[j]," ",d,"\n"
R = win::MsgBox(buttons[i],marks[j],buttons[i],marks[j],d)
print R
print

next d
next i
next j
```

## ShutDown a Windows NT server or workstation

Calling this function shuts down a Windows NT Server, Workstation or
W2K machine. It was tested on a W2K Professional.

```
win::ShutDown "machine","message",timeout,force,reboot
```

The first argument is the name of the machine to reboot. If this is `undef`
then the actual machine running is shutting down.

The second argument is the message that the system will display on the screen
before starting the actual shutdown. This argument may also be `undef` in case
there is no need for such a message.

The argument `timeout` is the number of seconds that the operating system will wait
before starting the actual shutdown.

`force` specifies whether applications with unsaved changes are to be forcibly closed.
If this parameter is TRUE, such applications are closed. If this parameter is FALSE,
a dialog box is displayed prompting the user to close the applications.

`reboot` specifies whether the computer is to restart immediately after shutting
down. If this parameter is TRUE, the computer is to restart. If this parameter is FALSE,
the system flushes all caches to disk, clears the screen, and displays a message indicating
that it is safe to power down or switches off automatically.

The function calls the Windows NT `InitiateSystemShutdown` function. Some sentences of this
ScriptBasic documentation was copied from the original documentation of the Windows NT system
API documentation.

## Get the list of the running processes

This procedure gathers the information on the actually executiong processes and returns
the information in an array.

```
win::ListProcesses PS
```

The result is stored in the variable `PS`. This will be an array, with each element being also an
array holding information for the process. The number of running processes can be determined
using `ubound(PS)+1`.

The elements of the array is indexed from 0 to 8 contaiing the following elements:

- 0 Number of references to the process. A process exists as long as its usage count is nonzero. As soon as its usage count becomes zero, a process terminates.
- 1 Identifier of the process. The contents of this member can be used by Win32 API elements.
- 2 Identifier of the default heap for the process. The contents of this member has meaning only to the tool help functions. It is not a handle, nor is it usable by Win32 API elements.
- 3 Module identifier of the process. The contents of this member has meaning only to the tool help functions. It is not a handle, nor is it usable by Win32 API elements.
- 4 Number of execution threads started by the process.
- 5 Identifier of the process that created the process being examined. The contents of this member can be used by Win32 API elements.
- 6 Base priority of any threads created by this process.
- 7 Reserved; do not use.
- 8 Path and filename of the executable file for the process.

## Start a Windows NT service

This function starts an installed Windows NT service.

```
win::StartService "Service name"
```

The argument for this subroutine call is the name of the service to start. This is the
name that was used to create the service and you can read this name when you start the
graphical service control manager application.

The subroutine returns `undef` value. If the service can not be started the function
raises error. Calling this subroutine for an already running service is not an error.

## Stop a Windows NT service

This function stops an already running Windows NT service.

```
win::StopService "Service name"
```

The argument for this subroutine call is the name of the service to start. This is the
name that was used to create the service and you can read this name when you start the
graphical service control manager application.

The subroutine returns `undef` value. If the service can not be stopped the function
raises error.

## Pause a Windows NT service

This function pauses an already running Windows NT service.

```
win::PauseService "Service name"
```

The argument for this subroutine call is the name of the service to start. This is the
name that was used to create the service and you can read this name when you start the
graphical service control manager application.

The subroutine returns `undef` value. If the service can not be paused the function
raises error.

## Continue a paused Windows NT service

This function asks a Windows NT service to continue after it was paused

```
win::ContinueService "Service name"
```

The argument for this subroutine call is the name of the service to start. This is the
name that was used to create the service and you can read this name when you start the
graphical service control manager application.

The subroutine returns `undef` value. If the service can not be continued the function
raises error.

## Create a hard link on a file or directory

This function function establishes an NTFS hard link between an existing file/directory
and a new file/directory. An NTFS hard link is similar to a POSIX hard link.

```
win::HardLink "existing file","new filename"
```

Any directory entry for a file, is a hard link to the associated file. Additional hard links,
created with the HardLink function, allow you to have multiple directory entries for a file,
that is, multiple hard links to the same file. These may be different names in
the same directory, or they may be the same (or different) names in different directories.
However, all hard links to a file must be on the same volume.

Because hard links are just directory entries for a file, whenever an application modifies
a file through any hard link, all applications using any other hard link
to the file see the changes. Also, all of the directory entries are updated if the file changes.
For example, if the file's size changes, all of the hard links to the file will show the new size.

When creating hard link to a directory the situation is a bit different. In these cases the original
directory entry and the created links are not equivalent. If you delete the link then the original
directory and its content remains untouched. However if you delete the directory any link to that
directory becomes unusable.
