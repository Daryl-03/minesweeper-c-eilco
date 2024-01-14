#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "game.h"

void save_game(Game *game);

Game *load_game(int id);

int print_for_loading();

Game *print_statistics(int level, int *size);

void horitontal_line();

void table_row(Game *game, char *level);

void table_head();

int number_of_lines_in_file(FILE *file);

int game_level(Game *game);

void echanger_tas(Game *tas, int i, int j);

void inserer(Game *heap, int *size, Game game);

void deleteMin(Game *heap, int *size);

void trierParTas(Game *tableau, const int *size);

#endif