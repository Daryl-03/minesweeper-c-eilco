#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

// Let's define the game structures

typedef struct {
	int x;
	int y;
} Position;

typedef struct {
	int width;
	int height;
} Size;

// grid is a 2D array of integers
typedef struct {
	int **grid;
	Size size;
} Grid;

// now to the cell structure
typedef struct {
	Position position; // position of the cell in the grid
	bool revealed; // is the cell revealed?
	int value; // value of the cell
	bool flagged; // is the cell flagged?
} Cell;

// now to the game structure
typedef struct {
	Grid grid; // the grid of the game
	int mines; // number of mines in the game
	int flags; // number of flags in the game
	int revealed; // number of revealed cells in the game
	int score; // score of the game
	bool over; // is the game over?
} Game;

// some constants
const Size EASY = {9, 9};
const Size MEDIUM = {16, 16};
const Size HARD = {30, 16};


#endif // GAME_H