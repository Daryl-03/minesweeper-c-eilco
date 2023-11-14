//
// Created by Naku on 12/11/2023.
//
#include "include/game.h"


int main(){
    srand(time(NULL));
    Size size = HARD_SIZE;
    Game *game = newGame(1, "test", size, HARD_MINES);

    printGrid(game->grid);
    return 0;
}