#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define CLOSE_SOCKET closesocket
    #include <windows.h>  
#else
    #include <arpa/inet.h>
    #include <unistd.h>  
    typedef int socket_t;
    #define CLOSE_SOCKET close
#endif

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 1111
#define BUFFER_SIZE 1024

void printBoard(int n, int board[n][n]) {
    printf("\033[2J\033[H"); 

    for (int i = 0; i < n; i++) {
        printf("|");
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                printf(" X ");
            } else if (board[i][j] == -1) {
                printf(" O ");
            } else {
                printf("   ");  
            }
            printf("|");

        }
        
        printf("\n");
    }
}



int main() {
    
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        fprintf(stderr, "Initialisation du socket échouée.\n");
        exit(EXIT_FAILURE);
    }
#endif

    socket_t clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        perror("Échec de la création du socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr = {0};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("Adresse du serveur invalide");
        CLOSE_SOCKET(clientSocket);
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Échec de la connexion au serveur");
        CLOSE_SOCKET(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur!\n");

    int n = 3;  
    int board[n][n];
    int gameState = 2;  
    int myTurn = 0;     

    while (1) {
        
        int recvRet = recv(clientSocket, (char *)board, n * n * sizeof(int), 0);
        if (recvRet <= 0) {
            perror("Échec de la réception de la grille");
            break;
        }

        recvRet = recv(clientSocket, (char *)&gameState, sizeof(gameState), 0);
        if (recvRet <= 0) {
            perror("Échec de la réception de l'état du jeu");
            break;
        }

        recvRet = recv(clientSocket, (char *)&myTurn, sizeof(myTurn), 0);
        if (recvRet <= 0) {
            perror("Échec de la réception des informations sur le tour");
            break;
        }

        printBoard(n, board);  

        if (gameState != 2) {
            break;  
        }

        if (myTurn) {
            int move;
            printf("Votre tour. Entrez un numéro entre 1 et %d : ", n * n);
            scanf("%d", &move);

            
            while (move < 1 || move > n * n) {
                printf("Mouvement invalide. Réessayez : ");
                scanf("%d", &move);
            }

            
            int sendRet = send(clientSocket, (char *)&move, sizeof(move), 0);
            if (sendRet <= 0) {
                perror("Échec de l'envoi du coup");
                break;
            }
        } else {
            
            printf("En attente de l'autre joueur...\n");


#ifdef _WIN32
            Sleep(1000);  
#else
            sleep(1);  
#endif
        }
    }

    
    char endMessage[BUFFER_SIZE] = {0};
    int recvRet = recv(clientSocket, endMessage, sizeof(endMessage), 0);
    if (recvRet > 0) {
        printf("\nFin de la partie : %s\n", endMessage);
    } else {
        perror("Échec de la réception du message de fin");
    }

    CLOSE_SOCKET(clientSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
