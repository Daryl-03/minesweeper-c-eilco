#include "database_management.h"

void save_game(Game *game){
    FILE *file, *tmpFile;
    char *fileName =  game->over ? "games_completed": "games_non_completed", ch;
    int nbrLignes = 1;

    file = fopen(fileName, "a+");
    tmpFile = fopen("tmpFile", "a+");
    if(!file || !tmpFile) exit(EXIT_FAILURE);

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') nbrLignes++;
    }

    game->id = nbrLignes;
    fprintf(tmpFile, "{%d;%s;%d-%d;%d;%d;%d;%d;%d}", game->id, game->name, game->grid.size.width, game->grid.size.height, game->mines, game->flags, game->revealed, game->score, game->over);

    for(int i = 0; i < game->grid.size.width; i++){
        for(int j = 0; j < game->grid.size.height; j++){
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


