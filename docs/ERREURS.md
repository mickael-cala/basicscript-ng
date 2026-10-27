# ERREURS — la référence du traitement des erreurs

Ce document est la référence du sujet dans ce dépôt : comment une erreur
naît, comment elle se signale, comment elle s'attrape, comment elle se
lit. La table complète des codes est **engendrée du code** dans
[ANNEXE-ERREURS.md](ANNEXE-ERREURS.md) — comme tout ce qui est généré
ici, elle ne s'édite pas à la main.

## 1. La philosophie : crier sur l'entier, pôles réels = `undef`

Deux mondes, deux honnêtetés :

- **La division (et le modulo) entière par zéro lève l'erreur 3**
  (`COMMAND_ERROR_DIV`, message « Division by zero or other calculation
  error »). Aucune valeur entière n'est honnête ici : le langage crie,
  l'erreur est **trappable** et **lisible**. Avant la phase 8.1
  (2026-10-26), elle rendait `undef` en silence — voir
  [COMPATIBILITE.md](COMPATIBILITE.md).
- **Le pôle d'une fonction réelle rend `undef`** : `cotan(0)`,
  `tan2(1,0)`, `cosecant(0)` rendent `undef` — c'est l'idiome testé
  (`isundef(cotan(0))` dans `tests/trig.bas`), mathématiquement honnête
  (« non défini »), et `undef` se propage : un calcul bâti sur un pôle
  reste `undef`, jamais un nombre trompeur.
- **Un opérande `undef` se propage** : `undef + 1` rend `undef`. Le
  diagnostic de ce chemin existe mais reste opt-in (voir § 4).

## 2. Attraper : `on error`

```basic
on error goto Attrape
x = 1 \ 0                      ' division entière par zéro
print "jamais atteint\n"
end
Attrape:
print "code   : ", error, "\n"       ' 3
print "message: ", error$(), "\n"    ' Division by zero or...
```

- `on error goto label` — l'erreur saute au label ; l'exécution y reste.
- `on error resume next` — l'erreur saute à **l'instruction suivante** ;
  l'instruction fautive ne produit pas de valeur.
- `resume` / `resume next` — reprendre après le traitement (voir
  [MANUEL.md](MANUEL.md) § erreurs).

## 3. Lire : `error` et `error$()`

| appel | rend |
|---|---|
| `error` | le **code** de la dernière erreur interceptée (3 = division, 22 = fichier impossible à ouvrir…) |
| `error$()` | le **message** de cette erreur |

Ces deux fonctions sont le contrat de lisibilité : toute erreur
trappable a un code stable (voir la table engendrée) et un texte. Le
code d'erreur d'un **module** dépasse 133 : l'espace `0x00080000` et
plus appartient aux modules, et le message est fourni par le module
lui-même (E46 fixe la règle : ce message arrive toujours, sur les deux
plateformes et les deux chemins).

## 4. La clé `raisematherror` — l'échappatoire

La configuration peut rendre le silence :

```
raisematherror 0        ' 0 : tout silencieux (comportement amont)
                        ' 1 (défaut) : la division entière par zéro lève
                        ' 2 : opérande undef dans une commande = erreur
                        ' 4 : opérande undef dans une expression = erreur
```

C'est un **masque de bits** : 1 = division par zéro, 2 = opérande
`undef` dans une commande, 4 = opérande `undef` dans une expression. Le
défaut du dépôt est **1** — les bits 2 et 4 restent opt-in parce que
l'idiome `undef` qui se propage est utilisé par les tests et par les
programmes existants.

## 5. Le piège E3 : le code de sortie ne dit rien

ScriptBasic rend **toujours 0**, même sur une erreur non interceptée :
le diagnostic part dans le rapporteur (`fichier(ligne): error &H…`),
jamais dans le code de sortie. Conséquences :

- un test ne peut pas se fier au code de sortie : il cherche `ECHEC` ou
  `error &H` dans la sortie ([E3](ERRATA.md#e3)) ;
- la campagne `--cgi-post` relit le code complet dans la page quand le
  code de sortie POSIX ne peut pas le porter (POSIX n'en transmet que
  8 bits — E46).

## 6. La position : `fichier(ligne)`

Le rapporteur précède chaque diagnostic du fichier et de la ligne :
`tests/trig.bas(57): error &H3:...`. C'est la base du débogage, et la
raison pour laquelle les contrats comparent la sortie octet par octet :
une erreur qui bouge de ligne se voit.

## 7. La table des codes (engendrée)

La liste complète — code, symbole(s) C, message — est engendrée depuis
`src/errcodes.h` et `src/errcodes.c` :
[ANNEXE-ERREURS.md](ANNEXE-ERREURS.md). En cas de divergence entre ce
document et la table, la table a raison : elle vient du code.
