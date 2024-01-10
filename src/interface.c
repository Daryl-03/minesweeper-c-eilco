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


void displayGrid(Grid *grid) {
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
            else if (cells[i][j].revealed)
                printf(" %d ║", cells[i][j].value);
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

void placeMines(Cell **grid, int mines, int width, int height) {
    srand(time(NULL));
    int i, j;
    for (int count = 0; count < mines; ++count) {
        do {
            i = rand() % height;
            j = rand() % width;
        } while (grid[i][j].value == -1);
        grid[i][j].value = -1;
    }
}

void initializeGrid(Cell **grid, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j].value = 1;
            grid[i][j].revealed = false;
            grid[i][j].flagged = false;
            grid[i][j].position.x = i;
            grid[i][j].position.y = j;
        }
    }
}

void revealAll(Cell **grid, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j].revealed = true;
        }
    }
}

bool checkIfWon(int width, int height, int mines, int opened){
    if(width*height - mines == opened)
    return true;
};


void minesweeper(Grid *grid, int mines)
{
    int width = grid->size.width;
    int height = grid->size.height;
    int opened = 0;
    Cell **cells = grid->cells;

    initializeGrid(cells, width, height);
    placeMines(cells, mines, width, height);
    displayGrid(grid);

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

            if (coord.x < 0 || coord.x >= width || coord.y < 0 || coord.y >= height)
            {
                clearScreen();
                displayGrid(grid);
                printf("\nDernier mouvement effectué: ⚠️ Coordonnées invalides⚠️");
            }
            else
            {
                cells[coord.x][coord.y].revealed = true;
                clearScreen();
                printf("Dernier mouvement effectué: %s\n", input);
                displayGrid(grid);

                if (cells[coord.x][coord.y].value == -1)
                {
                    clearScreen();
                    revealAll(cells, width, height);
                    lost = true;
                    break;
                }
                else 
                    opened++;
            }
        }
    }

    if(lost){
        displayGrid(grid);
        printf("\nPartie perdue!");
    }
    else if(checkIfWon(width, height, mines, opened)){
        revealAll(cells, width, height);
        displayGrid(grid);
        printf("\nVictoire !");
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

void newGame() {
    Size gridSize = {GRID_WIDTH, GRID_HEIGHT};
    Cell **grille = malloc(gridSize.height * sizeof(Cell *));
    for (int i = 0; i < gridSize.height; ++i) {
        grille[i] = malloc(gridSize.width * sizeof(Cell));
    }

    int mines = 5; // Nombre de mines dans la grille
    int difficulty;

    clearScreen();
    printf("Choisir la difficulté\n");
    printf("1. Facile\n");
    printf("2. Moyen\n");
    printf("3. Difficile\n");

    scanf("%d", &difficulty);

    Grid gameGrid;
    gameGrid.cells = grille;
    gameGrid.size = gridSize;

    if (difficulty == 1) {
        minesweeper(&gameGrid, mines);
    } else if (difficulty == 2) {
        minesweeper(&gameGrid, mines);
    } else if (difficulty == 3) {
        minesweeper(&gameGrid, mines);
    }

    // Après le jeu, libérez la mémoire allouée pour la grille
    for (int i = 0; i < gridSize.height; ++i) {
        free(grille[i]);
    }
    free(grille);
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

