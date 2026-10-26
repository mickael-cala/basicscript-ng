' re.bas - test fonctionnel de l'extension re
'
' Le module re expose le moteur d'expressions regulieres de Henry Spencer, le
' meme code que celui de la libc BSD. C'est le premier module du portage dont
' les sources ne compilaient pas du tout a l'origine : la distribution ne livre
' ni regex.h, ni utils.h, ni regex2.h, ni cclass.h, ni cname.h, ni engine.ci,
' les six fichiers du moteur. Ils ont ete reconstitues a partir des .c eux-memes
' (voir PATCHES.md section B.9), et ce test est la contre-epreuve : les types,
' les prototypes et les tables sont coherents si, et seulement si, le moteur
' calcule juste.
'
' Les attendus ne sont pas inventes. Les trois premiers viennent du manuel du
' module, qui accompagne les sources (blocs =verbatim de interface.c) : ils
' fixent la semantique de dollar(), de $0 et de $1, ainsi que le contenu du
' remplacement. Les suivants exercent ce que ces trois-la ne touchent pas :
' classes POSIX, repetition bornee, alternative, ancres, option du module et
' echappement.
'
' Trois particularites du langage et du module, verifiees pendant le portage :
'
' 1) Une fonction du module rend -1 quand elle reussit et 0 quand le motif ne
'    correspond pas : -1 est le "vrai" de ScriptBasic. Le test compare donc a
'    zero au lieu de comparer a -1, ce qui reste vrai si la convention change.
'
' 2) Les constantes declarees dans un module s'accedent sous le nom du module :
'    re::CaseInsensitive, comme cgi::Get dans les exemples du depot. Les ecrire
'    telles quelles donne undef et l'option vaut alors zero, sans le moindre
'    message : c'est le piege du mecanisme d'option.
'
' 3) interface.c impose REG_EXTENDED. Or en mode etendu l'antislash rend le
'    caractere suivant ordinaire : \1 est le chiffre 1, pas une reference
'    arriere (celles-ci n'existent qu'en mode basic). Le test verifie donc ce
'    comportement-la, qui est le comportement POSIX correct.
'
' Enfin, pour ecrire ces essais : l'apostrophe ferme un commentaire meme dans une
' chaine, un guillemet dans un commentaire ouvre une chaine, et l'antislash est
' construit par chr(92) pour ne pas dependre du lexeur.

import re.bas

ok = 1
print "=== extension re (expressions regulieres, moteur Henry Spencer) ===\n"

' ---------------------------------------------------------------------------
' 1. L'exemple du manuel du module, recopie tel quel.
'    Le motif est le deuxieme argument, $0 est la correspondance entiere et $1
'    le premier groupe.
' ---------------------------------------------------------------------------
r = re::match("ahlma","h(.*)","haho $0 $1 $0 q")
print "exemple du manuel          : [", r, "]\n"
if r <> "haho hlma lma hlma q" then ok = 0

n = re::n()
print "nombre de groupes          : ", n, "  (attendu 1)\n"
if n <> 1 then ok = 0

d0 = re::dollar(0)
d1 = re::dollar(1)
print "dollar(0) / dollar(1)      : [", d0, "] / [", d1, "]\n"
if d0 <> "hlma" then ok = 0
if d1 <> "lma" then ok = 0

' ---------------------------------------------------------------------------
' 2. Repetition bornee, ancres et trois groupes : une date ISO.
' ---------------------------------------------------------------------------
m = re::match("2008-05-12","^([0-9]{4})-([0-9]{2})-([0-9]{2})$")
print "date ISO reconnue          : ", m, "  (non nul attendu)\n"
if m = 0 then ok = 0
y = re::dollar(1)
mo = re::dollar(2)
da = re::dollar(3)
print "annee / mois / jour        : ", y, " ", mo, " ", da, "  (attendu 2008 05 12)\n"
if y <> "2008" then ok = 0
if mo <> "05" then ok = 0
if da <> "12" then ok = 0

' Un jour hors format : la meme expression ne doit plus convenir.
m2 = re::match("2008-05-12x","^([0-9]{4})-([0-9]{2})-([0-9]{2})$")
print "date ISO avec un suffixe   : ", m2, "  (attendu 0)\n"
if m2 <> 0 then ok = 0

' ---------------------------------------------------------------------------
' 3. Recherche simple, sans ancres : la correspondance commence ou elle veut.
' ---------------------------------------------------------------------------
s = re::match("hello world","world")
print "recherche sans ancre       : ", s, "  (non nul attendu)\n"
if s = 0 then ok = 0

' ---------------------------------------------------------------------------
' 4. Alternative et classe de caracteres explicite.
' ---------------------------------------------------------------------------
a = re::match("Le chat dort","^Le (chat|chien) [a-z]+$")
g = re::dollar(1)
print "alternative capturee       : ", a, " [", g, "]  (attendu non nul [chat])\n"
if a = 0 then ok = 0
if g <> "chat" then ok = 0

