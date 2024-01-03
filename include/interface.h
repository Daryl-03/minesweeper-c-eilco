#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define GRID_SIZE 5 // Taille de la grille

struct position
{
    int x;
    int y;
} typedef Position;

struct size
{
    int width;
    int height;
} typedef Size;

struct cell
{
    Position position;
    bool revealed;
    int value;
    bool flagged;
} typedef Cell;

void setConsoleToUTF8();
void clearScreen();
void afficherGrille(Cell grille[][GRID_SIZE], int taille);
void placerMines(Cell grille[][GRID_SIZE], int mines);
void initialiserGrille(Cell grille[][GRID_SIZE], int taille);
void revealAll(Cell grid[][GRID_SIZE]);
void minesweeper(Cell grille[][GRID_SIZE], int taille, int mines);
void printMenu();
void newGame();
void loadGame();
void printStatistics();
void Menu();


#endif // !INTERFACE_H