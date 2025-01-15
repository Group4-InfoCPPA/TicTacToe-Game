#include "server_utils.h"


Game OnLineGame[MAX_WAITING_USER]; 
int waitingGame[MAX_WAITING_USER] = {0};
int freeId = 0;
int grid[3][3] = {  {0,0,0},
                    {0,1,0},
                    {0,0,0}
                };
int curr_play = 1;
int baseGame = 1517;

int main() {
    startSocket();

    socket_t server_sock = socket(FAMILY, SOCK_TYPE, 0);
    if (server_sock == inv_sock) {
        ErrorMess("Impossible de créer le socket.");
    }

    struct sockaddr_in server_addr = {
        .sin_family = FAMILY,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        ErrorMess("Échec du bind.");
    }

    if (listen(server_sock, MAX_WAITING_USER) < 0) {
        ErrorMess("Échec de l'écoute.");
    }

    printf("Serveur démarré sur le port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        socket_t *client_sock = malloc(sizeof(socket_t));
        *client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (*client_sock == inv_sock) {
            fprintf(stderr, "Échec de l'acceptation d'une connexion.\n");
            free(client_sock);
            continue;
        }

        pthread_t thread_id;
        int thread_out = pthread_create(&thread_id, NULL, handle_client, client_sock);
        if (thread_out != 0) {
            fprintf(stderr, "Erreur lors de la création du thread.\n");
            free(client_sock);
            continue;
        }

        pthread_detach(thread_id);
    }
    closesocket(server_sock);
    stopSocket();
    return 0;
}
