#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "item.h"
#include "player.h"

typedef struct zArbre {
    Item item;
    int profondeur;
    struct zArbre* fils;
    struct zArbre* frere;
} *Arbre;

Arbre create_empty_arbre();
Item getItem(Arbre arbre);
int getProfondeur(Arbre arbre);
Arbre getFils(Arbre arbre);
Arbre getFrere(Arbre arbre);
void addFils(Arbre pere, Arbre fils);
void setArbre(Arbre arbre, Item item);
void produireFils(Arbre arbre, int profondeur, Player player1, Player player2);
void printArbre(Arbre arbre);
void freeArbre(Arbre arbre);
Arbre copieArbre(Arbre arbre);

#endif // ARBRE_H