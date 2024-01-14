//
// Created by Naku on 12/11/2023.
//
#include "include/game.h"
#include "include/interface.h"


int main(){

    srand(time(NULL));
    setConsoleToUTF8();
    // test the game

    while(true){
        launchMinesweeper();
    }

    return 0;
}