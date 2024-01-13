#include"../include/interface.h"

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
void clearScreen()
{
    system("clear");
}
void setConsoleToUTF8()
{
}
#endif




void afficherGrille(Grid *grid) {
    int width = grid->size.width;
    int height = grid->size.height;
    Cell **cells = grid->cells;

    printf("  ");
    for (int i = 1; i <= width; ++i)
        printf(" %d  ", i);
    printf("\n");
    printf(" ╔");
    for (int i = 0; i < width - 1; ++i)
        printf("═══╦");
    printf("═══╗\n");

    for (int i = 0; i < height; ++i) {
        printf("%c", 'A' + i);
        printf("║");
        for (int j = 0; j < width; ++j) {
            if (!cells[i][j].revealed)
                printf(" ■ ║");
            else if (cells[i][j].value == -1 && cells[i][j].revealed)
                printf(" ✹ ║");
            else if (cells[i][j].revealed){
                if(cells[i][j].value == 0)
                    printf("   ║");
                else
                    printf(" %d ║", cells[i][j].value);
            }
        }
        printf("\n");
        if (i < height - 1) {
            printf(" ╠");
            for (int j = 0; j < width - 1; ++j)
                printf("═══╬");
            printf("═══╣\n");
        }
    }
    printf(" ╚");
    for (int i = 0; i < width - 1; ++i)
        printf("═══╩");
    printf("═══╝\n");
}

void placerMines(Cell grille[][GRID_SIZE], int mines)
{
    srand(time(NULL));
    int i, j;
    for (int count = 0; count < mines; ++count)
    {
        do
        {
            i = rand() % GRID_SIZE;
            j = rand() % GRID_SIZE;
        } while (grille[i][j].value == -1);
        grille[i][j].value = -1;
    }
}

void initialiserGrille(Cell grille[][GRID_SIZE], int taille)
{
    for (int i = 0; i < taille; ++i)
    {
        for (int j = 0; j < taille; ++j)
        {
            grille[i][j].value = 1;
            grille[i][j].revealed = false;
            grille[i][j].flagged = false;
            grille[i][j].position.x = i;
            grille[i][j].position.y = j;
        }
    }
}

void revealAll(Cell grid[][GRID_SIZE])
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            grid[i][j].revealed = true;
        }
    }
}

//void minesweeper(Cell grille[][GRID_SIZE], int taille, int mines)
//{
//    clearScreen();
//    initialiserGrille(grille, GRID_SIZE);
//    placerMines(grille, mines);
//    afficherGrille(grille, GRID_SIZE);
//
//    Position coord;
//    char input[3];
//    bool playing = true;
//    bool lost = false;
//
//    while (playing)
//    {
//        printf("\nEntrez les coordonées ou q pour quitter la partie: ");
//        scanf("%s", input);
//
//        if (tolower(input[0]) == 'q')
//        {
//            playing = false;
//        }
//        else if (!lost)
//        {
//            coord.x = tolower(input[0]) - 'a';
//            coord.y = input[1] - '1';
//
//            if (coord.x < 0 || coord.x >= GRID_SIZE || coord.y < 0 || coord.y >= GRID_SIZE)
//            {
//                clearScreen();
//                afficherGrille(grille, GRID_SIZE);
//                printf("\nDernier mouvement effectué: ⚠️ Coordonnées invalides⚠️");
//            }
//            else
//            {
//                grille[coord.x][coord.y].revealed = true;
//                clearScreen();
//                printf("Dernier mouvement effectué: %s\n", input);
//                afficherGrille(grille, GRID_SIZE);
//
//                if (grille[coord.x][coord.y].value == -1)
//                {
//                    clearScreen();
//                    revealAll(grille);
//                    lost = true;
//                    break;
//                }
//            }
//        }
//    }
//
//    if(lost){
//        afficherGrille(grille,GRID_SIZE);
//        printf("Partie perdue!");
//    }
//}

void getActionFromUser(Grid* grid, Position *position, InGameAction *action){
    char letter;
    char actionChar;
    int number;

//    printGridFromPlayerPerspective(*grid);
    afficherGrille(grid);

    printf("\nEntrez une action suivie des coordonnees (ex: r A1)\n");
    printf("Actions possibles: \n");
    printf("r: reveler une case\n");
    printf("f: placer un drapeau\n");
    printf("u: enlever un drapeau\n");
    printf("s: sauvegarder la partie\n");
    printf("q: quitter la partie\n\n");

    scanf(" %c %c %d", &actionChar, &letter, &number);

    position->y = tolower(letter) - 'a';
    position->x = number - 1;
    actionChar = tolower(actionChar);
    *action = (InGameAction) actionChar;

    //logs
//    printf("Action: %c\n", *action);

    // action is r
//    printf("action is r: %d\n", *action == REVEAL);

    //position
//    printf("Position: %d %d\n", position->x, position->y);

    if(position->x < 0 || position->x >= grid->size.width || position->y < 0 || position->y >= grid->size.height){
        clearScreen();
        printf("Coordonnees invalides\n");
        getActionFromUser(grid, position, action);
    }
    if(*action != REVEAL && *action != FLAG && *action != UNFLAG && *action != SAVE && *action != QUIT){
        clearScreen();
        printf("Action invalide\n");
        getActionFromUser(grid, position, action);
    }
}

void printMenu()
{

    printf("        Bienvenue dans le jeu de Demineur !\n\n");
    printf("1. Lancer une nouvelle partie\n");
    printf("2. Charger une partie\n");
    printf("3. Voir les statistiques\n");
    printf("4. Quitter le jeu\n");
}

void newGame()
{
//    Cell grille[GRID_SIZE][GRID_SIZE];
//    int mines = 5; // Nombre de mines dans la grille
//    int difficulty;
//
//    clearScreen();
//    printf("Choisir la difficulté\n");
//    printf("1. Facile\n");
//    printf("2. Moyen\n");
//    printf("3. Difficile\n");
//
//    scanf("%d", &difficulty);
//
//    if (difficulty == 1)
//    {
//        minesweeper(grille, GRID_SIZE, mines);
//    }
//    else if (difficulty == 2)
//    {
//        minesweeper(grille, GRID_SIZE, mines);
//    }
//    else if (difficulty == 3)
//    {
//        minesweeper(grille, GRID_SIZE, mines);
//    }
}

void menu()
{
    int choice;
    clearScreen();
    printMenu();

    scanf("%d", &choice);

    if (choice == 1)
    {
        createAndPlayGame();
    }
    else if (choice == 2)
    {
        loadGame();
    }
    else if (choice == 3)
    {
        printStatistics();
    }
    else if (choice == 4)
    {
        exit(0);
    } else {
        printf("Choix invalide\n");
        menu();
    }
}

void getDifficultySettingsFromUser(int *mines, Size *size){
    int choice;

    clearScreen();
    printf("Choisir la difficulte\n");
    printf("1. Facile\n");
    printf("2. Moyen\n");
    printf("3. Difficile\n");
    printf("4. Retour\n");

    scanf("%d", &choice);

    if(choice == 1){
        *mines = EASY_MINES;
        *size = EASY_SIZE;
    } else if(choice == 2){
        *mines = MEDIUM_MINES;
        *size = MEDIUM_SIZE;
    } else if(choice == 3){
        *mines = HARD_MINES;
        *size = HARD_SIZE;
    } else if(choice == 4){
        return;
    } else {
        printf("Choix invalide\n");
        getDifficultySettingsFromUser(mines, size);
    }
}

void loadGame()
{
    printf("Ensemble des jeux sauvegardes\n");
}

void printStatistics()
{
    printf("Ensemble des statistiques");
}



