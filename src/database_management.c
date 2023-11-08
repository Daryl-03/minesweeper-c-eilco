#include "database_management.h"

#define file_games_completed "games_completed.txt"
#define file_games_non_completed "games_non_completed.txt"

void save_game(Game *game){
    FILE *file, *tmpFile;
    char *fileName = game->over ? file_games_completed: file_games_non_completed, ch;
    int nbrLignes = 1;

    file = fopen(fileName, "a+");
    tmpFile = fopen("tmpFile", "a+");
    if(!file || !tmpFile) exit(EXIT_FAILURE);

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') nbrLignes++;
    }

    game->id = nbrLignes;
    fprintf(tmpFile, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", game->id, game->name, game->grid.size.width, game->grid.size.height, game->mines, game->flags, game->revealed, game->score, game->over);

    for(int i = 0; i < game->grid.size.height; i++){
        for(int j = 0; j < game->grid.size.width; j++){
            fprintf(tmpFile, "-{%d-%d;%d;%d;%d}", game->grid.grid[i][j].position.x, game->grid.grid[i][j].position.y, game->grid.grid[i][j].revealed, game->grid.grid[i][j].value, game->grid.grid[i][j].flagged);
        }
    }
    fprintf(tmpFile, "\n");

    rewind(file);
    while ((ch = fgetc(file)) != EOF) {
        fputc(ch, tmpFile);
    }
    fclose(file);
    fclose(tmpFile);

    remove(fileName);
    rename("tmpFile", fileName);
}

Game *load_game(int id){
    FILE *file = NULL;
    file = fopen(file_games_non_completed, "r");
    int over;
    char ch = '1';
    Game *game = malloc(sizeof(Game));
    if(!game) exit(EXIT_FAILURE);

    while (ch != EOF){
        fscanf(file, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", &game->id, game->name, &game->grid.size.width, &game->grid.size.height, &game->mines, &game->flags, &game->revealed, &game->score, &over);
        game->over = over;
        if(game->id == id){
            game->grid.grid = (Cell**) malloc(game->grid.size.height * sizeof(Cell*));
            if(!game->grid.grid) exit(EXIT_FAILURE);

            for(int i = 0; i < game->grid.size.height; i++){
                game->grid.grid[i] = (Cell*) malloc(game->grid.size.width * sizeof(Cell));
                for(int j = 0, revealed, flagged; j < game->grid.size.width; j++){
                    fscanf(file, "-{%d-%d;%d;%d;%d}", &game->grid.grid[i][j].position.x, &game->grid.grid[i][j].position.y, &revealed, &game->grid.grid[i][j].value, &flagged);
                    game->grid.grid[i][j].revealed = revealed;
                    game->grid.grid[i][j].flagged = flagged;
                }
            }
            return game;
        }
        while((ch = fgetc(file))!= EOF && ch != '\n');
    }
    fclose(file);
    return NULL;
}
