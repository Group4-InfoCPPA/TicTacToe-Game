# Create a downloadable README.md file


# Projet Tic Tac Toe et Chat en Ligne

Ce projet implémente un jeu de **Tic Tac Toe** en ligne avec un serveur de chat. Le projet est divisé en quatre exécutables principaux :

- `chatServer` : Serveur de chat pour la communication entre les clients.
- `chatClient` : Client de chat pour se connecter au serveur de chat.
- `tictactoe` : Serveur pour gérer les jeux de Tic Tac Toe en ligne.
- `tictactoeClient` : Client pour jouer au Tic Tac Toe en ligne.

## Prérequis

Avant de commencer, assurez-vous que les éléments suivants sont installés sur votre système :

- **gcc** : Le compilateur C
- **Make** : Pour la gestion de la compilation
- **Libc** : Pour les fonctions standard C (normalement déjà installée)

## Compilation

Le projet utilise un `Makefile` pour faciliter la compilation.

1. Clonez le dépôt ou placez-vous dans le répertoire du projet.
2. Ouvrez un terminal dans ce répertoire.
3. Exécutez la commande suivante pour compiler le projet :

   ```bash
    gcc -o bin/chatServer chatServer.c -lws2_32
    gcc -o bin/chatClient chatClient.c -lws2_32
    gcc -o bin/tictactoe tictactoe.c -lws2_32
    gcc -o bin/tictactoeClient tictactoeClient.c -lws2_32
   ```
Cette commande génère les quatre exécutables suivants :

chatServer
chatClient
tictactoe
tictactoeClient
Utilisation
Chat
Lancer le serveur de chat :

Pour démarrer le serveur de chat, exécutez la commande suivante (depuis le bin):

```bash
    ./chatServer
```
Se connecter en tant que client :

Pour connecter un client au serveur de chat, lancez l'exécutable chatClient :

```bash
    ./chatClient
```
Tic Tac Toe
Lancer le serveur Tic Tac Toe :

Pour démarrer le serveur Tic Tac Toe, exécutez la commande suivante :

```bash
    ./tictactoe
```
Se connecter en tant que client :

Pour jouer en tant que client, exécutez l'exécutable tictactoeClient :

```bash
    ./tictactoeClient
```
Fonctionnalités
Chat : Communication en temps réel entre les clients via le serveur de chat.
Tic Tac Toe : Jeu de Tic Tac Toe en ligne avec deux joueurs, avec gestion de la grille et des coups via le réseau.


