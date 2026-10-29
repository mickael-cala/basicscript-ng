/*

--GNU LGPL
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

pack — la fabrication et la lecture d'octets, phase 8.2.

Ce module répond au besoin des protocoles binaires (Modbus TCP/RTU,
trames embarquées…) : assembler des entiers signés ou non signés de
8 à 64 bits et des flottants IEEE, dans l'ordre d'octets EXPLICITE de
chaque champ, et les relire. Les valeurs restent les entiers 64 bits
du langage — aucun type non signé n'est exposé au BASIC, le module
fait la conversion.

Le format est une chaîne de jetons séparés par des espaces ou des
virgules, insensible à la casse :

  u8 i8                     octets
  u16be u16le ... u64le     entiers, ordre d'octets explicite
  i16be ... i64le           idem, complément à deux
  f32be f32le f64be f64le   flottants IEEE 754
  x  x3                     un ou N octets de bourrage nul

L'ordre par défaut est BE (l'ordre réseau) pour tout ce qui dépasse
l'octet : u16 vaut u16be.

Le CRC-16/MODBUS est fourni : c'est celui du RTU (polynôme réfléchi
0xA001, init 0xFFFF), testable sans bus série.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../../basext.h"

#define PACK_MAXCHAMPS 64

typedef struct packChamp {
  int octets;  /* taille du champ en octets (0 pour le bourrage) */
  int big;     /* 1 : ordre reseau, 0 : petit-boutiste */
  int signe;   /* 1 : complement a deux ; le bourrage est non signe */
  int reel;    /* 1 : IEEE 754 */
  long quantite; /* pour le bourrage : N octets */
  } packChamp;

/* parse le format entier. Rend le nombre de champs (bourrage inclus),
   pose la taille totale d'octets ; rend -1 si un jeton est inconnu. */
static long pack_Parse(const char *pFmt,
                       packChamp *aChamps,
                       long *plTotal){
  long n = 0;
  long total = 0;

  while( *pFmt ){
    int bits = 0, signe = 0, reel = 0, big = 1;
    char *fin;

    while( *pFmt == ' ' || *pFmt == ',' )pFmt++;
    if( ! *pFmt )break;

    if( n >= PACK_MAXCHAMPS )return -1;

    if( *pFmt == 'x' || *pFmt == 'X' ){
      pFmt++;
      if( isdigit((unsigned char)*pFmt) ){
        aChamps[n].quantite = strtol(pFmt,&fin,10);
        if( aChamps[n].quantite < 1 )aChamps[n].quantite = 1;
        pFmt = fin;
        }else{
        aChamps[n].quantite = 1;
        }
      aChamps[n].octets = 0;
      aChamps[n].big = 0;
      aChamps[n].signe = 0;
      aChamps[n].reel = 0;
      total += aChamps[n].quantite;
      n++;
      continue;
      }

    if( *pFmt == 'u' )signe = 0;
    else if( *pFmt == 'i' || *pFmt == 'I' )signe = 1;
    else if( *pFmt == 'f' || *pFmt == 'F' )reel = 1;
    else return -1;
    pFmt++;

    bits = (int)strtol(pFmt,&fin,10);
    if( fin == pFmt )return -1;
    pFmt = fin;
    if( reel ){
      if( bits != 32 && bits != 64 )return -1;
      }else{
      if( bits != 8 && bits != 16 && bits != 32 && bits != 64 )return -1;
      }

    if( pFmt[0] == 'b' && pFmt[1] == 'e' ){ big = 1; pFmt += 2; }
    else if( pFmt[0] == 'l' && pFmt[1] == 'e' ){ big = 0; pFmt += 2; }

    aChamps[n].octets = bits/8;
    aChamps[n].big = big;
    aChamps[n].signe = signe;
    aChamps[n].reel = reel;
    aChamps[n].quantite = 1;
    total += bits/8;
    n++;
    }
  *plTotal = total;
  return n;
  }

