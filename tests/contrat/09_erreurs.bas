' contrat 09 : le langage crie (phase 8.1) - la division entiere par zero
' leve l'erreur 3 trappable et lisible ; les poles reels rendent undef.
on error goto Attrape
x = 1 \ 0
print "jamais atteint\n"
end
Attrape:
print "division entiere : code ", error, "\n"
print "division entiere : [", error$(), "]\n"

print "pole reel cotan(0) : ", isundef(cotan(0)), "\n"

on error resume next
y = 0
z = 5 \ y
print "resume next : code ", error, "\n"
print "fin 09\n"
