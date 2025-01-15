# Projet Tic Tac Toe et Chat en C (Version 1.0)

Ce projet regroupe deux fonctionnalités principales :
1. Un jeu de **Tic Tac Toe** (en local et client-serveur).
2. Une application de **chat** (client-serveur).

Les deux applications démontrent l'utilisation de sockets en C pour la communication réseau ainsi que la gestion des fonctionnalités interactives.

---

## Table des matières
1. [Structure du projet](#structure-du-projet)
2. [Prérequis](#prérequis)
3. [Compilation](#compilation)
4. [Exécution](#exécution)
5. [Nettoyage](#nettoyage)

---

## Structure du projet

Voici les fichiers principaux du projet :

```
├── chatServer.c         # Serveur pour l'application de chat.
├── chatClient.c         # Client pour l'application de chat.
├── tictactoe.c          # Jeu de Tic Tac Toe en local.
├── tictactoeClient.c    # Client réseau pour jouer au Tic Tac Toe à distance.
├── makefile             # Fichier pour automatiser la compilation.
└── bin/                 # Répertoire pour les exécutables générés.
```

---

## Prérequis

Avant de commencer, assurez-vous d'avoir :
- Un environnement Windows avec MinGW installé.
- Le compilateur `gcc` configuré.
- Le fichier `makefile` fourni avec le projet.

---

## Compilation

Pour compiler tous les exécutables, utilisez simplement la commande suivante dans votre terminal :

```bash
make
```

Cela générera les exécutables dans le dossier `bin`. Si le dossier `bin` n'existe pas, il sera automatiquement créé.

---

## Exécution

### Application de Chat

#### Lancer le serveur de chat :
```bash
./bin/chatServer.exe
```

#### Lancer un client de chat :
```bash
./bin/chatClient.exe
```

Répétez cette commande pour chaque client que vous souhaitez connecter au serveur.

### Jeu de Tic Tac Toe

#### Lancer le jeu en local :
```bash
./bin/tictactoe.exe
```

#### Lancer le serveur pour le Tic Tac Toe réseau :
```bash
./bin/tictactoe.exe
```

#### Lancer le client pour le Tic Tac Toe réseau :
```bash
./bin/tictactoeClient.exe
```

---

## Nettoyage

Pour supprimer les fichiers générés lors de la compilation, exécutez la commande suivante :

```bash
make clean
```

Cela supprimera les fichiers `.o` et les exécutables dans le dossier `bin`.

---

## Auteurs

- **Votre Nom** - Développeur principal

## Licence

Ce projet est fourni sous licence MIT. Vous êtes libre de le modifier et de le distribuer.
