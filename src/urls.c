#include "urls.h"
#include "views.h"

ROUTE routes[] = {
    {"/", "GET", home_view, 0},
    {"/about", "GET", about_view, 1},
    {"/404", "GET", not_found_view, 2},
    {"/game", "GET", online_game_view, 3},
    {"/getGame", "POST", get_game_view, 4},  
    {"/sendMove", "POST",send_move_view, 5},
    {"/GetComputerMove", "POST", get_computer_move_view, 5}

};
int num_routes = sizeof(routes) / sizeof(ROUTE);

