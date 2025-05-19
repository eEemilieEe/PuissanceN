#include "player.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Player new_player(char* name, char signe) {
    Player player = malloc(sizeof(struct zPlayer));
    player->name = name;
    player->signe = signe;
    player->symbol = (signe == '$') ? 'X' : 'O';
    player->color = (signe == '$') ? RED : YELLOW;
    return player;
}

int play(Board g, int column, Player player) {
    int i;
    int n = g->row;
    i = 0;
    while ((i < n) && ((g->board)[i][column] == '.')) {
        i++;
    }
    (g->board)[i-1][column] = player->signe;
    g->libre--;
    return i-1;
}

Board simulate(Board g1, int column, Player player) {
    Board g = copieBoard(g1);
    play(g, column, player);
    return g;
}

void print_player(Player player) {
    printf("%s: %c\n", player->name, player->symbol);
}