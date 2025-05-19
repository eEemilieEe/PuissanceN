#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Définition de la structure Board
typedef struct zBoard {
    int row;           // Nombre de lignes
    int column;        // Nombre de colonnes
    int libre;         // Espaces libres restants
    char** board;      // Matrice de la grille de jeu
} *Board;

// Fonctions pour la création et la gestion de la grille
Board create_new_Board(int n, int m);
void freeBoard(Board board);
Board copieBoard(Board g);

// Fonctions pour le controle de l'état du jeu
int checkWin(Board board, int p);
int check_playable(Board g);
int* playable_positions(Board g);

// Fonctions de support
int isChar(char c);
int check_winTAB(char* tab, int p);
int check_winNN(Board board, int i, int j, int p);
void print_Board(Board g);
void print_tab(int* t, int n);

#endif // BOARD_H