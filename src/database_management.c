#include "database_management.h"

#define file_games_completed "games_completed.txt"
#define file_games_non_completed "games_non_completed.txt"


void save_game(Game *game) {
    FILE *file, *tmpFile;
    char *fileName = game->over ? file_games_completed : file_games_non_completed, chaine[500], ch;
    int nbrLignes = 1;

    file = fopen(fileName, "a+");
    tmpFile = fopen("tmpFile", "a+");
    if (!file) {
        printf("Error on file %s: %s", fileName, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (!tmpFile) {
        printf("Error on file tmpfile: %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // count the number of lines in the files
    while ((ch = (char) fgetc(file)) != EOF) {
        if (ch == '\n') nbrLignes++;
    }

    nbrLignes = nbrLignes >= 10 ? 9 : nbrLignes;
    game->id = nbrLignes--; // attribution of game id

    // removing spaces
    for (int i = 0; i < strlen(game->name); i++)
        if (game->name[i] == ' ') game->name[i] = '|';

    // writing game's characteristic
    fprintf(tmpFile, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", game->id, game->name, game->grid.size.width,
            game->grid.size.height, game->mines, game->flags, game->revealed, game->score, game->over);

    // writing the characteristics of the game cells
    for (int i = 0; i < game->grid.size.height; i++) {
        for (int j = 0; j < game->grid.size.width; j++) {
            fprintf(tmpFile, "-{%d-%d;%d;%d;%d}", game->grid.grid[i][j].position.x, game->grid.grid[i][j].position.y,
                    game->grid.grid[i][j].revealed, game->grid.grid[i][j].value, game->grid.grid[i][j].flagged);
        }
    }
    fprintf(tmpFile, "\n");

    // we copy the old backup
    rewind(file);
    while ((fgets(chaine, 500, file)) != NULL && nbrLignes >= 1) {
        chaine[1] = (char) ('0' + nbrLignes--);
        fputs(chaine, tmpFile);
    }
    fclose(file);
    fclose(tmpFile);

    remove(fileName);
    rename("tmpFile", fileName);
}

Game *load_game(int id) {
    FILE *file = NULL;
    file = fopen(file_games_non_completed, "r");
    int over;
    char ch = '1';
    Game *game = malloc(sizeof(Game));

    if (!file) {
        printf("Error on file %s: %s", file_games_non_completed, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (!game) {
        printf("Error during allocation");
        exit(EXIT_FAILURE);
    }

    while (ch != EOF) {
        // fetch a game
        fscanf(file, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", &game->id, game->name, &game->grid.size.width,
               &game->grid.size.height, &game->mines, &game->flags, &game->revealed, &game->score, &over);
        game->over = over;

        // check if it's the right game
        if (game->id == id) {

            // reintroduces spaces
            for (int i = 0; i < strlen(game->name); i++)
                if (game->name[i] == '|') game->name[i] = ' ';

            game->grid.grid = (Cell **) malloc(game->grid.size.height * sizeof(Cell *));
            if (!game->grid.grid) {
                printf("Error during allocation");
                exit(EXIT_FAILURE);
            }

            // fetching game cells information
            for (int i = 0; i < game->grid.size.height; i++) {
                game->grid.grid[i] = (Cell *) malloc(game->grid.size.width * sizeof(Cell));
                for (int j = 0, revealed, flagged; j < game->grid.size.width; j++) {
                    fscanf(file, "-{%d-%d;%d;%d;%d}", &game->grid.grid[i][j].position.x,
                           &game->grid.grid[i][j].position.y, &revealed, &game->grid.grid[i][j].value, &flagged);
                    game->grid.grid[i][j].revealed = revealed;
                    game->grid.grid[i][j].flagged = flagged;
                }
            }
            return game;
        }

        // line code to go to the next line
        while ((ch = (char) fgetc(file)) != EOF && ch != '\n');
    }
    fclose(file);
    free(game);
    return NULL;
}

void print_for_loading() {
    FILE *file = NULL;
    file = fopen(file_games_non_completed, "r");
    char ch;
    int over, nbrLignes = 0;
    Game *game = malloc(sizeof(Game));

    if (!file) {
        printf("Error on file %s: %s", file_games_non_completed, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (!game) {
        printf("Error during allocation");
        exit(EXIT_FAILURE);
    }

    // count the number of line in the file
    while ((ch = (char) fgetc(file)) != EOF) {
        if (ch == '\n') nbrLignes++;
    }

    // fetch each game and print it
    rewind(file);
    for (int i = 1; i <= nbrLignes; i++) {
        fscanf(file, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", &game->id, game->name, &game->grid.size.width,
               &game->grid.size.height, &game->mines, &game->flags, &game->revealed, &game->score, &over);
        game->over = over;
        fprintf(stdout, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}\n", i, game->name, game->grid.size.width, game->grid.size.height,
                game->mines, game->flags, game->revealed, game->score, game->over);
    }

    free(game);
    fclose(file);
}
