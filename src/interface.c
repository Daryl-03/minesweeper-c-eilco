#include"../include/interface.h"
#include "../include/database_management.h"
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
void clearScreen()
{
    system("cls"); // Efface l'écran de la console sur Windows
}

void setConsoleToUTF8()
{
    SetConsoleOutputCP(65001); // Définit le codage de page de la console à l'UTF-8
}
#else

// Fonction de nettoyage de l'écran pour les systèmes non-Windows
void clearScreen() {
    system("clear");
}

void setConsoleToUTF8() {
}

#endif


void afficherGrille(Grid *grid) {
    char *helpList[] = {"Aide de jeu",
                        "Entrez une action suivie des coordonnees (ex: r A1)",
                        "Actions possibles: ",
                        "r: reveler une case",
                        "f: placer un drapeau",
                        "u: enlever un drapeau",
                        "s: sauvegarder la partie",
                        "q: quitter la partie",
    };
    int helpListIndex = 0;
    int width = grid->size.width;
    int height = grid->size.height;
    Cell **cells = grid->cells;

    printf("  ");
    for (int i = 0; i < width; ++i)
        printf(" %c  ", (char) ('A' + i));
    printf("\n");
    printf(" ╔");
    for (int i = 0; i < width - 1; ++i)
        printf("═══╦");
    printf("═══╗\n");

    for (int i = 0; i < height; ++i) {
        printf("%c", 'a' + i);
        printf("║");
        for (int j = 0; j < width; ++j) {
            if (!cells[i][j].revealed) {
                if (cells[i][j].flagged) {
                    printf(" ⚑ ║");
                } else {
                    printf(" ■ ║");
                }
            } else {
                if (cells[i][j].value == -1)
                    printf(" ✹ ║");
                else {
                    if (cells[i][j].value == 0)
                        printf("   ║");
                    else
                        printf(" %d ║", cells[i][j].value);
                }
            }
        }


        printf("\n");
        if (i < height - 1) {
            printf(" ╠");
            for (int j = 0; j < width - 1; ++j)
                printf("═══╬");
            printf("═══╣");
            if (helpListIndex < 8) printf("\t\t%s", helpList[helpListIndex++]);
            printf("\n");
        }
    }

    printf(" ╚");
    for (int i = 0; i < width - 1; ++i)
        printf("═══╩");
    printf("═══╝\n");
}

void getActionFromUser(Game *game, Position *position, InGameAction *action) {
    char entrees[3];

    afficherGrille(&(game->grid));
    printf("\n>> ");
    scanf("%s", entrees);
    getchar();


    position->x = tolower(entrees[1]) - 'a';
    position->y = tolower(entrees[2]) - 'a';
    *action = (InGameAction) (char) tolower(entrees[0]);

    if ((*action == QUIT || *action == SAVE)) {
        return;
    } else {
        if (*action != REVEAL && *action != FLAG && *action != UNFLAG && *action != SAVE && *action != QUIT) {
            clearScreen();
            printf("Action invalide\n");
            getActionFromUser(game, position, action);
        }
        if (position->x < 0 || position->x >= game->grid.size.width || position->y < 0 ||
            position->y >= game->grid.size.height) {
            clearScreen();
            printf("Coordonnees invalides\n");
            getActionFromUser(game, position, action);
        }
    }
}

void printMenu() {
    printf("        Bienvenue dans le jeu de Demineur !\n\n");
    printf("1. Lancer une nouvelle partie\n");
    printf("2. Charger une partie\n");
    printf("3. Voir les statistiques\n");
    printf("4. Quitter le jeu\n");
}

void menu() {
    int choice;
    clearScreen();
    printMenu();

    printf(">> ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            playGame(setupNewGame());
            break;
        case 2:
            loadGame();
            break;
        case 3:
            printStatistics();
            break;
        case 4:
            printf("Au revoir\n");
            break;
        default:
            printf("Choix invalide\n");
            menu();
            break;
    }
}

void getDifficultySettingsFromUser(int *mines, Size *size) {
    int choice;

    clearScreen();
    printf("Choisir la difficulte\n");
    printf("1. Facile\n");
    printf("2. Moyen\n");
    printf("3. Difficile\n");
    printf("4. Retourner au menu\n");

    printf(">> ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            *mines = EASY_MINES;
            *size = EASY_SIZE;
            break;
        case 2:

            *mines = MEDIUM_MINES;
            *size = MEDIUM_SIZE;
            break;
        case 3:
            *mines = HARD_MINES;
            *size = HARD_SIZE;
            break;
        case 4:
            menu();
            break;
        default:
            printf("Choix invalide\n");
            getDifficultySettingsFromUser(mines, size);
            break;
    }
}

void loadGame() {
    int choice = -1, nbrLigne = 0;
    printf("\nEnsemble des jeux sauvegardes\n");
    nbrLigne = print_for_loading();

    while (!(-1 < choice && choice <= nbrLigne)) {
        printf("Selectionner une partie à chager (un nombre entre 1 et %d, 0 pour retourner au menu) : ", nbrLigne);
        scanf("%d", &choice);
    }

    if (choice == 0) {
        menu();
    } else {
        playGame(load_game(choice));
    }
}

void printStatistics() {
    printf("Ensemble des statistiques");
}

void getGameInformationFromUser(Game *game) {
    printf("Entrer un nom pour la partie : ");
    scanf("%[^\n]", game->name);
}



