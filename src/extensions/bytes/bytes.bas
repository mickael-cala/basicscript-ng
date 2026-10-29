' """
FILE: bytes.bas

This is the BASIC import file for the module bytes.

This is the import file of the module bytes. It is an ordinary source
file: edit it here.
"""

module bytes

' FUNCTION DECLARATIONS 
declare sub     ::Pack   alias "bytes_pack"   lib "bytes"
declare sub     ::Size   alias "bytes_size"   lib "bytes"
declare sub     ::Get    alias "bytes_get"    lib "bytes"
declare sub     ::Crc16  alias "bytes_crc16"  lib "bytes"
declare sub     ::Asc    alias "bytes_asc"    lib "bytes"

end module
