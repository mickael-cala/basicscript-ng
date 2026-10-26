' """
FILE: win.bas

This is the BASIC import file for the module win.

This is the import file of the module win. It is an ordinary source
file: edit it here.
"""

module win

' FUNCTION DECLARATIONS 
declare sub     ::RegRead         alias "regread"            lib "win"
declare sub     ::RegDel          alias "regdel"             lib "win"
declare sub     ::RegWrite        alias "regwrite"           lib "win"
declare sub     ::MsgBox          alias "msgbox"             lib "win"
declare sub     ::ShutDown        alias "win_shutdown"        lib "win"
declare sub     ::ListProcesses   alias "proclist"           lib "win"
declare sub     ::StartService    alias "win_startservice"    lib "win"
declare sub     ::StopService     alias "win_stopservice"     lib "win"
declare sub     ::PauseService    alias "win_pauseservice"    lib "win"
declare sub     ::ContinueService alias "win_continueservice" lib "win"
declare sub     ::HardLink        alias "win_hardlink"        lib "win"

end module
