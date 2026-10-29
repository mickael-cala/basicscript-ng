<!-- Fichier engendre par pod2md.py depuis les sources. -->
<!-- Ne pas editer a la main : python pod2md.py --check le verifie. -->

# Module `bytes`

Source : `src/extensions/bytes/interface.c`

## pack::Pack
@c Build a binary string out of the format and the values

Usage:

```
trame = pack::Pack("u16be u16be f32le x2", id, 40001, 3.14)
```

Rend la chaÃ®ne binaire : chaque jeton du format consomme une valeur
(dans l'ordre), dans l'ordre d'octets demandÃ©. Les jetons x ne
consomment rien : ils bourrent de zÃ©ros. L'ordre par dÃ©faut est BE.

## pack::Size
@c Number of bytes of the format

Usage:

```
taille = pack::Size("u16be f32le x2")   ' 8
```

Rend le nombre d'octets total des jetons du format.

## pack::Get
@c The value of the i-th field of a binary string

Usage:

```
valeur = pack::Get("u16be f32le", trame, 0)   ' premier champ
```

Rend la valeur du champ numÃ©ro i (Ã  partir de zÃ©ro) de la chaÃ®ne
binaire : un entier pour les jetons u/i, un rÃ©el pour les f. Trame
trop courte, champ de bourrage ou jeton inconnu : erreur 8.

## pack::Crc16
@c The CRC-16/MODBUS of a binary string

Usage:

```
crc = pack::Crc16(trame)   ' le CRC du RTU, sur 16 bits
```

Rend le CRC-16/MODBUS de la chaÃ®ne (polynÃ´me rÃ©flÃ©chi 0xA001, init
0xFFFF) : la valeur attendue en fin de trame RTU, petit-boutiste.

## pack::Asc
@c The code of the first byte of a string

Usage:

```
code = pack::Asc(mid$(trame,3,1))   ' le 3e octet, 0..255
```

Rend le code du premier octet de la chaÃ®ne, 0 Ã  255. Une chaÃ®ne vide
rend `undef`. C'est le lecteur d'octet du langage : la chaÃ®ne est
binaire-sÃ»re, ce lecteur l'est aussi.
