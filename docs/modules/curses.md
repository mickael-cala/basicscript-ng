<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `curses`

Source : `src/extensions/curses/interface.c`

## the module CURSES
This module implements a ScriptBasic-curses interface (something like CIO
for Unix) that makes direct cursor control and screen drawing possible on
Unix systems.

Author: Berki Lukacs Tamas

## Installation

The distribution contains the C language source file and the ScriptBasic
include file. The C source must be compiled with the following command:

```
cc -shared -o libcurses.so curses.c -lcurses
```

The curses library and its include files (for Debian users: libncurses5 and
libncurses5-dev packages) must be installed.

Move the library to your ScriptBasic library directory and the include file
to the ScriptBasic include file directory (or redefine include and library
directories in basic.conf).

I hope that this module will be included in the ScriptBasic distribution
RSN.

My original plan was to port the CIO library to Unices, but most of its
functions could not be adapted and curses supports a much wider range
of screen operations than CIO, so this extension became a curses interface.

This means that curses routines are not compatible with CIO, although
it should be very easy to make a subset of CIO routines available under
Unices.

sbkbhit is the equivalent of getch(0, 0).
Console title modification is (of course) not supported, but something
could be done with xterm's title modification escape sequences.

NoBreak is not supported. ScriptBasic programs should be terminated with
Ctrl-C (SIGINT).

SizeX and SizeY are the equivalent of getmaxx and getmaxy.

Color support is VERY different in curses.

Cls is the direct equivalent of erase.

Naturally, the ncurses console cannot be resized like the SetWindow command
does it under Windows.

The only non-straightforward thing about the curses library is its
color support.

Every character on the screen has an associated color pair number. A
color pair consists of a background and a foreground color. There are a
a very finite number of available color pairs, which can be found out
using the maxcolors function.

If you want to print a character in a specified color, you initialize
a color pair to that color, and then make that the active color pair
using setcolor.

You cannot cheat (i.e. print more colors on the screen than the
number of available color pairs) by printing a character and then
changing its color pair's definition, because if a color pair is
changed, its occurrences on the screen are repainted using the new
color. This is quirk in curses (I think for the sake of being
compatible with some kind of old terminals).

This is VERY straightforward. Windows can be created and deleted using
newwin and delwin. Sub-window support is there in ncurses, but I chose
not to support that because the documentation of ncurses says that it
is still buggy.

Console handling low level routines for Unix ncurses environment.

This module implements interface routines to the ncurses library.

## Set current window

```
setwin(WindowHandle)
```

Set current window to the specified one. Returns 0 in case of error, returns
-1 otherwise. The handle of stdscr (full-screen default window) is 1.

## Beep.

```
border
```

Sound the bell of the terminal.

## Create a new window

```
WindowHandle = newwin(X, Y, Width, Height)
```

Creates a new window with specified dimensions and returns its handle.
Returns 0 in case of error.
Window handles are small integers starting from 1.

## Delete a window

```
delwin(WindowHandle)
```

Delete the specified window. Return 0 in case of error.

## Set automatic refresh

```
autorefresh(1)
```

Set automatic refresh flag. If it is true, a screen refresh occurs every
time a curses function is called that can possibly modify the contents
of the screen.

Even if autorefresh is inactive, there are some functions (mainly the
keyboard input functinos) that refresh the screen automatically. This
is a limitation of the curses library.

## Beep.

```
beep
```

Sound the bell of the terminal.

## Flash the screen

```
flash
```

Flash the screen (cause a visible bell). If the terminal does not support
visible bell, the audible bell is sounded.

## Erase the screen

```
erase
```

Copy blanks to every character position on the screen, thus erasing it.

## Refresh the screen

```
refresh
```

Refresh the screen.

## Move the cursor

```
move(X, Y)
```

Move the cursor to the specified coordinates. The coordinates are 0-based.
Return 0 on error, -1 otherwise.

## Get X position of cursor

```
XCoord = getx()
```

Get the X coordinate of cursor position.

## Get Y position of cursor

```
YCoord = gety()
```

Get the Y coordinate of cursor position.

## Get width of current window

```
XSize = getsizex()
```

Returns the width of current window.

## Get height of current window

```
YSize = getsizey()
```

Returns the height of current window.

## Print a string

```
addstr("Hello, World!")
```

Print the specified string in the current window.

## Insert a string

```
insstr "Hello, World!"
```

Insert the specified string under the cursor. Control characters
(tab, backspace, newline) are handled appropriately. It possible
that characters at the end of line are lost.

## Insert or delete lines

```
insdelln(10)
```

If the argument is positive, insert the appropriate number of lines above
the current line. If the argument is negative, delete the appropriate number
of lines starting from the one containing the cursor.

## Delete character

```
```

Delete the character under the cursor.

## Get a string from the current window

```
Content = instr(10)
```

These routines return  a  string  of  characters  in  str,
extracted  starting  at the current cursor position in the
named window.

## Turn on the specified character attributes

```
attron(ABold or AReverse)
```

Turn on the specified attributes for the current window. The argument should
be a bit mask constructed from the A* constants.

## Turn off the specified character attributes

```
attroff(ABold or AReverse)
```

Turn off the specified attributes for the current window. The argument should
be a bit mask constructed from the A* constants.

## Set current character attributes.

```
attrset(ABold or AReverse)
```

Set current character attributes to the specified value. The argument should
be a bit mask constructed from the A* constants.

## Change attributes on screen

```
chgat(5, 1, ABold)
```

Change the attributes of the characters on the screen and their color. In
the examples, the next five characters from the cursor will be changed
to the first color pair and bold face.

## Return number of available color pairs

```
Colors = maxcolors()
```

Returns the number available color pairs on this system.

## Initialize color pair

```
initpair(Color, Foreground, Background)
```

Set a color pair (foreground and background).
Returns 0 on error and -1 otherwise.

## Set current color

```
setcolor(ColorPair)
```

Set current color to the specified color pair. Returns 0 on error and
-1 otherwise.

## Set background color and attributes

```
setbackground(ColorPair, Attributes, Update)
```

Set background color and attributes for the current window. The attribute
is logically OR'ed with all non-blank characters that are written on the
window. If Update is true, change all characters on the screen to the
new background.

## Get character

```
Char = getch(Echo, Delay)
```

Waits for a keypress and returns its curses key code. If Echo is true,
echo the character on the screen. If Delay is false, return -1 if
there is no available keystroke (and do not wait for one). Both
arguments are optional.

## Get string

```
Str = getstr(Length)
```

Get string input from user. Length specifies the maximum possible length
for the string. Line-editing characters are handled appropriately.

## Return name of a key

```
Name = keyname(Key)
```

Return the name of a keystroke (probably returned by getch) as a string.
