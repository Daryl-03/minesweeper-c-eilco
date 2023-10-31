#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H

#include "game.h"

void save_game(Game *game);

Game *load_game();

void print_statistics();
#endif 