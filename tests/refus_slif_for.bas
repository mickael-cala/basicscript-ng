' Le THEN d'un IF d'une ligne tient exactement une commande, et cette
' commande ne peut pas ouvrir une construction qui continue sur les lignes
' suivantes : la branche sortirait de la ligne. Refus a l'analyse (E28).
if 1 then for i = 1 to 2
