//
// Created by Naku on 12/11/2023.
//
#include "include/game.h"
#include "include/interface.h"


int main(){
    srand(time(NULL));
//    Size size = EASY_SIZE;
//    Game *game = newGameGeneration(1, "test", size, EASY_MINES);
//
//    // print the grid
//    printGridFromPlayerPerspective(game->grid);
//    int x, y;
//
//    // init the game
//    printf("Enter the x coordinate: ");
//    scanf("%d\n", &x);
//    printf("Enter the y coordinate: ");
//    scanf("%d", &y);
//
//    initGame(game, x, y);
//    printGrid(game->grid);
//    printGridFromPlayerPerspective(game->grid);
//
//    while (!game->over){
//
//        printf("\n\n");
//        printf("Enter the x coordinate: ");
//        scanf("%d", &x);
//        printf("Enter the y coordinate: ");
//        scanf("%d", &y);
//        printf("\n\n");
//        revealCell(game, x, y);
//
//        if(game->over){
//            printf("Game over!\n");
//            break;
//        }
//
//        printGridFromPlayerPerspective(game->grid);
//    }
    setConsoleToUTF8();
    // test the game
    launchMinesweeper();
//    printf("%d", REVEAL=='r');
    return 0;
}