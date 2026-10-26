' La definition d'une subroutine borne la portee de EXIT : le for ouvert
' autour de la definition n'est pas une boucle ou EXIT peut sauter. C'est
' l'erreur &H74 sur la ligne exit que ce cas verrouille ; les erreurs qui
' suivent viennent de la declaration d'une subroutine dans une boucle, que
' le langage refuse par ailleurs (E75), et ne sont pas ce que l'on teste.
for i = 1 to 2
  sub essai
    exit
  end sub
  essai()
next
