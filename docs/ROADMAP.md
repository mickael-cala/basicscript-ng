# ROADMAP — ce qui reste à faire, par ordre

Chaque tâche a un **critère d'acceptation** vérifiable : une commande et un
résultat attendu. Tant qu'il n'est pas rempli, la tâche n'est pas finie.

Légende : **P0** = à faire tout de suite, **P1** = ensuite, **P2**… = plus
tard, « dette » = connu et non planifié.

---

## P0 — Mettre le dépôt en service

- [ ] **Créer le dépôt distant et pousser `main`.**
      Aucun `remote` n'est configuré aujourd'hui.
      `git remote add origin git@github.com:<compte>/scriba-zig.git && git push -u origin main`
- [ ] **Vérifier la CI sur le dépôt distant** (`.github/workflows/build.yml`).
      Critère : les trois jobs (Windows, Ubuntu, macOS) sont verts.
- [ ] **Poser un tag** `v0.1.0` sur le premier commit publié.
- [ ] **Décider de la licence du portage** : `LICENSE` est la LGPL 2.1
      d'origine ; ajouter une note « modifications 2026 » si le fichier doit
      être modifié.

---

## P1 — Prouver les cibles qu'on ne peut pas exécuter ici

- [ ] **`macos-arm64` exécuté par la CI.** Le job macOS lance déjà
      `tests/run.py --target macos-arm64` : vérifier qu'il passe, sinon lire
      l'échec (c'est la seule exécution macOS possible).
- [ ] **`macos-x64`** : le runner `macos-latest` est ARM ; compiler pour x64
      est déjà fait, l'exécuter demanderait un runner Intel (disponible chez
      GitHub jusqu'à nouvel ordre) ou Rosetta.
- [ ] **`linux-arm64`** : ajouter `qemu-user-static` au job Ubuntu puis
      `qemu-aarch64 ./dist/linux-arm64/scriba tests/hello.bas`.
      Critère : le programme affiche son résultat.

---

## P2 — Modules

C'est le trou fonctionnel le plus visible : sans module, ni `re`, ni `hash`,
ni `cgi` en dehors du serveur, ni compression.

