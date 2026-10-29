' bytes.bas - le module bytes : fabrication et lecture d'octets (phase 8.2).
'
' Ce que le test etablit :
'   - Pack produit les octets EXACTS, dans l'ordre demande (verifies par
'     bytes::Asc sur chaque octet, 0..255) ;
'   - Get relit exactement ce que Pack a ecrit, sur u/i/f et les deux
'     ordres d'octets, y compris 32 bits non signe et negatifs 16 bits ;
'   - Crc16 rend le CRC-16/MODBUS de reference ("123456789" -> 0x4B37) ;
'   - un champ hors borne leve l'erreur 8, trappable.

global echecs
global essais
global ok
echecs = 0
essais = 0
ok = 1

sub check(nom, obtenu, attendu)
  essais = essais + 1
  if str(obtenu) = str(attendu) then
    print "  OK    ", nom, "   -> ", str(obtenu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", str(obtenu), " attendu=", str(attendu), "\n"
    echecs = echecs + 1
  endif
end sub

import bytes.bas

print "=== extension bytes (octets, endianness, CRC) ===\n"

' 1. taille du format
check "size u16be f32le x2", bytes::Size("u16be f32le x2"), 8

' 2. les octets exacts d'un u16be
t = bytes::Pack("u16be", 0x1234)
check "pack u16be taille",   len(t),            2
check "u16be octet 1",       bytes::Asc(t),      0x12
check "u16be octet 2",       bytes::Asc(right(t,1)), 0x34

' 3. l'ordre LE : les memes octets inversement
t = bytes::Pack("u16le", 0x1234)
check "u16le octet 1",       bytes::Asc(t),      0x34
check "u16le octet 2",       bytes::Asc(right(t,1)), 0x12

' 4. les negatifs en 16 bits signes : -1 -> FF FF
t = bytes::Pack("i16be", -1)
check "i16be -1 o1",         bytes::Asc(t),      255
check "i16be -1 o2",         bytes::Asc(right(t,1)), 255

' 5. aller-retour 32 bits non signe (le socle Modbus)
t = bytes::Pack("u32le", 4294967295)
check "u32le taille",        len(t),            4
check "u32le relit",         bytes::Get("u32le", t, 0), 4294967295

' 6. aller-retour signe et float : le format passe a Get est celui de
'    la trame ENTIERE, l'index designe le champ dans ce format.
t = bytes::Pack("i32be f64be", -123456, 2.5)
check "i32be relit",         bytes::Get("i32be f64be", t, 0), -123456
check "f64be relit",         bytes::Get("i32be f64be", t, 1), 2.5

' 7. f32 : le flottant simple precision
t = bytes::Pack("f32be", 2.5)
check "f32be relit",         bytes::Get("f32be", t, 0), 2.5

' 8. le bourrage
t = bytes::Pack("u8 x3 u8", 1, 2)
check "bourrage taille",     len(t),            5
check "bourrage o2",         bytes::Asc(mid(t,2,1)), 0

' 9. CRC-16/MODBUS de reference ("123456789" -> 4B37)
c = bytes::Crc16("123456789")
check "crc16 reference",     c,                 0x4B37

' 10. Get hors borne : erreur 8, trappable
on error goto Attrape
v = bytes::Get("u16be", t, 9)
print "  ECHEC : hors borne ne leve rien\n"
ok = 0
goto Suite
Attrape:
Suite:
on error goto null

if ok = 1 and echecs = 0 then
  print "\nbytes : OK\n"
else
  print "\nbytes : ECHEC\n"
end if
print "essais : ", essais, "   echecs : ", echecs, "\n"
