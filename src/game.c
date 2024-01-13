//
// Created by Naku on 12/11/2023.
//

#include "../include/game.h"
#include "../include/interface.h"
#include "../include/database_management.h"

void freeGameStructure(Game *game);

Cell createCell(int x, int y){
    Position position = {x, y};
    Cell cell = {
            .position = position,
            .flagged = false,
            .revealed = false,
            .value = 0
    };
    return cell;
}

Grid newGrid(Size size){
    Cell **cells = malloc(sizeof (Cell*)*size.height);

    for(int i=0; i<size.height; i++){
        cells[i] = malloc(sizeof (Cell)*size.width);

        // error handling
        if (cells[i] == NULL) {
            // Handle memory allocation failure
            fprintf(stderr, "Memory allocation failed during grid creation\n");
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < size.width; j++) {
            cells[i][j] = createCell(j, i);
        }
    }

    Grid grid = {
            .cells= cells,
            .size=size
    };

    return grid;
}

void initGame(Game *game, int x, int y){
    int guessingList[game->grid.size.width * game->grid.size.height];
    int range = game->grid.size.width * game->grid.size.height;
    // fill the guessing list
    for(int i=0; i<game->grid.size.width * game->grid.size.height; i++){
        guessingList[i] = i;
    }
    // remove the cell at the position of the first click
    guessingList[y*game->grid.size.width + x] = guessingList[game->grid.size.width * game->grid.size.height - 1];
    printf("%d %d\n", x, y);
    printf("%d %d\n", y*game->grid.size.width + x, game->grid.size.width * game->grid.size.height - 1);
    range--;
    // remove the surrounding cells
    range = removeSurroundingCells(game, x, y, guessingList, range);
//    generateMinesFromShuffling(game, guessingList);
    generateMinesFromRandomGuess(game, guessingList, range);

//    reveal the cell
    revealCell(game, x, y);
}

