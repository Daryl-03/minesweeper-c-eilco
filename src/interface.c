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




void afficherGrille(Cell grille[][GRID_SIZE], int taille)
{
    int i, j;
    printf("  ");
    for (i = 1; i <= taille; ++i)
        printf(" %d  ", i);
    printf("\n");
    printf(" ╔");
    for (i = 0; i < taille - 1; ++i)
        printf("═══╦");
    printf("═══╗\n");
    for (i = 0; i < taille; ++i)
    {
        printf("%c", 'A' + i);
        printf("║");
        for (j = 0; j < taille; ++j)
        {
            if (!grille[i][j].revealed)
                printf(" ■ ║");
            else if (grille[i][j].value == -1 && grille[i][j].revealed)
                printf(" ✹ ║");
            else if(grille[i][j].revealed)
                printf(" %d ║", grille[i][j].value);
        }
        printf("\n");
        if (i < taille - 1)
        {
            printf(" ╠");
            for (j = 0; j < taille - 1; ++j)
                printf("═══╬");
            printf("═══╣\n");
        }
    }
    printf(" ╚");
    for (i = 0; i < taille - 1; ++i)
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

void minesweeper(Cell grille[][GRID_SIZE], int taille, int mines)
{
    clearScreen();
    initialiserGrille(grille, GRID_SIZE);
    placerMines(grille, mines);
    afficherGrille(grille, GRID_SIZE);

    Position coord;
    char input[3];
    bool playing = true;
    bool lost = false;

    while (playing)
    {
        printf("\nEntrez les coordonées ou q pour quitter la partie: ");
        scanf("%s", input);

        if (tolower(input[0]) == 'q')
        {
            playing = false;
        }
        else if (!lost)
        {
            coord.x = tolower(input[0]) - 'a';
            coord.y = input[1] - '1';

            if (coord.x < 0 || coord.x >= GRID_SIZE || coord.y < 0 || coord.y >= GRID_SIZE)
            {
                clearScreen();
                afficherGrille(grille, GRID_SIZE);
                printf("\nDernier mouvement effectué: ⚠️ Coordonnées invalides⚠️");
            }
            else
            {
                grille[coord.x][coord.y].revealed = true;
                clearScreen();
                printf("Dernier mouvement effectué: %s\n", input);
                afficherGrille(grille, GRID_SIZE);

                if (grille[coord.x][coord.y].value == -1)
                {
                    clearScreen();
                    revealAll(grille);
                    lost = true;
                    break;
                }
            }
        }
    }

    if(lost){
        afficherGrille(grille,GRID_SIZE);
        printf("Partie perdue!");
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
    Cell grille[GRID_SIZE][GRID_SIZE];
    int mines = 5; // Nombre de mines dans la grille
    int difficulty;

    clearScreen();
    printf("Choisir la difficulté\n");
    printf("1. Facile\n");
    printf("2. Moyen\n");
    printf("3. Difficile\n");

    scanf("%d", &difficulty);

    if (difficulty == 1)
    {
        minesweeper(grille, GRID_SIZE, mines);
    }
    else if (difficulty == 2)
    {
        minesweeper(grille, GRID_SIZE, mines);
    }
    else if (difficulty == 3)
    {
        minesweeper(grille, GRID_SIZE, mines);
    }
}

void loadGame()
{
    printf("Ensemble des jeux sauvegardés\n");
}

void printStatistics()
{
    printf("Ensemble des statistiques");
}

void Menu()
{
    int choice;
    clearScreen();
    printMenu();

    scanf("%d", &choice);

    if (choice == 1)
    {
        newGame();
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
    }
}

