import win.bas

print win::RegRead("HKLM\\SOFTWARE\\SCRIPTBASIC\\CONFIG"),"\n"
print win::RegRead("HKLM\\SOFTWARE\\SCRIPTBASIC\\DWORD"),"\n"
print win::RegRead("HKLM\\SOFTWARE\\SCRIPTBASIC\\BIN"),"\n"

' win::RegDel "HKLM\\SOFTWARE\\SCRIPTBASIC\\BIN"
win::RegWrite "HKLM\\SOFTWARE\\SCRIPTBASIC\\BIN1","kakukk"
win::RegWrite "HKLM\\SOFTWARE\\SCRIPTBASIC\\DWORD1",614

win::RegWrite "HKLM\\SOFTWARE\\SCRIPTBASIC\\" , "default value"

' win::RegWrite "HKLM\\SOFTWARE\\SCRIPTBASIC\\SUB\\DWORD1",614

