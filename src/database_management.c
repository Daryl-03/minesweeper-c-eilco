#include "database_management.h"

#define file_games_completed "games_completed.txt"
#define file_games_non_completed "games_non_completed.txt"

// data form in the file : {id;name ;width;height;mines;flags;revealed;score;over}


void print_char(char c, int n) {
    for (int i = 0; i < n; i++) printf("%c", c);
}

void save_game(Game *game) {
    FILE *file, *tmpFile;
    char *fileName = game->over ? file_games_completed : file_games_non_completed, chaine[10000];
    int nbrLignes;

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
    nbrLignes = number_of_lines_in_file(file) + 1;

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
            fprintf(tmpFile, "-{%d-%d;%d;%d;%d}", game->grid.cells[i][j].position.x, game->grid.cells[i][j].position.y,
                    game->grid.cells[i][j].revealed, game->grid.cells[i][j].value, game->grid.cells[i][j].flagged);
        }
    }
    fprintf(tmpFile, "\n");

    // we copy the old backup
    rewind(file);
    while ((fgets(chaine, 10000, file)) != NULL && nbrLignes >= 1) {
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
    int over, nbrLignes = 0;
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

    // count the number of line in the file
    nbrLignes = number_of_lines_in_file(file);

    if (!nbrLignes) return NULL;
    id = nbrLignes - id + 1;

    while (ch != EOF) {
        game->name = malloc(sizeof(char ) * GAME_NAME_SIZE);
        if (!(game->name)) {
            printf("Error during allocation");
            exit(EXIT_FAILURE);
        }

        // fetch a game
        fscanf(file, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", &game->id, game->name, &game->grid.size.width,
               &game->grid.size.height, &game->mines, &game->flags, &game->revealed, &game->score, &over);
        game->over = over;

        // check if it's the right game
        if (game->id == id) {

            // reintroduces spaces
            for (int i = 0; i < strlen(game->name); i++)
                if (game->name[i] == '|') game->name[i] = ' ';

            game->grid.cells = (Cell **) malloc(game->grid.size.height * sizeof(Cell *));
            if (!game->grid.cells) {
                printf("Error during allocation");
                exit(EXIT_FAILURE);
            }

            // fetching game cells information
            for (int i = 0; i < game->grid.size.height; i++) {
                game->grid.cells[i] = (Cell *) malloc(game->grid.size.width * sizeof(Cell));
                for (int j = 0, revealed, flagged; j < game->grid.size.width; j++) {
                    fscanf(file, "-{%d-%d;%d;%d;%d}", &game->grid.cells[i][j].position.x,
                           &game->grid.cells[i][j].position.y, &revealed, &game->grid.cells[i][j].value, &flagged);
                    game->grid.cells[i][j].revealed = revealed;
                    game->grid.cells[i][j].flagged = flagged;
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
        printf("Aucune sauvegarde trouvée");
        if (game) free(game);
        return;
    }
    if (!game) {
        printf("Error during allocation");
        exit(EXIT_FAILURE);
    }

    // count the number of line in the file
    nbrLignes = number_of_lines_in_file(file);

    if (!nbrLignes) {
        printf("La sauvegarde est vide");
        return;
    }

    // table head
    horitontal_line();
    table_head();
    horitontal_line();

    // fetch each game and print it
    rewind(file);
    for (int i = 1; i <= nbrLignes; i++) {
        game->name = malloc(sizeof(char ) * GAME_NAME_SIZE);
        if (!(game->name)) {
            printf("Error during allocation");
            exit(EXIT_FAILURE);
        }

        fscanf(file, "{%d;%s ;%d-%d;%d;%d;%d;%d;%d}", &game->id, game->name, &game->grid.size.width,
               &game->grid.size.height, &game->mines, &game->flags, &game->revealed, &game->score, &over);
        game->over = over;

        // reintroduces spaces
        for (int j = 0; j < strlen(game->name); j++)
            if (game->name[j] == '|') game->name[j] = ' ';

        game->id = i;
        table_row(game, game->grid.size.width == 9 ? "Easy" : game->grid.size.width == 16 ? "Medium" : "Hard");

        // line code to go to the next line
        while ((ch = (char) fgetc(file)) != EOF && ch != '\n');
    }
    horitontal_line();

    free(game);
    fclose(file);
}

void print_statistics() {
    FILE *file = NULL;
    file = fopen(file_games_completed, "r");

    if (!file) {
        printf("Aucune statistique trouvée");
        return;
    }

    fclose(file);
}

int number_of_lines_in_file(FILE *file) {
    int nbrLignes = 0;
    char ch = '1';
    while ((ch = (char) fgetc(file)) != EOF) {
        if (ch == '\n') nbrLignes++;
    }
    rewind(file);
    return nbrLignes;
}

void table_head() {
    printf("| Id ");
    print_char('|', 1);
    print_char(' ', (40 - strlen("name")) / 2);
    printf("Name");
    print_char(' ', (40 - strlen("name")) / 2);
    printf("|");
    printf(" Levels ");
    printf("|");
    printf(" Mines ");
    printf("|");
    printf(" Flags ");
    printf("|");
    printf(" Revealed ");
    printf("|");
    print_char(' ', (strlen("23h59m59s") - strlen("score")) / 2);
    printf(" Score ");
    print_char(' ', (strlen("23h59m59s") - strlen("score")) / 2);
    printf("|\n");
}

void table_row(Game *game, char *level) {
    char str[GAME_NAME_SIZE];
    if (game) {
        sprintf(str, "0%d", game->id);
        printf("| %s ", str);
        print_char('|', 1);
        sprintf(str, "%s", game->name);
        print_char(' ', (40 - (int) strlen(str)) / 2);
        printf("%s", str);
        print_char(' ', (40 - (int) strlen(str)) / 2 + (strlen(str) % 2 == 1));
        printf("|");
        print_char(' ', (int) (strlen("Levels") + 2 - strlen(level)) / 2);
        printf("%s", level);
        print_char(' ', (int) (strlen("Levels") + 2 - strlen(level)) / 2 + (strlen(level) % 2 == 1));
        printf("|");
        sprintf(str, "%d", game->mines);
        print_char(' ', (int) (strlen("Mines") + 2 - strlen(str)) / 2);
        printf("%s", str);
        print_char(' ', (int) (strlen("Mines") + 2 - strlen(str)) / 2 + (strlen(str) % 2 == 0));
        printf("|");
        sprintf(str, "%d", game->flags);
        print_char(' ', (int) (strlen("Flags") + 2 - strlen(str)) / 2);
        printf("%s", str);
        print_char(' ', (int) (strlen("Flags") + 2 - strlen(str)) / 2 + (strlen(str) % 2 == 0));
        printf("|");
        sprintf(str, "%d", game->revealed);
        print_char(' ', (int) (strlen("Revealed") + 2 - strlen(str)) / 2);
        printf("%s", str);
        print_char(' ', (int) (strlen("Revealed") + 2 - strlen(str)) / 2 + (strlen(str) % 2 == 1));
        printf("|");
        sprintf(str, "%d", game->score);
        print_char(' ', (int) (strlen("23h59m59s") - strlen(str)) / 2);
        printf(" %s ", str);
        print_char(' ', (int) (strlen("23h59m59s") - strlen(str)) / 2);
        printf("|\n");
    }
}

void horitontal_line() {
    print_char('+', 1);
    print_char('-', 4);
    print_char('+', 1);
    print_char('-', 40);
    print_char('+', 1);
    print_char('-', strlen("level ") + 2);
    print_char('+', 1);
    print_char('-', strlen("mines") + 2);
    print_char('+', 1);
    print_char('-', strlen("flags") + 2);
    print_char('+', 1);
    print_char('-', strlen("revealed") + 2);
    print_char('+', 1);
    print_char('-', strlen("23h59m59s") + 2);
    print_char('+', 1);
    printf("\n");
}