/* le champ est-il un flottant ? (le jeton f a une taille 4 ou 8 et le
   parseur a pose reel ; le tableau de champs le porte) */

static void pack_EcrisEntier(unsigned char *dest,
                             long long n,
                             int cb,
                             int big){
  unsigned long long u = (unsigned long long)n;
  int i;

  for( i = 0 ; i < cb ; i++ )
    dest[i] = big ? (unsigned char)(u >> (8*(cb-1-i)))
                  : (unsigned char)(u >> (8*i));
  }

static long long pack_LisEntier(const unsigned char *src,
                                int cb,
                                int big,
                                int signe){
  unsigned long long u = 0;
  int i;

  for( i = 0 ; i < cb ; i++ )
    u = (u << 8) | (big ? src[i] : src[cb-1-i]);
  if( ! signe || cb >= 8 )return (long long)u;
  if( u & (1ULL << (8*cb-1)) )u -= 1ULL << (8*cb);
  return (long long)u;
  }

/*POD
=section bytes
=H pack::Pack
@c Build a binary string out of the format and the values

Usage:

=verbatim
trame = pack::Pack("u16be u16be f32le x2", id, 40001, 3.14)
=noverbatim

Rend la chaîne binaire : chaque jeton du format consomme une valeur
(dans l'ordre), dans l'ordre d'octets demandé. Les jetons x ne
consomment rien : ils bourrent de zéros. L'ordre par défaut est BE.
CUT*/
besFUNCTION(bytes_pack)
  VARIABLE vFmt;
  const char *pFmt;
  unsigned char *s;
  long cbAlloue, cb = 0;
  packChamp aChamps[PACK_MAXCHAMPS];
  long total, iChamp;
  unsigned long iArg = 2;

  besRETURNVALUE = NULL;

  vFmt = besARGUMENT(1);
  besDEREFERENCE(vFmt);
  if( vFmt == NULL || vFmt->vType != VTYPE_STRING )
    return COMMAND_ERROR_ARGUMENT_TYPE;
  pFmt = STRINGVALUE(vFmt);
  if( pack_Parse(pFmt,aChamps,&total) < 0 )
    return COMMAND_ERROR_ARGUMENT_RANGE;

  cbAlloue = total > 64 ? total : 64;
  s = besALLOC((size_t)cbAlloue);
  if( s == NULL )return COMMAND_ERROR_MEMORY_LOW;

  cb = 0;
  for( iChamp = 0 ; iChamp < pack_Parse(pFmt,aChamps,&total) ; iChamp++ ){
    packChamp *pC = aChamps + iChamp;
    if( pC->octets == 0 ){
      memset(s+cb,0,(size_t)pC->quantite);
      cb += pC->quantite;
      continue;
      }
    if( iArg > (unsigned long)besARGNR )return COMMAND_ERROR_FEW_ARGS;
    {
    VARIABLE v = besARGUMENT(iArg);
    besDEREFERENCE(v);
    iArg++;
    if( v == NULL )return COMMAND_ERROR_ARGUMENT_TYPE;

    if( pC->reel ){
      /* IEEE 754 exact par copie de bits, dans l'ordre demandé */
      double d = DOUBLEVALUE(besCONVERT2DOUBLE(v));
      int j;
      if( pC->octets == 4 ){
        float f = (float)d;
        uint32_t u;
        memcpy(&u,&f,4);
        for( j = 0 ; j < 4 ; j++ )
          s[cb+j] = pC->big ? (unsigned char)(u >> (8*(3-j)))
                            : (unsigned char)(u >> (8*j));
        }else{
        uint64_t u;
        memcpy(&u,&d,8);
        for( j = 0 ; j < 8 ; j++ )
          s[cb+j] = pC->big ? (unsigned char)(u >> (8*(7-j)))
                            : (unsigned char)(u >> (8*j));
        }
      }else{
      /* les helpers bes* sont l'ABI du module : GETLONGVALUE est du coeur */
      pack_EcrisEntier(s+cb,LONGVALUE(besCONVERT2LONG(v)),pC->octets,pC->big);
      }
    cb += pC->octets;
    }
    }

  {
  pFixSizeMemoryObject pR = besNEWMORTALSTRING((unsigned long)cb);
  if( pR == NULL )return COMMAND_ERROR_MEMORY_LOW;
  memcpy(STRINGVALUE(pR),s,(size_t)cb);
  besRETURNVALUE = pR;
  }
  return COMMAND_ERROR_SUCCESS;
