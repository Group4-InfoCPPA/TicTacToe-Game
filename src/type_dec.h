#ifndef TYPE_DEC_H
    #define TYPE_DEC_H
    #include "const_dec.h"
    #include "dep_sock.h"
    typedef struct {
        char method[20];
        char url[MAX_LEN_FOR_URL];
        char json[MAX_FILE_CONTENT_SIZE];
    } REQUEST;

    typedef struct {
        int status;
        char message[MAX_PER_REQUEST];
        char *file;  
        size_t file_size;  
        char content_type[MAX_PER_REQUEST];
        char *body;
        char *content;
        size_t body_size;
        socket_t Client_sock;
        
    } RESPONSE;
    typedef struct {
                char url[MAX_LEN_FOR_URL];
                char method[20];
                void (*view_func)(RESPONSE *response);
                int id;
        } ROUTE;
    typedef struct{
        int id;
        int grid[3][3];
        int player1Id;
        int player2Id;
        int curr_play;
        int state;
    }Game;


    extern Game OnLineGame[MAX_WAITING_USER]; 
    extern int waitingGame[MAX_WAITING_USER];
    extern int freeId;
    extern int grid[3][3] ;
    extern int baseGame;

#endif // !