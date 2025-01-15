#ifndef TICTACTOE
    #define TICTACTOE
    #define VERY_BAD_SCORE (-5)
    typedef struct {
            int x;
            int y;
        } couple;
    int find_player(int n,int grid[][n]);
    void fillByEl(int n, int m, int tab[n][m], int el);
    int state(int n, int tab[n][n]);
    couple findTheBest(int n, int grid[n][n], int player);
    int HowGoodWasIt(int n, int grid[n][n], couple move, int player, int depth) ;
    void copy_mat(int n, int m, int dest[n][m], int src[n][m]);

#endif