int removeSurroundingCells(const Game *game, int x, int y, int *guessingList, int range) {
    if(y - 1 >= 0){
        if(x-1 >= 0){
            guessingList[(y-1)*game->grid.size.width + x-1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
            range--;
        }
        guessingList[(y-1)*game->grid.size.width + x] = guessingList[game->grid.size.width * game->grid.size.height - 1];
        range--;
        if(x+1 < game->grid.size.width){
            guessingList[(y-1)*game->grid.size.width + x+1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
            range--;
        }
    }
    if(x-1 >= 0){
        guessingList[y*game->grid.size.width + x-1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
        range--;
    }
    if(x+1 < game->grid.size.width){
        guessingList[y*game->grid.size.width + x+1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
        range--;
    }
    if(y+1 < game->grid.size.height){
        if(x-1 >= 0){
            guessingList[(y+1)*game->grid.size.width + x-1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
            range--;
        }
        guessingList[(y+1)*game->grid.size.width + x] = guessingList[game->grid.size.width * game->grid.size.height - 1];
        range--;
        if(x+1 < game->grid.size.width){
            guessingList[(y+1)*game->grid.size.width + x+1] = guessingList[game->grid.size.width * game->grid.size.height - 1];
            range--;
        }
    }
    return range;
}

void generateMinesFromShuffling(Game *game, int *guessingList) {
    // shuffle the guessing list
    for(int i=0; i<game->grid.size.width * game->grid.size.height; i++){
        int j = rand() % (game->grid.size.width * game->grid.size.height);
        int temp = guessingList[i];
        guessingList[i] = guessingList[j];
        guessingList[j] = temp;
    }

    // fill the grid with mines
    for(int i=0; i<game->mines; i++){
        int haut = guessingList[i] / game->grid.size.width;
        int larg = guessingList[i] % game->grid.size.width;
        game->grid.cells[haut][larg].value = -1;
        addOneToCellsAroundMine(&game->grid, haut, larg);
    }
}

void generateMinesFromRandomGuess(Game *game, int *guessingList, int range) {

    for(int i=0; i<game->mines; i++){
        int j = rand() % range;
        int value = guessingList[j];
        int haut = value / game->grid.size.width;
        int larg = value % game->grid.size.width;
        game->grid.cells[haut][larg].value = -1;
        addOneToCellsAroundMine(&game->grid, haut, larg);

        // suppression de la valeur choisie et remplacement de la case
        guessingList[j] = guessingList[range-1];
        range--;
    }
}

void printGrid(Grid grid){
    for(int i=0; i<grid.size.height; i++){
        for(int j=0; j<grid.size.width; j++){
            if(grid.cells[i][j].value == -1)
                printf("X ");
            else if(grid.cells[i][j].value == 0)
                printf("  ");
            else
                printf("%d ", grid.cells[i][j].value);
        }
        printf("\n");
    }
}

void printGridFromPlayerPerspective(Grid grid){
    for(int i=0; i<grid.size.height; i++){
        for(int j=0; j<grid.size.width; j++){
            if(grid.cells[i][j].revealed){
                if(grid.cells[i][j].value == -1)
                    printf("X ");
                else if(grid.cells[i][j].value == 0)
                    printf("  ");
                else
                    printf("%d ", grid.cells[i][j].value);
            } else {
                printf("H ");
            }
        }
        printf("\n");
    }
}

Game* newGameGeneration(int id, char *name, Size size, int mines){
    Game *game = malloc(sizeof (Game));

    // error handling
    if (game == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Memory allocation failed during game creation\n");
        exit(EXIT_FAILURE);
    }

    game->id = id;
    game->name = name;
    game->grid = newGrid(size);
    game->mines = mines;
    game->flags = 0;
    game->revealed = 0;
    game->score = 0;
    game->over = false;

//    initGame(game);

    return game;
}

/**
 * Add one to the value of the cells around a mine
 * @param grid
 * @param haut
 * @param larg
 */
void addOneToCellsAroundMine(Grid* grid, int haut, int larg){
    // cells above the mine
    Cell **cells = grid->cells;
    Cell c = cells[haut][larg];
    if(haut-1 >= 0){
        if(larg-1 >= 0 && grid->cells[haut-1][larg-1].value != -1){
            cells[haut-1][larg-1].value++;
        }
        if(cells[haut-1][larg].value != -1){
            cells[haut-1][larg].value++;
        }
        if(larg+1 < grid->size.width && cells[haut-1][larg+1].value != -1){
            cells[haut-1][larg+1].value++;
        }
    }

//    cells on the same line as the mine
    if(larg-1 >= 0 && cells[haut][larg-1].value != -1){
        cells[haut][larg-1].value++;
    }

    if(larg+1 < grid->size.width && cells[haut][larg+1].value != -1){
        cells[haut][larg+1].value++;
    }

    // cells under the mine
    if(haut+1 < grid->size.height){
        if(larg-1 >= 0 && cells[haut+1][larg-1].value != -1){
            cells[haut+1][larg-1].value++;
        }
        if(cells[haut+1][larg].value != -1){
            cells[haut+1][larg].value++;
        }
        if(larg+1 < grid->size.width && cells[haut+1][larg+1].value != -1){
            cells[haut+1][larg+1].value++;
        }
    }
}

void revealCell(Game* game, int x, int y){
//    printf("Revealing cell at %d %d\n", x, y);
    game->grid.cells[y][x].revealed = true;

    int value = game->grid.cells[y][x].value;

    if(value == -1){
        printf("Game over !\n");
        game->over = true;
    } else if (value == 0){
        revealAdjacentCells(game, x, y);
    }

    game->revealed++;
}

void revealAdjacentCells(Game *game, int x, int y) {
//    if(game->grid.cells[y][x].revealed)
//        return;

    Cell** cells = game->grid.cells;

    if(y-1 >= 0){
        if(x-1 >= 0 && !cells[y-1][x-1].revealed){
            cells[y-1][x-1].revealed = true;
            game->revealed++;
            if(cells[y-1][x-1].value == 0){
                revealAdjacentCells(game, x-1, y-1);
            }
        }
        if(!cells[y-1][x].revealed){
            cells[y-1][x].revealed = true;
            game->revealed++;
            if(cells[y-1][x].value == 0){
                revealAdjacentCells(game, x, y-1);
            }
        }
        if(x+1 < game->grid.size.width && !cells[y-1][x+1].revealed){
            cells[y-1][x+1].revealed = true;
            game->revealed++;
            if(cells[y-1][x+1].value == 0){
                revealAdjacentCells(game, x+1, y-1);
            }
        }
    }

    if(x-1 >= 0 && !cells[y][x-1].revealed){
        cells[y][x-1].revealed = true;
        game->revealed++;
        if(cells[y][x-1].value == 0){
            revealAdjacentCells(game, x-1, y);
        }
    }
    if(x+1 < game->grid.size.width && !cells[y][x+1].revealed){
        cells[y][x+1].revealed = true;
        game->revealed++;
        if(cells[y][x+1].value == 0){
            revealAdjacentCells(game, x+1, y);
        }
    }

    if(y+1 < game->grid.size.height){
        if(x-1 >= 0 && !cells[y+1][x-1].revealed){
            cells[y+1][x-1].revealed = true;
            game->revealed++;
            if(cells[y+1][x-1].value == 0){
                revealAdjacentCells(game, x-1, y+1);
            }
        }
        if(!cells[y+1][x].revealed){
            cells[y+1][x].revealed = true;
            game->revealed++;
            if(cells[y+1][x].value == 0){
                revealAdjacentCells(game, x, y+1);
            }
        }
        if(x+1 < game->grid.size.width && !cells[y+1][x+1].revealed){
            cells[y+1][x+1].revealed = true;
            game->revealed++;
            if(cells[y+1][x+1].value == 0){
                revealAdjacentCells(game, x+1, y+1);
            }
        }
    }

}

void createAndPlayGame(){

    int mines = 0;
    Size size;
    getDifficultySettingsFromUser(&mines, &size);

    if(mines == 0){
        return;
    }

    Game *game = newGameGeneration(0, "", size, mines);
    bool init = false, play = true;
    InGameAction action;
    Position position = {0, 0};

    while(play){

        getActionFromUser(&(game->grid), &position, &action);

        // logs
//        printf("Action: %c\n", action);
//        printf("Position: %d %d\n", position.x, position.y);


        if(!init){
            initGame(game, position.x, position.y);
            printf("Position init: %d %d\n", position.x, position.y);
            init = true;
        } else {
            switch (action) {
                case REVEAL:
                    revealCell(game, position.x, position.y);
                    if(game->over){
                        printf("Game over!\n");
                        play = false;
                    } else if(isGameWon(game)){
                        handleGameWon(game);
                    }
                    break;
                case FLAG:
                    game->grid.cells[position.y][position.x].flagged = true;
                    break;
                case UNFLAG:
                    game->grid.cells[position.y][position.x].flagged = false;
                    break;
                case SAVE:
                    save_game(game);
                    break;
                case QUIT:
                    play = false;
                    break;
                default:
                    break;
            }
        }
    }

    freeGameStructure(game);
    game = NULL;
}

void freeGameStructure(Game *game) {
    if(game == NULL) {
        fprintf(stderr, "Attempted to free a game with a NULL value\n");
    }


    if (game->grid.cells != NULL){
        for (int i = 0; i < game->grid.size.height; i++) {
            if (game->grid.cells[i] != NULL){
                free(game->grid.cells[i]);
            }
        }
        free(game->grid.cells);
    }
    free(game);
}

void handleGameWon(Game *game){
    clearScreen();
    revealAllCells(game);
    afficherGrille(&game->grid);
    printf("You won!\n");
}

bool isGameWon(const Game *game) {
    return game->revealed == game->grid.size.width * game->grid.size.height - game->mines;
}

void revealAllCells(Game *game) {
    for(int i=0; i<game->grid.size.height; i++){
        for(int j=0; j<game->grid.size.width; j++){
            game->grid.cells[i][j].revealed = true;
        }
    }
}

void launchMinesweeper(){

    clearScreen();
    menu();

}
