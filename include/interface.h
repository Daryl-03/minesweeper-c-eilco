#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define GRID_WIDTH 5  // Largeur de la grille
#define GRID_HEIGHT 7 // Hauteur de la grille

struct position {
    int x;
    int y;
} typedef Position;

struct size {
    int width;
    int height;
} typedef Size;

struct cell {
    Position position;
    bool revealed;
    int value;
    bool flagged;
} typedef Cell;

struct grid {
    Cell **cells;
    Size size;
} typedef Grid;

void setConsoleToUTF8();
void clearScreen();
void displayGrid(Grid *grid);
void placeMines(Cell **grid, int mines, int width, int height);
void initializeGrid(Cell **grid, int width, int height);
void revealAll(Cell **grid, int width, int height);
void minesweeper(Grid *grid, int mines);
void printMenu();
void newGame();
void loadGame();
void printStatistics();
void Menu();

#endif // !INTERFACE_H
