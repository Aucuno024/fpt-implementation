# FTP SR – Implémentation C (Master/Slave)

Implémentation d’un protocole FTP simplifié en C réalisée pour le cours **Systèmes & Réseaux**.

Le projet inclut :
- un **client interactif** ;
- un **serveur maître** (répartition de charge) ;
- un ou plusieurs **serveurs esclaves** (transferts et opérations fichiers) ;
- une **reprise de téléchargement** côté client (`.part` / `.part.meta`) ;
- une **suite de tests unitaires** (CuTest).

## Fonctionnalités

- Transferts de fichiers en blocs (`GET`, `PUT`) avec en-tête de transfert.
- Reprise de téléchargement via la commande interne `RESUME` (automatique côté client).
- Commandes distantes : `ls`, `rm`, `bye`.
- Répartition de charge par le maître (`master_dns`) vers des esclaves FTP.
- Réplication « faiblement cohérente » des opérations `RM` / `PUT` entre esclaves via `UPDATE`.
- Gestion explicite des erreurs de protocole (types, chemins, déconnexion client, etc.).

## Prérequis

- Linux (ou environnement POSIX compatible)
- `gcc`
- `make`
- Bibliothèque `libcrypt` (liée via `-lcrypt`)

## Compilation

Compilation standard :

```bash
make
```

Binaries générés dans `bin/` :
- `bin/client`
- `bin/master_dns`
- `bin/serveur_ftp`
- `bin/test`

Compilation en mode debug :

```bash
make DEBUG=1
```

Nettoyage :

```bash
make clean
```

## Variables de build utiles

Le `Makefile` accepte des paramètres pour adapter l’exécution :

- `DEFAULT_CLIENT_DIR` : dossier client local (défaut : `clientdir`)
- `DEFAULT_SERVER_DIR` : dossier serveur local (défaut : `serverdir`)
- `SLAVE_PORT` : port d’écoute des serveurs esclaves (défaut : `2121`)
- `PASSWORD` : hash `crypt(3)` attendu par le maître
- `DELAY` : délai artificiel entre blocs (utile pour tester la reprise)

Exemple :

```bash
make DEBUG=1 DEFAULT_CLIENT_DIR='"./clientdir/"' DEFAULT_SERVER_DIR='"./serverdir/"' SLAVE_PORT=2121
```

## Configuration réseau

Deux fichiers de configuration sont utilisés :

- `config_master` : liste des IP des esclaves (une IP par ligne), lue par `master_dns`
- `config_slave` : IP du maître, lue par `serveur_ftp`

Pour un test local minimal, utilisez `127.0.0.1` dans les deux fichiers.

## Lancer l’application

### 1) Démarrer le maître

```bash
./bin/master_dns
```

Le maître écoute :
- les clients sur `2121`
- les esclaves sur `2222` (canal de cohérence)

### 2) Démarrer un ou plusieurs esclaves

```bash
./bin/serveur_ftp
```

Chaque esclave lit `config_slave`, puis écoute les clients sur `SLAVE_PORT`.

### 3) Démarrer le client

```bash
./bin/client <ip_ou_nom_du_master>
```

Le client :
1. se connecte au maître ;
2. envoie le mot de passe ;
3. reçoit l’IP/port d’un esclave ;
4. ouvre une session FTP avec cet esclave.

## Commandes client

Dans l’invite `ftp>` :

- `get <chemin>` : télécharge un fichier depuis le serveur
- `put <chemin>` : envoie un fichier local vers le serveur
- `ls <chemin>` : liste le contenu d’un dossier serveur
- `rm <chemin>` : supprime un fichier côté serveur
- `bye` : ferme la connexion proprement

Exemples :

```text
ftp> ls .
ftp> get test/client.c
ftp> put dos/fichier2
ftp> rm obsolete.txt
ftp> bye
```

## Reprise de téléchargement

Lors d’un `get`, le client écrit :
- un fichier partiel `<fichier>.part`
- des métadonnées `<fichier>.part.meta`

Au redémarrage du client, les téléchargements interrompus dans le dossier client sont repris automatiquement.

## Tests

Le binaire de tests unitaires est compilé avec `make` :

```bash
./bin/test
```

Les tests actuels couvrent surtout des éléments de structure interne (journal/log).

## Arborescence (principale)

- `src/` : code source applicatif
- `include/` : headers
- `src_test/`, `include_test/` : tests CuTest
- `clientdir/` : stockage côté client
- `serverdir/` : stockage côté serveur
- `config_master`, `config_slave` : configuration IP

## Rapport PDF

Le script `compile_compte_rendu.sh` génère `compte_rendu.pdf` à partir du template LaTeX.

```bash
./compile_compte_rendu.sh "Auteur 1" "Auteur 2"
```

Prérequis : `pdflatex` (TeX Live).

## Limitations connues

- IPv4 uniquement.
- Les options de sécurité (authentification fine pour `rm`/`put`, chiffrement) sont partielles/non finalisées.
- La robustesse en cas de panne d’esclave peut être améliorée (reconnexion/failover).

## Licence

Voir `LICENSE`.
