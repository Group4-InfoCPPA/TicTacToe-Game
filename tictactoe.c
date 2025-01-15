#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define CLOSE_SOCKET closesocket
    #define SOCK_ERR SOCKET_ERROR
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <sys/socket.h> 
    typedef int socket_t;
    #define CLOSE_SOCKET close
    #define SOCK_ERR -1
#endif

#define SERVER_PORT 1111
#define BUFFER_SIZE 1024
#define GRID_SIZE 3


typedef struct {
    socket_t player1;
    socket_t player2;
    int board[GRID_SIZE][GRID_SIZE];
    int currentPlayer;
    int gameState;
} GameData;


void initializeBoard(int n, int board[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 0;  
        }
    }
}

bool isValidMove(int n, int board[n][n], int move) {
    int row = (move - 1) / n;
    int col = (move - 1) % n;
    return (move >= 1 && move <= n * n && board[row][col] == 0);
}

void applyMove(int n, int board[n][n], int move, int player) {
    int row = (move - 1) / n;
    int col = (move - 1) % n;
    board[row][col] = player;
}

int checkWinner(int n, int board[n][n]) {
    
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j < n; j++) {
            sum += board[i][j];
        }
        if (sum == n) return 1;   
        if (sum == -n) return -1; 
    }

    
    for (int j = 0; j < n; j++) {
        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += board[i][j];
        }
        if (sum == n) return 1;
        if (sum == -n) return -1;
    }

    
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += board[i][i];
    }
    if (sum == n) return 1;
    if (sum == -n) return -1;

    
    sum = 0;
    for (int i = 0; i < n; i++) {
        sum += board[i][n - 1 - i];
    }
    if (sum == n) return 1;
    if (sum == -n) return -1;

    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 0) return 2; 
        }
    }

    return 0; 
}

void sendToPlayer(socket_t client, int n, int board[n][n], int gameState, int turn) {
    send(client, (char *)board, n * n * (int)(sizeof(int)), 0);
    send(client, (char *)&gameState, sizeof(gameState), 0);
    send(client, (char *)&turn, sizeof(turn), 0);
}

void *playGame(void *arg) {
    GameData *game = (GameData *)arg;

    int move;

    while (game->gameState == 2) {
        
        sendToPlayer(game->player1, GRID_SIZE, game->board, game->gameState, game->currentPlayer == 1);
        sendToPlayer(game->player2, GRID_SIZE, game->board, game->gameState, game->currentPlayer == -1);

        
        socket_t currentSocket = (game->currentPlayer == 1) ? game->player1 : game->player2;
        int recvRet = recv(currentSocket, (char *)&move, sizeof(move), 0);
        if (recvRet <= 0) {
            perror("Échec de la réception du coup");
            break;
        }

        
        if (isValidMove(GRID_SIZE, game->board, move)) {
            applyMove(GRID_SIZE, game->board, move, game->currentPlayer);
            game->gameState = checkWinner(GRID_SIZE, game->board);
            game->currentPlayer = -game->currentPlayer;  
        } else {
            printf("Coup invalide reçu du joueur %d\n", (game->currentPlayer == 1) ? 1 : 2);
        }
    }

    
    const char *endMessage = (game->gameState == 1) ? "Le joueur 1 gagne !" :
                             (game->gameState == -1) ? "Le joueur 2 gagne !" :
                             "Match nul !";
    sendToPlayer(game->player1, GRID_SIZE, game->board, game->gameState, 0);  
    sendToPlayer(game->player2, GRID_SIZE, game->board, game->gameState, 0);  
    send(game->player1, endMessage, (int)strlen(endMessage), 0);
    send(game->player2, endMessage, (int)strlen(endMessage), 0);

    
    CLOSE_SOCKET(game->player1);
    CLOSE_SOCKET(game->player2);

    free(game);  
    return NULL;
}


int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Échec de l'initialisation des sockets.\n");
        exit(EXIT_FAILURE);
    }
#endif

    socket_t serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == SOCK_ERR) {
        perror("Échec de la création du socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCK_ERR) {
        perror("Échec de l'association du socket");
        CLOSE_SOCKET(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 2) == SOCK_ERR) {
        perror("Échec de l'écoute du socket");
        CLOSE_SOCKET(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("En attente de connexions des joueurs...\n");

    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    while (1) { 
        socket_t player1 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (player1 == SOCK_ERR) {
            perror("Échec de l'acceptation du joueur 1");
            continue;  
        }
        printf("Le joueur 1 est connecté !\n");
        socket_t player2 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (player2 == SOCK_ERR) {
            perror("Échec de l'acceptation du joueur 2");
            CLOSE_SOCKET(player1);
            continue;  
        }
        printf("Le joueur 2 est connecté !\n");
        GameData *game = malloc(sizeof(GameData));
        game->player1 = player1;
        game->player2 = player2;
        initializeBoard(GRID_SIZE, game->board);
        game->currentPlayer = 1;  
        game->gameState = 2;          
        pthread_t thread;
        if (pthread_create(&thread, NULL, playGame, (void *)game) != 0) {
            perror("Échec de la création du thread");
            CLOSE_SOCKET(player1);
            CLOSE_SOCKET(player2);
            free(game);
            continue;
        }   
        pthread_detach(thread);
        printf("Partie en cours entre les joueurs...\n");
    }
    CLOSE_SOCKET(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif

    printf("Le serveur est arrêté.\n");
    return 0;
}