besEND

/*POD
=section bytes
=H pack::Size
@c Number of bytes of the format

Usage:

=verbatim
taille = pack::Size("u16be f32le x2")   ' 8
=noverbatim

Rend le nombre d'octets total des jetons du format.
CUT*/
besFUNCTION(bytes_size)
  VARIABLE vFmt;
  const char *pFmt;
  packChamp aChamps[PACK_MAXCHAMPS];
  long total;

  besRETURNVALUE = NULL;

  vFmt = besARGUMENT(1);
  besDEREFERENCE(vFmt);
  if( vFmt == NULL || vFmt->vType != VTYPE_STRING )
    return COMMAND_ERROR_ARGUMENT_TYPE;
  pFmt = STRINGVALUE(vFmt);
  if( pack_Parse(pFmt,aChamps,&total) < 0 )
    return COMMAND_ERROR_ARGUMENT_RANGE;

  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) = total;
  return COMMAND_ERROR_SUCCESS;
besEND

/*POD
=section bytes
=H pack::Get
@c The value of the i-th field of a binary string

Usage:

=verbatim
valeur = pack::Get("u16be f32le", trame, 0)   ' premier champ
=noverbatim

Rend la valeur du champ numéro i (à partir de zéro) de la chaîne
binaire : un entier pour les jetons u/i, un réel pour les f. Trame
trop courte, champ de bourrage ou jeton inconnu : erreur 8.
CUT*/
besFUNCTION(bytes_get)
  VARIABLE vFmt, vBin, vInd;
  const char *pFmt;
  const unsigned char *src;
  long cbSrc, cherche;
  packChamp aChamps[PACK_MAXCHAMPS];
  long total, nChamps, iChamp, offset;

  besRETURNVALUE = NULL;

  vFmt = besARGUMENT(1);
  besDEREFERENCE(vFmt);
  vBin = besARGUMENT(2);
  besDEREFERENCE(vBin);
  vInd = besARGUMENT(3);
  besDEREFERENCE(vInd);
  if( vFmt == NULL || vFmt->vType != VTYPE_STRING ||
      vBin == NULL || vBin->vType != VTYPE_STRING )
    return COMMAND_ERROR_ARGUMENT_TYPE;
  if( vInd == NULL )return COMMAND_ERROR_ARGUMENT_TYPE;
  pFmt = STRINGVALUE(vFmt);
  src = (const unsigned char *)STRINGVALUE(vBin);
  cbSrc = (long)STRLEN(vBin);

  nChamps = pack_Parse(pFmt,aChamps,&total);
  if( nChamps < 0 )return COMMAND_ERROR_ARGUMENT_RANGE;
  cherche = LONGVALUE(besCONVERT2LONG(vInd));
  if( cherche < 0 || cherche >= nChamps )return COMMAND_ERROR_ARGUMENT_RANGE;

  offset = 0;
  for( iChamp = 0 ; iChamp < cherche ; iChamp++ )
    offset += aChamps[iChamp].octets ? aChamps[iChamp].octets
                                     : aChamps[iChamp].quantite;
  {
  packChamp *pC = aChamps + cherche;
  if( pC->octets == 0 || offset + pC->octets > cbSrc )
    return COMMAND_ERROR_ARGUMENT_RANGE;

  if( pC->reel ){
    double d;
    int j;
    uint64_t u = 0;
    for( j = 0 ; j < pC->octets ; j++ )
      u = (u << 8) | (pC->big ? src[offset+j] : src[offset+pC->octets-1-j]);
    if( pC->octets == 4 ){
      uint32_t u32 = (uint32_t)u;
      float f;
      memcpy(&f,&u32,4);
      d = f;
      }else{
      memcpy(&d,&u,8);
      }
    besALLOC_RETURN_DOUBLE;
    DOUBLEVALUE(besRETURNVALUE) = d;
    return COMMAND_ERROR_SUCCESS;
    }
  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) =
    pack_LisEntier(src+offset,pC->octets,pC->big,pC->signe);
  }
  return COMMAND_ERROR_SUCCESS;
