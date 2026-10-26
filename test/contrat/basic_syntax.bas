' test/contrat/basic_syntax.bas
' Test de validation de la grammaire de base et des structures de contrôle

PRINT "Début du test de syntaxe"

' Test de variables et affectation
DIM a AS INTEGER
a = 10
PRINT "Valeur de a : ", a

' Test de structure conditionnelle
IF a = 10 THEN
    PRINT "Condition IF valide"
ELSE
    PRINT "Erreur IF"
ENDIF

' Test de boucle FOR avec EXITFOR (ajout NG)
FOR i = 1 TO 3
    IF i = 2 THEN
        EXITFOR
    ENDIF
    PRINT "Boucle i = ", i
NEXT

' Test de fonction chaîne avec suffixe $ obligatoire
DIM nom AS STRING
nom = "ScriptBasic"
PRINT "Nom en majuscules : ", UCASESTR(nom)

PRINT "Fin du test. TOUT OK."