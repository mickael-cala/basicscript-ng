<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `cio`

Source : `src/extensions/cio/interface.c`

Console handling low level routines for Win32 environment.

This module implements routines that can handle the Windows character
console on low level. This is like getting a character without echo, or
checking that a key was pressed without stopping the program, disabling
control-c and so on.

## cio::GotoXY(x,y)

Position the console cursor at the coordinate (X,Y). The
next PRINT statement sending characters to the screen will
print to this position.

## cio::KbHit()

Returns `TRUE` if there is any keyboard pressed and waiting in the keyboard buffer and
returns `FALSE` if there is no key in the keyboard buffer.

## cio::GetCh()

Get a single character from the keyboard buffer without echo. If there
is no key pressed in the keyboard buffer then the function waits for the
user to press a key.

## cio::GetChE()

Get a single character from the keyboard buffer with echo. If there
is no key pressed in the keyboard buffer then the function waits for the
user to press a key.

## cio::Detach()

Close the console window. There is no way to open a new console after it has been closed.
If the program was started from the command line it does not close the console window, but
further print commands go into nowhere.

## cio::GetTitle()

The function returns the title text of the console window.

## cio::SetTitle()

Set the title of the console window.

## cio::NoBreak()

After calling this function the ScriptBasic interpreter will ignore
the user pressing control-c.

## cio::Break()

After calling this function if the user presses control-c the program terminates.
This is the default behaviour. This function should be used to return to the default
behaviour after calling `nobreak`.

## cio::BufferSizeX

This function returns the console buffer horizontal size.

The console buffer is the character buffer that holds all characters
that are visible on the console or that can be scrolled to the console
using the scroll bar.

## cio::BufferSizeY

This function returns the console buffer vertical size.

The console buffer is the character buffer that holds all characters
that are visible on the console or that can be scrolled to the console
using the scroll bar.

## cio::CursorX

Get the actual cursor position. The function returns the horizontal
position of the console cursor.

## cio::CursorY

Get the actual cursor position. The function returns the vertical
position of the console cursor.

## cio::SizeX

This function returns the horizontal size of the console window in terms
of characters.

## cio::SizeY

This function returns the vertical size of the console window in terms
of characters.

## cio::PossibleMaxX

This function returns the possible maximal horizontal size of the console
that could fit on the screen using the actual character set and screen
resolution.

## cio::PossibleMaxY

This function returns the possible maximal vertical size of the console
that could fit on the screen using the actual character set and screen
resolution.

## cio::SetWindow

This function is supposed to set the actual size of the console. However
this does not work properly and I do not know why.

## cio::SetCursor

Use this subroutine to set the cursor size. The argument value gives
the percentages of the height of the cursor relative to the total
character tallness.

## cio::SetColor col

The the color of the consecutive printings or clear screeen `cls`.

The argument should be an integer value defining the actual color.
To create this value the constants defined in `cio.bas` should be used.
These are:

- `FBlue` the foreground in blue
- `FGreen` the foreground is green
- `FRed` the foreground in red
- `FIntense` the foreground is intense
- `BBlue` the background in blue
- `BGreen` the background is green
- `BRed` the background is red
- `BIntense` the background is intense

To set the actual color of the foreground and the background the
programmer may use and expression that `or` connects these values.
For example

```
cio::SetColor FBlue or BIntense
```

sets the foreground blue and the backgound black with intense bit set (a kind
of light grey). To get the actual colors play a bit with the sample program
`testconcol.bas` in the directory examples.

To ease different color settings the header file `cio.bas` defines the following
constants:
- `FGrey`
- `FWhite`
- `BGrey`
- `BWhite`

## cio::Cls

Clear the console screen and set all character cells to the
last color settings. See also `color`.
