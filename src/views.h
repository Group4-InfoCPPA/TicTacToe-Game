#ifndef VIEW
    #define VIEW
    #include "server_utils.h"
    #include "urls.h"
    #include "tictactoe.h"
    #include "base_utils.h"
    
    void home_view(RESPONSE *response);
    void about_view(RESPONSE *response);
    void online_game_view(RESPONSE *response);
    void not_found_view(RESPONSE *response);

    void get_game_view(RESPONSE *response);
    void send_move_view(RESPONSE *response);
    void get_computer_move_view(RESPONSE *response);

#endif