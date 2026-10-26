' slif.bas - le IF sur une seule ligne, avec ELSE et ELSEIF.
'
' La forme d'origine IF condition THEN commande est inchangee ; la forme
' etendue accepte en plus ELSE et une chaine de ELSEIF, chaque branche etant
' une seule commande de la meme ligne :
'
'   IF a THEN x ELSEIF b THEN y ELSE z
'
' Tout est verifie par l'effet des branches sur une variable, jamais par
' l'analyse du source : le programme se termine par `error 1` des qu'un ecart
' est vu.

global echecs
global essais
echecs = 0
essais = 0

sub check(nom, obtenu, attendu)
  essais = essais + 1
  if str(obtenu) = str(attendu) then
    print "  OK    ", nom, "   -> ", str(obtenu), "\n"
  else
    print "  ECHEC ", nom, "   obtenu=", str(obtenu), " attendu=", str(attendu), "\n"
    echecs = echecs + 1
  endif
end sub

' --------------------------------------------------------------------------
' 1. La forme d'origine, sans ELSE : la branche ne s'execute que si vrai.
' --------------------------------------------------------------------------

print "=== 1. IF THEN sans ELSE ===\n"

r = "vide"
if 1 then r = "vrai"
check "vrai", r, "vrai"

r = "vide"
if 0 then r = "vrai"
check "faux ne touche a rien", r, "vide"

r = 0
if 10 > 3 then r = 1
check "condition calculee", r, 1

' --------------------------------------------------------------------------
' 2. ELSE sur la meme ligne.
' --------------------------------------------------------------------------

print "\n=== 2. IF THEN ... ELSE ... ===\n"

r = ""
if 1 then r = "alors" else r = "sinon"
check "vrai prend THEN", r, "alors"

r = ""
if 0 then r = "alors" else r = "sinon"
check "faux prend ELSE", r, "sinon"

' Les deux branches ne peuvent pas s'executer : la seconde ecrase la
' premiere si le saut qui suit THEN manque.
r = ""
if 1 then r = r & "a" else r = r & "b"
check "une seule branche (vrai)", r, "a"

r = ""
if 0 then r = r & "a" else r = r & "b"
check "une seule branche (faux)", r, "b"

' Conditions non numeriques : undef est faux, une chaine vide est fausse.
r = ""
if undef then r = "alors" else r = "sinon"
check "undef est faux", r, "sinon"

r = ""
if "" then r = "alors" else r = "sinon"
check "chaine vide est fausse", r, "sinon"

r = ""
if "x" then r = "alors" else r = "sinon"
check "chaine non vide est vraie", r, "alors"

' --------------------------------------------------------------------------
' 3. Chaines de ELSEIF, chaque position de la branche vraie.
' --------------------------------------------------------------------------

print "\n=== 3. IF THEN ... ELSEIF ... ELSE ===\n"

sub chaine(a, b)
  local r
  r = "aucune"
  if a then r = "a" elseif b then r = "b" else r = "c"
  chaine = r
end sub

check "premier",  chaine(1, 1), "a"
check "second",   chaine(0, 1), "b"
check "else",     chaine(0, 0), "c"

' Trois ELSEIF et un ELSE : la branche du milieu.
sub chaine3(x)
  local r
  r = "aucune"
  if x = 1 then r = "un" elseif x = 2 then r = "deux" elseif x = 3 then r = "trois" else r = "autre"
  chaine3 = r
end sub

check "chaine 1", chaine3(1), "un"
check "chaine 2", chaine3(2), "deux"
check "chaine 3", chaine3(3), "trois"
check "chaine 4", chaine3(9), "autre"

' Sans ELSE, une chaine fausse ne fait rien.
r = "vide"
if 0 then r = "a" elseif 0 then r = "b"
check "chaine fausse sans ELSE", r, "vide"

' Un ELSEIF vrai s'arrete la : les suivants ne s'executent pas.
r = ""
if 0 then r = r & "a" elseif 1 then r = r & "b" elseif 1 then r = r & "c"
check "arret au premier vrai", r, "b"

' --------------------------------------------------------------------------
' 4. Imbrication : le IF d'une ligne dans un bloc, et l'inverse.
' --------------------------------------------------------------------------

print "\n=== 4. Imbrication ===\n"

r = ""
if 1 then
  if 0 then r = "interne" else r = "else-interne"
  r = r & "-suite"
end if
check "IF d'une ligne dans un bloc", r, "else-interne-suite"

' Le ELSE du bloc appartient au bloc, pas au IF d'une ligne qui le precede :
' la condition du bloc est vraie, donc rien du ELSE ne doit s'executer.
r = "depart"
if 1 then
  if 0 then r = "interne"
else
  r = "bloc-else"
end if
check "ELSE du bloc non pris", r, "depart"

r = ""
if 1 then
  for i = 1 to 3
    if i = 2 then r = r & "deux"
  next
end if
check "IF d'une ligne dans une boucle", r, "deux"

r = ""
if 1 then
  while len(r) < 3
    if len(r) = 0 then r = r & "a" else r = r & "b"
  wend
end if
check "IF d'une ligne dans un WHILE", r, "abb"

' Le IF d'une ligne est la derniere commande du corps du bloc.
r = "fin"
if 1 then
  if 0 then r = "change"
end if
check "derniere commande du corps", r, "fin"

' --------------------------------------------------------------------------
' 5. Effets de bord et commandes diverses comme branches.
' --------------------------------------------------------------------------

print "\n=== 5. Branches variees ===\n"

n = 0
if 1 then n = n + 1 else n = n - 1
check "branche THEN", n, 1

if 0 then n = n + 1 else n = n - 1
check "branche ELSE", n, 0

r = ""
if 1 then r = "valeur" else print "jamais\n"
check "branche ELSE non executee", r, "valeur"

' Un GOTO dans une branche saute bien au label.
cible = ""
if 1 then goto marque
cible = "rate"
marque:
if cible = "" then cible = "saute"
check "goto dans THEN", cible, "saute"

' Une fonction dans la condition, et un appel de sous-programme dans une
' branche (CALL est une commande comme une autre).
function pair(n)
  if n % 2 = 0 then pair = 1 else pair = 0
end function
r = ""
if pair(4) then r = "pair" else r = "impair"
check "condition fonction", r, "pair"

sub dit(v)
  r = r & v
end sub
r = ""
if 1 then call dit("a") else call dit("b")
check "call dans THEN", r, "a"

print "\n", essais, " essais, ", echecs, " echec(s)\n"
if echecs then error 1