besEND

/*POD
=section bytes
=H pack::Crc16
@c The CRC-16/MODBUS of a binary string

Usage:

=verbatim
crc = pack::Crc16(trame)   ' le CRC du RTU, sur 16 bits
=noverbatim

Rend le CRC-16/MODBUS de la chaîne (polynôme réfléchi 0xA001, init
0xFFFF) : la valeur attendue en fin de trame RTU, petit-boutiste.
CUT*/
besFUNCTION(bytes_crc16)
  VARIABLE vBin;
  const unsigned char *src;
  long cb, i;
  unsigned int crc = 0xFFFF;
  int bit;

  besRETURNVALUE = NULL;

  vBin = besARGUMENT(1);
  besDEREFERENCE(vBin);
  if( vBin == NULL || vBin->vType != VTYPE_STRING )
    return COMMAND_ERROR_ARGUMENT_TYPE;
  src = (const unsigned char *)STRINGVALUE(vBin);
  cb = (long)STRLEN(vBin);

  for( i = 0 ; i < cb ; i++ ){
    crc ^= src[i];
    for( bit = 0 ; bit < 8 ; bit++ )
      crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : (crc >> 1);
    }
  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) = (long long)crc;
  return COMMAND_ERROR_SUCCESS;
besEND

/*POD
=section bytes
=H pack::Asc
@c The code of the first byte of a string

Usage:

=verbatim
code = pack::Asc(mid$(trame,3,1))   ' le 3e octet, 0..255
=noverbatim

Rend le code du premier octet de la chaîne, 0 à 255. Une chaîne vide
rend T<undef>. C'est le lecteur d'octet du langage : la chaîne est
binaire-sûre, ce lecteur l'est aussi.
CUT*/
besFUNCTION(bytes_asc)
  VARIABLE v;

  besRETURNVALUE = NULL;

  v = besARGUMENT(1);
  besDEREFERENCE(v);
  if( v == NULL )return COMMAND_ERROR_SUCCESS;
  v = besCONVERT2STRING(v);
  if( STRLEN(v) == 0 ){
    besRETURNVALUE = NULL;
    return COMMAND_ERROR_SUCCESS;
    }
  besALLOC_RETURN_LONG;
  LONGVALUE(besRETURNVALUE) = (long long)(unsigned char)STRINGVALUE(v)[0];
  return COMMAND_ERROR_SUCCESS;
besEND

besSUB_START
  return COMMAND_ERROR_SUCCESS;
besEND

besSUB_FINISH
  return COMMAND_ERROR_SUCCESS;
besEND

besVERSION_NEGOTIATE
  return (int)INTERFACE_VERSION;
besEND

/* la table des fonctions exportees : la porte du module, aussi bien en
   chargement dynamique qu'en lien statique (phase 6). */
SLFST BYTES_SLFST[] ={
{ "versmodu" , versmodu },
{ "bootmodu" , bootmodu },
{ "finimodu" , finimodu },
{ "bytes_pack" , bytes_pack },
{ "bytes_size" , bytes_size },
{ "bytes_get" , bytes_get },
{ "bytes_crc16" , bytes_crc16 },
{ "bytes_asc" , bytes_asc },
{ NULL , NULL }
};
