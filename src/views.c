#include "views.h"
#include "server_utils.h"

void home_view(RESPONSE *response) {
    response->file = render("home.html");
    response->file_size = strlen(response->file);
}

void online_game_view(RESPONSE *response){
    int playerId = baseGame;
    baseGame++;
    char str_id[15] = {0};
    char str_gameId[15] = {0};
    snprintf(str_id, 15, "%d", playerId);
    int gameId = freeId;
    snprintf(str_gameId, 15, "%d", gameId);

    response->file = render("game.html");
    response->file = replace(response->file, "{{userId}}", str_id);
    
    response->file_size = strlen(response->file);   
    for (int i =0;i<MAX_WAITING_USER;i++){
        if(waitingGame[i]==1){
            OnLineGame[i].player2Id = playerId;
            int gameId = i;
            str_gameId[0] = 0;
            snprintf(str_gameId, 15, "%d", gameId);
            waitingGame[i] = 2;
            response->file = replace(response->file, "{{gameId}}", str_gameId);
            printf("player1: %d --(%d)-- player2:%d \n", OnLineGame[i].player1Id, gameId,OnLineGame[i].player2Id);
            return;
        }
        
    } 
    for (int i =0;i<MAX_WAITING_USER;i++){
        if(waitingGame[i]==0){
            OnLineGame[i].player1Id = playerId;
            OnLineGame[i].curr_play = playerId;
            fillByEl(3,3,OnLineGame[freeId].grid,0);
            int gameId = i;
            str_gameId[0] = 0;
            snprintf(str_gameId, 15, "%d", gameId);
            response->file = replace(response->file, "{{gameId}}", str_gameId);
            waitingGame[i] = 1;
            return;
        }
        
    } 
    
}

void about_view(RESPONSE *response) {
    response->file = render("about.html");
    response->file_size = strlen(response->file);
}

void not_found_view(RESPONSE *response) {
    response->file = render("404.html");
    response->file_size = strlen(response->file);
    strcpy(response->content_type, "text/html; charset=utf-8");
}




void get_computer_move_view(RESPONSE *response) {
    response->body = (char *)malloc(MAX_PER_REQUEST*2 * sizeof(char));
    int grid[3][3] = {0};
    int parsed = sscanf(response->content,
        "{\"grid\": [[%d, %d, %d], [%d, %d, %d], [%d, %d, %d]]}",
        &grid[0][0], &grid[0][1], &grid[0][2],
        &grid[1][0], &grid[1][1], &grid[1][2],
        &grid[2][0], &grid[2][1], &grid[2][2]
    );

    if (parsed != 9) {
        snprintf(response->body, MAX_PER_REQUEST*2, "{\"error\":\"Invalid grid format\"}");
        response->status = 400;
        return;
    }

    if (state(3, grid) != 2) {
        snprintf(response->body, MAX_PER_REQUEST*2, "{\"error\":\"Game is already over\"}");
        response->body_size = strlen(response->body)+1;
        response->status = 400;
        return;
    }

    int player = find_player(3, grid);

    couple best = findTheBest(3, grid, player);

    snprintf(response->body, MAX_PER_REQUEST*2, "{\"i\":%d,\"j\":%d,\"player\":%d}", best.x, best.y, player);
    response->status = 200;
    response->body_size = strlen(response->body);

}
void get_game_view(RESPONSE *response) {
    struct {int userId;int gameId;}gameInfo;
    int status;
    response->body = (char *)malloc(MAX_PER_REQUEST * sizeof(char));
    if (response->body == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire pour la réponse du jeu.\n");
        response->status = 500;
        strcpy(response->message, "Internal Server Error");
        response->file = render("500.html");
        response->file_size = strlen(response->file);
        return;
    }
    sscanf(response->content,"{\"userId\":%d ,\"gameId\":%d }", &gameInfo.userId, &gameInfo.gameId);
    status = state(3, OnLineGame[gameInfo.gameId].grid);
    OnLineGame[gameInfo.gameId].state = status;
    snprintf(response->body, MAX_PER_REQUEST,
             "{\"grid\": [[%d, %d, %d], [%d, %d, %d], [%d, %d, %d]], \"player\": %d, \"score\":%d}",
             OnLineGame[gameInfo.gameId].grid[0][0], OnLineGame[gameInfo.gameId].grid[0][1], OnLineGame[gameInfo.gameId].grid[0][2],
             OnLineGame[gameInfo.gameId].grid[1][0], OnLineGame[gameInfo.gameId].grid[1][1], OnLineGame[gameInfo.gameId].grid[1][2],
             OnLineGame[gameInfo.gameId].grid[2][0], OnLineGame[gameInfo.gameId].grid[2][1], OnLineGame[gameInfo.gameId].grid[2][2],
             OnLineGame[gameInfo.gameId].curr_play, status==1 ? (OnLineGame[gameInfo.gameId].player1Id): status==-1 ? (OnLineGame[gameInfo.gameId].player2Id):status);

    
    response->body_size = strlen(response->body);

    
    strcpy(response->content_type, "application/json");
    response->status = 200;
    strcpy(response->message, "OK");
}

void send_move_view(RESPONSE *response){
    struct {int i; int j; int userId;int gameId; } MoveInfo;
    sscanf(response->content,"{\"i\":%d,\"j\":%d,\"userId\":%d,\"gameId\":%d}", &MoveInfo.i, &MoveInfo.j, &MoveInfo.userId, &MoveInfo.gameId);
    Game *game = &OnLineGame[MoveInfo.gameId];
    // printf("userId: %d, player1Id:%d, player2Id:%d\n",MoveInfo.userId, game->player1Id, game->player2Id);
    // if(game->state!=2) return;

    if(!(MoveInfo.i>=0 && MoveInfo.i<3 && MoveInfo.j>=0 && MoveInfo.j<3 && game->grid[MoveInfo.i][MoveInfo.j]==0)){
        return;
    }
    if(MoveInfo.userId == game->player1Id && game->player1Id == game->curr_play){
        game->grid[MoveInfo.i][MoveInfo.j] = 1;
        game->curr_play = game->player2Id;
    }    
    else if(MoveInfo.userId == game->player2Id && game->player2Id == game->curr_play){
        game->grid[MoveInfo.i][MoveInfo.j] = -1;
        game->curr_play = game->player1Id;

    }    
    game->state = state(3, game->grid);
    if(game->state!=2){
        printf("End:player1:%d (%d:%d) player2:%d\n ", game->player1Id, MoveInfo.gameId,game->state,game->player2Id);
        waitingGame[MoveInfo.gameId] = 0;
    }    
    return;    
}