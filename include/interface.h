#ifndef INTERFACE_H
#define INTERFACE_H

#include "game.h"

#define GRID_SIZE 5 // Taille de la grille


void setConsoleToUTF8();
void clearScreen();
void afficherGrille(Grid *grid);
//void afficherGrille(Grid* grid);
void placerMines(Cell grille[][GRID_SIZE], int mines);
void initialiserGrille(Cell grille[][GRID_SIZE], int taille);
void revealAll(Cell grid[][GRID_SIZE]);
void minesweeper(Cell grille[][GRID_SIZE], int taille, int mines);
void printMenu();
void newGame();
void loadGame();
void printStatistics();
// show and handle the menu
void menu();

// get difficulty settings from user
void getDifficultySettingsFromUser(int *mines, Size *size);

// get action from user
void getActionFromUser(Grid* grid, Position *position, InGameAction *action);

#endif // !INTERFACE_H