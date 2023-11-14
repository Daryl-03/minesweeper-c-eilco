#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "game.h"

void save_game(Game *game);

Game *load_game(int id);

void print_for_loading();

void print_statistics();

void horitontal_line();

void table_row(Game *game, char *level);

void table_head();

int number_of_lines_in_file(FILE *file);

#endif 