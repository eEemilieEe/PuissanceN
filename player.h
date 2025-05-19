#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "board.h"

typedef struct zPlayer {
    char* name;
    char symbol;  // 'X' ou 'O' pour l'affichage
    char signe;   // '$' ou '#' pour la grille interne
    Color color;
} *Player;

Player new_player(char* name, char signe);
int play(Board g, int column, Player player);
Board simulate(Board g1, int column, Player player);
void print_player(Player player);

#endif // PLAYER_H