- [x] **Construire `hash`, `re`, `t`, `trial`, `cgi`, `mt`, `cio`, `win`,
      `unix`.** `build.py --modules` filtre par OS et produit les six cibles en
      `.dll`, `.so` ou `.dylib`, puis copie les déclarations dans
      `modules/include/`. Vérifié le 2026-10-23 : `tests/run.py --modules`
      rend 14/14 sous Windows x86 et x64, 13/13 sous Linux x64 ; voir
      [MODULES.md § 6](MODULES.md#6-construire-les-modules-aujourdhui).
- [x] **Ajouter ces tests au jeu**, avec `--modules` : `hash`, `re`, `md5`
      (module `t`), `trial`, `cgi`, `mt`, plus `cio` et `win` sous Windows,
      `unix` sous Linux/macOS. Le jeu par défaut reste sans module (sept
      scripts et un refus attendu), pour qu'un binaire sans modules se
      vérifie seul.
- [x] **Réimporter `re_engine`** : `tests/re_engine.c` (venu de
      `build_tcc/test/re_engine.c`) est construit par `--re-engine` et lancé
      par `tests/run.py --re-engine`, et la CI le construit et le lance pour
      chaque cible testée. Il vérifie `REG_BASIC`, `REG_PEND`, les
      références arrière, `cnames` et `findmust` : 17/17 sous Windows x86/x64
      et Linux x64 (WSL), verdict `moteur re : OK`.
- [x] **Brancher `tests/upstream/` sur le lanceur.** `tests/upstream/MANIFEST.tsv`
      décrit les 32 `.bas` ; `tests/run.py --upstream` lance 12 tests du
      coeur et des modules construits, et saute les 20 autres avec une raison
      (réseau, module absent, interaction, registre). `--upstream-all` ajoute
      `test_sleep`, `roundbugin1-0b27` et `seggfault` : 15 lancés, 17 sautés.
      `stresstest.bas` reste exclu : il dépasse la pile du parseur
      (0xC00000FD sous Windows x64) et n'est pas un test de non-régression.
- [x] **Réparer `hash::gethv`/`thishv`.** Les deux fonctions ne peuvent pas
      rendre une référence mortelle (`memory_NewMortalRef` est un bouchon
      volontaire). Elles rendent maintenant une copie ; `hash::Value` et
      `hash::ThisValue` remplacent `refValue`/`refThisValue`, et
      `examples/hash.bas` fonctionne. Vérifié par `tests/hash.bas`.
- [x] **Sortir `trial` des artefacts de production.** Le module reste dans
      `--modules` pour les tests, mais il est marqué `test_only` ;
      `--modules --release` le saute et retire un ancien binaire de `dist/`.
- [x] **Réduire les opérateurs étendus.** 257 symboles étaient déclarés,
      256 refusés comme opérateurs d'expression. Six affectations composées
      fonctionnaient déjà comme instructions (`+=`…), `#` est le tiret de
      fichier ; cinq opérateurs utiles ont
      été ajoutés (`<<`, `>>`, `??`, `**`, `//`) et les 245 autres retirés de
      `NASYMBOLS`, `BINARIES`, `UNARIES`, `extops.c` et `syntax.h`. Leur
      emploi est maintenant une erreur &H77 à l'analyse. Vérifié par
      `tests/operators.bas` ; détail dans [EVOLUTIONS](EVOLUTIONS.md).
- [ ] **`dbg`** n'est pas un module (préprocesseur externe) : décider si on le
      construit en `.dll` pour `scriba -p`, avec un test qui préprocesse un
      fichier. Aucune cible aujourd'hui.
- [ ] **Générateur de `include/error.bas`** (voir [ERRATA E1](ERRATA.md#e1)) :
      un script Python qui lit `src/errcodes.h` et écrit le `.bas`. Critère :
      régénérer le fichier ne produit aucun diff.
- [ ] **Convention de nommage des modules** : le projet d'origine livrait
      `hash.dll` en 32 bits et `hash64.dll` en 64 bits. Décider si le portage
      garde ce suffixe (le `-k` de `scriba` et le dossier `modules/` de la
      configuration sont les deux endroits à accorder).

---

## P3 — Serveur HTTP

- [x] **Porter `sbhttpd` sous Linux/macOS** ([ERRATA E7](ERRATA.md#e7)) :
      `<arpa/inet.h>` dans `httpd.c` et `websrv.c`, `stricmp`/`strnicmp`
      ramenés à `strcasecmp`/`strncasecmp`, et `static dummy()` remis en C23
      dans `service.c`. Vérifié le 2026-10-23 : `python build.py --target
      linux-x64 --httpd` réussit, et le serveur répond `sbhttpd OK` à
      `curl --http0.9 http://127.0.0.1:18080/index.bas` depuis WSL.
- [x] **Ajouter les modules à la CI** : `build.py --modules`,
      `tests/run.py --modules` et `tests/run.py --upstream` tournent sur les
      trois systèmes de `.github/workflows/build.yml`, ce qui exécute aussi
      `macos-arm64`.
- [ ] **Ajouter le serveur à la CI** (Windows et Linux) avec un petit test
      qui démarre, interroge, et arrête.
- [ ] **Vérifier le mode service Windows** (`sbhttpd -install`, `-remove`,
      `-start`). Jamais essayé ; la campagne TCC s'est arrêtée au mode
      console.
- [x] **Test multi-thread** : 20 requêtes simultanées lancées avec
      `xargs -P 10` contre `index.bas` ; les 20 réponses sont `sbhttpd OK`,
      sans erreur dans `logs/err.txt` ; 10 requêtes 404 parallèles rendent
      10 codes 404. Le serveur est configuré avec `threads 4`. Fait le
      2026-10-24 avec `dist/windows-x64/sbhttpd.exe`.
- [ ] **Chemins** : trois règles différentes cohabitent — sous Windows
      `scriba.conf` est cherché à côté de l'exécutable, sous UNIX il faut
      `SCRIBACONF` ou `/etc/scriba/basic.conf` ([E19](ERRATA.md#e19)), et
      `webtest/`, `logs/`, `modules/` sont relatifs au répertoire courant
      ([E15](ERRATA.md#e15)). Choisir une règle et l'appliquer partout.

---

## P4 — Documentation

- [x] **`pod2md.py`** : extrait les blocs `/*POD` des `interface.c` vers
      `docs/modules/<nom>.md` et les blocs `/**` des commandes vers
      `docs/MANUEL.md`. Critère tenu : les quatorze modules documentés ont leur
      page versionnée, dont `re` et `cgi` ; `python pod2md.py --check` échoue
      si un fichier a divergé des sources.
- [x] **Publier le manuel du langage** : [MANUEL.md](MANUEL.md), 212 entrées
      classées par section, engendré depuis le code depuis le 2026-10-24.
- [ ] **Ajouter un exemple commenté par module** dans `examples/`, en
      vérifiant qu'il tourne.

---

## P5 — Formes embarquées

Trois formes sont construites depuis le 2026-10-24 : `--standalone`,
`--stndlone` et `--libscriba`. Les formes inutilisables (`basicdll`,
`scribawx`) ont été retirées ; les autres restent des sources historiques
sans cible (voir
[PROGRAMMES.md § 2](PROGRAMMES.md#2-les-autres-programmes-et-formes-du-dépôt)).

- [x] **`--standalone`** : intégré au pilote avec `scriba -E`.
      `python build.py --target windows-x64 --standalone` produit
      `dist/windows-x64/hello_standalone.exe`, qui affiche le résultat de
      `tests/hello.bas`. Vérifié aussi sous Windows x86 et Linux x64 (WSL).
- [x] **`--stndlone`** : second chemin, `-C` + `stndlone.o` + coeur.
      `python build.py --target windows-x64 --stndlone` produit
      `hello_stndlone.exe`, vérifié sous Windows x86/x64 et Linux x64 ;
      `tests/run.py --stndlone` contrôle la sortie sans `.bas`.
- [x] **`libscriba`** : archive statique `libscriba.a` + `dist/<cible>/include/`,
      construite par `--libscriba` pour les six cibles (47 objets). Testée
      avec un hôte C sous Windows x64 et Linux x64 ; sous UNIX l'hôte doit
      définir `char **_environ;` pour ne pas tirer `libscriba.o` et son `main`.
- [x] **`basicdll.dll`** : retiré le 2026-10-24 avec son `.h` et
      `tools/calldllbasic.c` ; `basicdll.c` n'était qu'une déclaration, sans
      implémentation ([E21](ERRATA.md#e21)).
- [x] **`basicc` et `basiccmd`** : retirés le 2026-10-24 ; `stndlone` et
      `scribacmd` les remplacent.
- [x] **`scribawx`** : retiré le 2026-10-24 avec `lmt_wx.c` ; il demandait
      wxWidgets, absent du dépôt ([E22](ERRATA.md#e22)).
- [ ] **Vérifier `-E`, `-C` et `--libscriba` sous macOS** : Windows x86/x64 et
      Linux x64 sont vérifiés, macOS non.

---

## P6 — Le reste

- [x] **Nettoyer les doublons et alias du langage.** `elif`, `elsif` et
      `else if` retirés ; `ucase$` et `lcase$` retirés ; les 12 noms morts
      `mkd`/`mki`/`mks`/`mkl` (avec et sans `$`) et `cvd`/`cvi`/`cvs`/`cvl`
      retirés ; `atan` implémenté et `atn` conservé comme alias. `now`
      conservé comme seule forme, `time` retiré ; `kill` et `delete`
      conservés. Détail dans [EVOLUTIONS](EVOLUTIONS.md).
- [x] **Modules à bibliothèque tierce : non planifiés, `mysql` excepté.**
      Décision écrite dans [MODULES.md § 6.3](MODULES.md#63-décision--les-modules-à-bibliothèque-tierce).
      Aucun exemple ni test du dépôt n'utilise `psql`, `bdb`, `xml`, `curses`
      ou `zlib` ; `mysql` est la seule bibliothèque qui rend exécutable une
      application présente (SIBAWA). À rouvrir seulement si l'objectif devient
      le portage fidèle de la distribution.
- [x] **`odbc` en 64 bits** ([ERRATA E11](ERRATA.md#e11)) : corrigé le
      2026-10-26 (fusion, phase 2), construit et testé en 32 comme en 64
      bits sous Windows ; reste Linux, bloqué à la liaison (`E42`).
- [x] **`unix`** (ex-`ux`) : construit pour Linux/macOS avec `-include
      unistd.h -include sys/stat.h`, testé sous Linux x64 (`tests/unix.bas`).
      Reste à vérifier macOS par la CI, et `fork()` à la main (le test
      automatique n'appelle que `chmod`).
- [x] **Décider du sort de `dyc`** : il reste exclu des modules 64 bits. Son
      appel par pile est une ABI 32 bits ; ce n'est pas une correction de type.
      Les tests `dyc*.bas` restent hors du jeu par défaut.
- [ ] **`dyc` en 64 bits** : c'est un autre module (ABI d'appel différente),
      pas une correction.

---

## P7 — Exemple SIBAWA

- [ ] **Rendre SIBAWA exécutable** : `cgi` et `mt` sont désormais des modules
      chargeables et testés ; il reste `mysql` (voir § P6) et la base de
      données. Choisir ensuite pour ses gabarits : pré-générer
      `Templates/*.html` et les versionner, ou réécrire `mktm.cmd` en Python.
      Critère : l'application démarre sous `sbhttpd` sans Jamal ni Perl.

---

## Dette technique connue

| dette | où | pourquoi ce n'est pas fait |
|---|---|---|
| `include/error.bas` figé | [E1](ERRATA.md#e1) | générateur Perl disparu |
| `macos-*` non exécutés localement | [E5](ERRATA.md#e5) | machine Windows |
| 17 tests `upstream` hors jeu | [E4](ERRATA.md#e4) | réseau, modules absents, interaction, pile du parseur |
| gabarits SIBAWA en `.jam` | [README § 3.2](README.md#32-ce-qui-reste--2-fichiers-et-ce-quils-sont-vraiment) | l'exemple n'est pas exécutable aujourd'hui (modules absents) ; convertir n'aurait aucun usage |