' ---------------------------------------------------------------------------
' 5. Classes POSIX nommees : c'est le seul chemin du moteur qui lise la table
'    cclasses de cclass.h, donc la seule facon de verifier cette table.
' ---------------------------------------------------------------------------
c = re::match("abc123","[[:digit:]]+")
chiffres = re::dollar(0)
print "classe [[:digit:]]         : ", c, " [", chiffres, "]  (attendu non nul [123])\n"
if c = 0 then ok = 0
if chiffres <> "123" then ok = 0

c2 = re::match("abc123","[[:upper:]]+")
print "classe [[:upper:]] absente : ", c2, "  (attendu 0)\n"
if c2 <> 0 then ok = 0

' La classe nommee se combine a une negation de classe.
c3 = re::match("abc123","[^[:digit:]]+")
lettres = re::dollar(0)
print "classe niée [[:digit:]]    : ", c3, " [", lettres, "]  (attendu non nul [abc])\n"
if c3 = 0 then ok = 0
if lettres <> "abc" then ok = 0

' ---------------------------------------------------------------------------
' 6. Non-correspondance : la fonction doit rendre faux, pas planter.
' ---------------------------------------------------------------------------
z = re::match("abc","^[0-9]+$")
print "motif non satisfait        : ", z, "  (attendu 0)\n"
if z <> 0 then ok = 0

' ---------------------------------------------------------------------------
' 7. Sensibilite a la casse, puis option du module.
'    L'option est lue par interface.c sous le nom re$flag ; CaseInsensitive
'    vaut &HFFFFFFFE, valeur que le module inverse pour en extraire son bit
'    interne. Elle doit donc etre nommee sous forme qualifiee (re::).
' ---------------------------------------------------------------------------
cs = re::match("ABC","abc")
print "casse par defaut           : ", cs, "  (attendu 0)\n"
if cs <> 0 then ok = 0

option re$flag re::CaseInsensitive
print "option memorisee           : ", option("re$flag"), "  (attendu -2)\n"
if option("re$flag") <> -2 then ok = 0

ci = re::match("ABC","abc")
print "option CaseInsensitive     : ", ci, "  (non nul attendu)\n"
if ci = 0 then ok = 0

' MultiLine vaut &HFFFFFFFD, soit -3, et se pose aussi seul.
option re$flag re::MultiLine
print "option MultiLine seule     : ", option("re$flag"), "  (attendu -3)\n"
if option("re$flag") <> -3 then ok = 0

' Les deux constantes se cumulent mal, et ce n'est pas un defaut du portage :
' elles valent -2 et -3, dont le ou bit a bit vaut -1, que le module inverse en
' zero, soit aucune option. Une option a la fois, donc.
print "les deux par ou            : ", (re::CaseInsensitive or re::MultiLine), "  (attendu -1)\n"
if (re::CaseInsensitive or re::MultiLine) <> -1 then ok = 0

' ---------------------------------------------------------------------------
' 8. Echappement en mode etendu : l'antislash rend le caractere suivant
'    ordinaire. Le point redevient un point, et \1 un chiffre.
' ---------------------------------------------------------------------------
bs = chr(92)
point = "a" & bs & ".b"
e1 = re::match("a.b",point)
e2 = re::match("axb",point)
print "point echappe              : ", e1, " / ", e2, "  (attendu non nul / 0)\n"
if e1 = 0 then ok = 0
if e2 <> 0 then ok = 0

un = "a" & bs & "1"
l1 = re::match("a1",un)
l2 = re::match("aa",un)
print "antislash 1 = chiffre 1    : ", l1, " / ", l2, "  (attendu non nul / 0)\n"
if l1 = 0 then ok = 0
if l2 <> 0 then ok = 0

' ---------------------------------------------------------------------------
' 9. Remplacement global, puis mise en forme : les deux autres entrees du
'    module, qui partagent l'analyseur de $1, $2 ...
' ---------------------------------------------------------------------------
rp = re::s("almakbat*","a(.)","$1s")
print "remplacement global        : [", rp, "]  (attendu [lsmksbts*])\n"
if rp <> "lsmksbts*" then ok = 0

' Le manuel precise qu'apres un remplacement, dollar(0) est la derniere
' correspondance et non le texte entier : ici le dernier a(.) de la chaine.
rd0 = re::dollar(0)
rd1 = re::dollar(1)
print "dollar apres remplacement  : [", rd0, "] [", rd1, "]  (attendu [at] [t])\n"
if rd0 <> "at" then ok = 0
if rd1 <> "t" then ok = 0

m3 = re::match("2008-05-12","([0-9]{4})-([0-9]{2})-([0-9]{2})")
f = re::format("le $3/$2/$1")
print "mise en forme              : [", f, "]  (attendu [le 12/05/2008])\n"
if f <> "le 12/05/2008" then ok = 0

' ---------------------------------------------------------------------------
' 10. Entree longue : 200 caracteres a balayer, pour faire travailler le moteur
'     de correspondance plutot que l'analyseur de motif.
' ---------------------------------------------------------------------------
longue = string(200,"a") & "b"
l = re::match(longue,"a+b$")
print "entree de 201 caracteres   : ", l, "  (non nul attendu)\n"
if l = 0 then ok = 0

if ok then
  print "\nre : OK\n"
else
  print "\nre : ECHEC\n"
end if
