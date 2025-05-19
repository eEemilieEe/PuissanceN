#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>  
#include "board.h"
#include "player.h"

typedef struct {
    Board board;
    int score;
} *Item;

Item create_empty_Item();
Board getBoard(Item item);
int getScore(Item item);
int score(char* c, int n, Player p1, Player p2);
int calculateScore(Item item, int n, Player p1, Player p2);
void setItem(Item item, Board board, int score);
Item copieItem(Item item);
void freeItem(Item item);

#endif // ITEM_H