#include "socketUtilsv2.h"

typedef struct {
    socket_t socket;
    int *running; 
} ThreadArgs;


void *listenServer(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    char buffer[BUFFER_SIZE];
    int recvRet;

    while (*(args->running)) {
        recvRet = recv(args->socket, buffer, sizeof(buffer) - 1, 0);
        if (recvRet <= 0) {
            printf("\n[Erreur] Déconnexion du serveur ou erreur de réception.\n");
            *(args->running) = 0;
            break;
        }
        buffer[recvRet] = '\0'; 

        
        printf("\n[Autre] %s\n", buffer);


        printf("[Vous] "); 
        fflush(stdout); 
    }

    return NULL;
}

void *ClientCore(void *arg){
    
    startSockOnWindows();
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
        freeResourcesC(clientSocket);
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Échec de la connexion au serveur");
        freeResourcesC(clientSocket);
        exit(EXIT_FAILURE);
    }

    printf("Connecté au serveur!\n\n");
    printf("Instructions :\n- Tapez votre message et appuyez sur Entrée pour l'envoyer.\n");
    printf("- Tapez '<exit>' pour quitter.\n\n");

    int running = 1;
    pthread_t listenerThread;
    ThreadArgs args = {clientSocket, &running};

    
    if (pthread_create(&listenerThread, NULL, listenServer, &args) != 0) {
        perror("Erreur lors de la création du thread d'écoute");
        freeResourcesC(clientSocket);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    while (running) {
        printf("[Vous] ");
        fflush(stdout); 

        
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break; 
        }

        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        
        if (strcmp(buffer, "<exit>") == 0) {
            running = 0;
            break;
        }

        


        
        if (send(clientSocket, buffer, strlen(buffer), 0) <= 0) {
            printf("\n[Erreur] Impossible d'envoyer le message.\n");
            running = 0;
            break;
        }
    }

    printf("\nDéconnexion...\n");
    running = 0;
    pthread_join(listenerThread, NULL);

    freeResourcesC(clientSocket);
    return NULL;
}

int main() {
    pthread_t ClientThread;
    pthread_create(&ClientThread,NULL, ClientCore, NULL);
    pthread_join(ClientThread, NULL);
    return 0;
}
