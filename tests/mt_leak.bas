' mt_leak.bas - preuve d'equilibre memoire du module dyc
'
' Le chantier memoire du portage a corrige la fuite du finimodu d'origine :
' la boucle liberait les DLL (FreeLibrary) mais JAMAIS les noeuds DllLoaded
' ni leurs pszName - en multithread (sbhttpd : un bootmodu/finimodu par
' requete), chaque fin d'interpreteur laissait au tas le ModuleObject, les
' noeuds et les chaines. Le finimodu corrige libere le tout, puis le
' ModuleObject lui-meme (le noyau ne relit jamais ModuleInternalParameters
' apres finimodu ; en double finimodu, le NULL est le second signal d'arret).
'
' LA PREUVE (deux niveaux, les deux obligatoires) :
'
' 1. Ce programme est rejoue par bin/scriba_dbg.exe, l'hote construit avec
'    -D_DEBUG (voir rsp/scriba_dbg.rsp) : tout le noyau passe par le
'    distributeur de diagnostic testalloc.c, et la fin du processus rend
'    "No memory leak" et le code du programme A L'EQUILIBRE, ou la liste
'    des blocs perdus et le code 1276 sinon. Si le finimodu fuyait toujours
'    (un ModuleObject, un noeud et une chaine par chargement de DLL), ce
'    programme echouerait a chaque fin d'interpreteur.
'
' 2. 1500 appels dyc, puis l'export LeakReport du module : le solde COURANT
'    du distributeur, lu PENDANT l'execution. En 32 bits, DynaCall malloc/free
'    son bloc d'arguments via le CRT du DLL, qui est le meme distributeur
'    sous -D_DEBUG : un tableau Parm non restitue se verrait ici. En 64 bits
'    le bloc d'arguments vit dans la struct DYC64 (pas de malloc) : l'export
'    rend le solde du noyau, tout aussi verifie.
'
' Lancer (le test officiel l'appelle via l'hote _DEBUG) :
'   bin/scriba_dbg.exe test/mt_leak.bas     (32 bits, audit reel)
'   bin/scriba.exe test/mt_leak.bas         (32 bits, second chemin)
'   bin64/scriba64.exe test/mt_leak.bas     (64 bits, second chemin)

import dyc.bas

ok = 1
for i = 1 to 1500
  n = dyc::dyc("ms,i,KERNEL32.DLL,lstrlenA,Z","ScriptBasic")
  if n <> 11 then ok = 0
next i

solde = dyc::LeakReport()
print "appels dyc reussis          = 1500 (chaque rendu verifie = 11)\n"
print "solde memoire pendant exec. = ", solde, "  (attendu 0)\n"
if solde <> 0 then ok = 0
if ok then
  print "mt_leak : OK - equilibre memoire constate (voir aussi le bilan final de l'hote _DEBUG)\n"
  else
  print "mt_leak : ECHEC\n"
  end if
