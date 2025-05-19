#include "item.h"
#include <stdlib.h>

Item create_empty_Item() {
    Item res = malloc(sizeof(*res));
    res->board = NULL;
    res->score = 0;
    return res;
}

Board getBoard(Item item) {
    return item->board;
}

int getScore(Item item) {
    return item->score;
}

void setItem(Item item, Board board, int score) {
    item->board = board;
    item->score = score;
}

Item copieItem(Item item) {
    Item res = create_empty_Item();
    res->board = copieBoard(item->board);
    res->score = item->score;
    return res;
}

int score(char* c, int n, Player p1, Player p2) {
    int score = 0;
    int p1Count = 0;
    int p2Count = 0;
    int emptyCount = 0;
    
    // Compte les jetons consécutifs
    for (int i = 0; i < n; i++) {
        if (c[i] == p1->symbol || c[i] == p1->signe) {
            p1Count++;
            p2Count = 0;  // Réinitialise le compteur de l'adversaire
        } else if (c[i] == p2->symbol || c[i] == p2->signe) {
            p2Count++;
            p1Count = 0;  // Réinitialise le compteur du joueur
        } else if (c[i] == '.') {
            emptyCount++;
        }
        
        // Attribue des scores en fonction des jetons consécutifs
        if (p1Count == 4) return 1000;  // Victoire p1
        if (p2Count == 4) return -1000; // Victoire p2
        
        if (p1Count == 3 && emptyCount > 0) score += 50;   // Trois alignés p1
        if (p2Count == 3 && emptyCount > 0) score -= 50;   // Trois alignés p2
        
        if (p1Count == 2 && emptyCount > 0) score += 10;   // Deux alignés p1
        if (p2Count == 2 && emptyCount > 0) score -= 10;   // Deux alignés p2
    }
    
    return score;
}

void freeItem(Item item) {
    if (item == NULL) return;
    
    if (item->board != NULL) {
        freeBoard(item->board);
        item->board = NULL;
    }
    
    free(item);
}

int calculateScore(Item item, int n, Player p1, Player p2) {
    if (item == NULL || item->board == NULL) return 0;
    
    Board b = item->board;
    int i, j, p;
    int k = b->row; // Vérifie le nom correct du champ
    int l = b->column; // Vérifie le nom correct du champ
    
    int sumrow = 0, sumcolumn = 0, sumdiagonal = 0;
    char** g = b->board;
    
    // Alloue de la mémoire dynamiquement au lieu d'utiliser VLA
    char* temp = (char*)malloc(l * sizeof(char));
    if (temp == NULL) return 0;
    
    // Pour les lignes
    for (i = 0; i < k; i++) {
        for (j = 0; j < l; j++) {
            temp[j] = g[i][j];
        }
        sumrow = sumrow + score(temp, l, p1, p2);
    }
    
    char* temp1 = (char*)malloc(k * sizeof(char));
    if (temp1 == NULL) {
        free(temp);
        return 0;
    }
    
    // Pour les colonnes
    for (j = 0; j < l; j++) {
        for (i = 0; i < k; i++) {
            temp1[i] = g[i][j];
        }
        sumcolumn = sumcolumn + score(temp1, k, p1, p2);
    }
    
    char* temp2 = (char*)malloc(n * sizeof(char));
    if (temp2 == NULL) {
        free(temp);
        free(temp1);
        return 0;
    }
    
    // Pour les diagonales
    for (i = 0; i < k - n + 1; i++) {
        for (j = 0; j < l - n + 1; j++) {
            for (p = 0; p < n; p++) {
                temp2[p] = g[i+p][j+p];
            }
            sumdiagonal = sumdiagonal + score(temp2, n, p1, p2);
        }
    }

    for (i = 0; i < k - n + 1; i++) {
        for (j = 0; j < l - n + 1; j++) {
            for (p = 0; p < n; p++) {
                temp2[p] = g[k-i-p-1][j+p];
            }
            sumdiagonal = sumdiagonal + score(temp2, n, p1, p2);
        }
    }
    
    free(temp);
    free(temp1);
    free(temp2);
    
    return sumrow + sumcolumn + sumdiagonal;
}