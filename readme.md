
# HTTP Server en C avec Tic Tac Toe 🎮

Bienvenue dans **HTTP Server en C**, un projet développé entièrement en langage C sans aucune bibliothèque externe, offrant une plateforme fluide et conviviale pour jouer au jeu classique **Tic Tac Toe** ! 🚀

---

## 🛠 Fonctionnalités principales

- **Serveur HTTP minimaliste :**
  - Développé sans bibliothèque externe, basé uniquement sur les fonctionnalités natives de C.
  - Support des requêtes HTTP basiques pour servir les pages web et interagir avec le jeu.

- **Tic Tac Toe fluide et amusant :**
  - **Mode Solo** : Affrontez une intelligence artificielle directement depuis la page d'accueil.
  - **Mode Multijoueur en ligne** : Jouez contre d'autres utilisateurs en temps réel via le réseau.
  - Interface intuitive et facile à prendre en main.

- **Architecture légère et performante :**
  - Optimisé pour une exécution rapide sur n'importe quelle machine.
  - Code simple et réutilisable pour les développeurs souhaitant s'inspirer ou étendre ce projet.

---

## 📂 Structure du projet

Voici une vue d'ensemble de la structure du projet :

```
/
├── src/                    # Code source du projet
│   ├── base_utils.c        # Fonctions utilitaires générales
│   ├── server.c            # Code principal du serveur HTTP
│   ├── server_utils.c      # Gestion des requêtes/réponses HTTP
│   ├── tictactoe.c         # Logique du jeu Tic Tac Toe
│   ├── urls.c              # Routes HTTP et gestion des endpoints
│   ├── views.c             # Contenus HTML et affichages dynamiques
├── bin/                    # Fichiers compilés et exécutable final
│   ├── *.o                 # Fichiers objets
│   ├── testServer.exe      # Exécutable principal
├── Makefile                # Script de compilation
├── README.md               # Documentation du projet
```

---

## 🚀 Installation et exécution

Suivez ces étapes pour installer et exécuter le projet :

### 1. Pré-requis

- Un environnement de développement avec **`gcc`** installé.  
  - **Windows :** Utilisez [MinGW](https://www.mingw-w64.org/).  
  - **Linux / macOS :** `gcc` est généralement préinstallé ou peut être installé via le gestionnaire de paquets.

### 2. Cloner le projet

Clonez le dépôt Git et accédez au répertoire du projet :

```bash
git clone https://github.com/Group4-InfoCPPA/TicTacToe-Game.git
cd TicTacToe-Game
```

### 3. Compiler le projet

Compilez les fichiers source avec la commande suivante (le fichier `Makefile` s'occupe de tout) :

```bash
make
```

### 4. Exécuter le serveur

Démarrez le serveur avec :

```bash
make run
```

Par défaut, le serveur écoute sur le port **8080**.

### 5. Accéder au jeu

- Ouvrez un navigateur web.
- Rendez-vous sur [http://localhost:1111](http://localhost:1111) pour accéder à l'interface du jeu.

---

## 🕹 Comment jouer ?

### Sur la page d'accueil

- **Mode Solo :** Jouez contre une intelligence artificielle (IA).  
  - Cliquez sur une case pour effectuer votre mouvement.
  - La partie se termine lorsqu'il y a un gagnant ou un match nul.

### Mode en ligne

- Accédez à l'onglet **Play Online**.  
- Attendez qu'un adversaire se connecte ou rejoignez une partie déjà disponible.  
- La grille est synchronisée en temps réel, permettant un jeu fluide entre deux joueurs.

---

## 📜 Détails techniques

- **Langage utilisé :** C
- **Protocole réseau :** HTTP implémenté via **Winsock2** (pour Windows).
- **Mode en ligne :**
  - Les joueurs communiquent via des requêtes HTTP.
  - Le serveur maintient un état global de la grille et synchronise les joueurs.

---

## 💡 Idées futures

- Intégrer un système de chat entre les joueurs.
- Ajouter un mode tournoi avec classement.
- Étendre le jeu à des grilles personnalisées (par ex. 4x4, 5x5).

---

## 🖋 Auteur

Projet réalisé par **le Groupe 7**.  

---

## 📄 Licence

Ce projet est sous licence **MIT** – vous êtes libre de l'utiliser, de le modifier et de le partager.