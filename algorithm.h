#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "arbre.h"
#include "item.h"
#include "player.h"

int minFils(Arbre arbre, Player p1, Player p2);
int maxFils(Arbre arbre, Player p1, Player p2);
void minimax(Arbre arbre, Player p1, Player p2, int n);
int MINMAX(Arbre arbre, int profondeur, Player p1, Player p2, int n);

#endif // ALGORITHM_H