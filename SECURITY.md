# Sécurité — BasicScript

## Signaler une vulnérabilité

**Merci de ne pas ouvrir d'issue publique pour un problème de sécurité.**

Envoyez plutôt un rapport via le formulaire **« Signaler une vulnérabilité »** du dépôt GitHub (Security → Report a vulnerability), ou par tout canal privé indiqué sur la page du dépôt.

Un mainteneur accusa réception sous **72 h ouvrées** et tiendra le rapporteur informé de l'avancement.

## Ce qui nous intéresse

- Exécution de code arbitraire via des scripts malveillants (le langage est *by design* capable d'exécuter du code : seules les failles du **moteur** — dépassements de tampon, UAF, élévation de privilèges du processus hôte — sont concernées) ;
- Corruptions mémoire dans le lexer/parser/runtime (`src/`) ou dans les extensions portées ;
- Injection dans le serveur HTTP embarqué (s'il est réactivé en Phase 4) ;
- Failles dans la chaîne d'approvisionnement (build, paquets).

## Processus de réponse

1. **Réception & tri** (≤ 72 h) : confirmation, sévérité estimée (CVSS).
2. **Investigation** : reproductibilité, expositions concernées (versions, plateformes).
3. **Correctif** développé en privé ; identification des contributeurs concernés.
4. **Publication coordonnée** : correctif + advisory GitHub + entrée `SECURITY` dans CHANGELOG, avec crédit au rapporteur sauf refus.

## Politique de support

| Version | Supportée |
|---------|-----------|
| ≥ 0.x (portage C23/Zig, à venir) | ✅ |
| ScriptBasic 2.0 (`old/`, archive) | ❌ lecture seule, non patchée — utilisez-la comme référence uniquement |

## Recommandations aux déployeurs

- Ne jamais exposer le moteur à des scripts non fiables sans bac à sable (conteneur/seccomp) : BASIC permet E/S fichiers et appels système selon les modules chargés.
- Désactiver les extensions réseau/fichiers dont votre application n'a pas besoin.
- Compiler avec les protections modernes (`-D_FORTIFY_SOURCE=3`, ASan en test).
