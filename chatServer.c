#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    typedef SOCKET socket_t;
    #define CLOSE_SOCKET closesocket
    #define SOCK_ERR INVALID_SOCKET
#else
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int socket_t;
    #define CLOSE_SOCKET close
    #define SOCK_ERR -1
#endif

#define SERVER_PORT 1112
#define BUFFER_SIZE 1024

typedef struct {
    socket_t client1;
    socket_t client2;
} ClientPair;


void sendMessage(socket_t client, const char *message) {
    send(client, message, strlen(message), 0);
}


void *handleClient1(void *arg) {
    ClientPair *pair = (ClientPair *)arg;
    char buffer[BUFFER_SIZE];
    int recvRet;

    while (1) {
        
        recvRet = recv(pair->client1, buffer, sizeof(buffer), 0);
        if (recvRet <= 0) {
            printf("Client 1 déconnecté.\n");
            break;
        }
        buffer[recvRet] = '\0';
        printf("Client 1: %s\n", buffer);

        
        sendMessage(pair->client2, buffer);
    }

    CLOSE_SOCKET(pair->client1);
    return NULL;
}


void *handleClient2(void *arg) {
    ClientPair *pair = (ClientPair *)arg;
    char buffer[BUFFER_SIZE];
    int recvRet;

    while (1) {
        
        recvRet = recv(pair->client2, buffer, sizeof(buffer), 0);
        if (recvRet <= 0) {
            printf("Client 2 déconnecté.\n");
            break;
        }
        buffer[recvRet] = '\0';
        printf("Client 2: %s\n", buffer);

        
        sendMessage(pair->client1, buffer);
    }

    CLOSE_SOCKET(pair->client2);
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

    if (listen(serverSocket, 10) == SOCK_ERR) {
        perror("Échec de l'écoute du socket");
        CLOSE_SOCKET(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Le serveur est en attente de connexions...\n");

    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    while (1) {
        socket_t client1 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (client1 == SOCK_ERR) {
            perror("Échec de l'acceptation du client 1");
            continue;  
        }
        printf("Client 1 connecté.\n");

        socket_t client2 = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (client2 == SOCK_ERR) {
            perror("Échec de l'acceptation du client 2");
            CLOSE_SOCKET(client1);
            continue;  
        }
        printf("Client 2 connecté.\n");

        
        ClientPair *pair = malloc(sizeof(ClientPair));
        pair->client1 = client1;
        pair->client2 = client2;

        
        pthread_t thread1;
        if (pthread_create(&thread1, NULL, handleClient1, (void *)pair) != 0) {
            perror("Échec de la création du thread pour client 1");
            CLOSE_SOCKET(client1);
            CLOSE_SOCKET(client2);
            free(pair);
            continue;
        }

        
        pthread_t thread2;
        if (pthread_create(&thread2, NULL, handleClient2, (void *)pair) != 0) {
            perror("Échec de la création du thread pour client 2");
            CLOSE_SOCKET(client1);
            CLOSE_SOCKET(client2);
            free(pair);
            continue;
        }

        pthread_detach(thread1);  
        pthread_detach(thread2);  

        printf("Les clients peuvent maintenant discuter.\n");
    }

    CLOSE_SOCKET(serverSocket);

#ifdef _WIN32
    WSACleanup();
#endif

    printf("Le serveur est arrêté.\n");
    return 0;
}
