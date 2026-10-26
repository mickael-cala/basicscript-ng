'
' FILE: unix.bas
'
' This is the module declaration of the ScriptBasic external module unix
'
module unix

const S_ISUID  = &H800
const S_ISGID  = &H400
const S_ISVTX  = &H200
const S_IRUSR  = &H100
const S_IREAD  = &H100
const S_IWUSR  = &H080
const S_IWRITE = &H080
const S_IXUSR  = &H040
const S_IEXEC  = &H040
const S_IRGRP  = &H020
const S_IWGRP  = &H010
const S_IXGRP  = &H008
const S_IROTH  = &h004
const S_IWOTH  = &H002
const S_IXOTH  = &H001

declare sub ::fork alias  "unixfork"  lib "unix"
declare sub ::chmod alias "unixchmod" lib "unix"

end module
