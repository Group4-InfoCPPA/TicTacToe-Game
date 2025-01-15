#include "tictactoe.h"

void fillByEl(int n, int m, int tab[n][m], int el) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            tab[i][j] = el;
        }
    }
}

int state(int n, int tab[n][n]) {
    int movesPlayed = 0;
    int diago1Sum = 0, diago2Sum = 0;

    for (int i = 0; i < n; i++) {
        int rowSum = 0, colSum = 0;

        for (int j = 0; j < n; j++) {
            if (tab[i][j] != 0) movesPlayed++;

            rowSum += tab[i][j];
            colSum += tab[j][i];

            if (i == j) diago1Sum += tab[i][j];
            if (i == n - j - 1) diago2Sum += tab[i][j];
        }

        if (rowSum == n || colSum == n) return 1;
        if (rowSum == -n || colSum == -n) return -1;
    }

    if (diago1Sum == n || diago2Sum == n) return 1;
    if (diago1Sum == -n || diago2Sum == -n) return -1;

    return (movesPlayed == n * n) ? 0 : 2;
}

void copy_mat(int n, int m, int dest[n][m], int src[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

int HowGoodWasIt(int n, int grid[n][n], couple move, int player, int depth) {
    int copy_grid[n][n];
    copy_mat(n, n, copy_grid, grid);
    copy_grid[move.x][move.y] = player;

    int gameState = state(n, copy_grid);
    if (gameState == player) return -3 * VERY_BAD_SCORE;
    if (gameState == -player) return 3 * VERY_BAD_SCORE;
    if (gameState == 0) return 0;
    return -HowGoodWasIt(n, copy_grid, findTheBest(n, copy_grid, -player), -player, depth + 1);
}


couple findTheBest(int n, int grid[n][n], int player) {
    couple bestMove = {0, 0};
    int maxScore = VERY_BAD_SCORE * 10;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) {
                int score = HowGoodWasIt(n, grid, (couple){i, j}, player, 0);
                if (score > maxScore) {
                    maxScore = score;
                    bestMove = (couple){i, j};
                }
            }
        }
    }

    return bestMove;
}

int find_player(int n,int grid[][n]){
    int s=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            s += grid[i][j];
        }
    }
    return s==1?-1:1;
}
