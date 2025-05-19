#include "algorithm.h"


int minFils(Arbre arbre, Player p1, Player p2){
	if(arbre->frere==NULL){
		return arbre->item->score;
	}
	else{
		int min;
		int myscore = arbre->item->score;
		int scoreFrere = minFils(arbre->frere, p1, p2);
		if(myscore<scoreFrere){
			min=myscore;
		}
		else{
			min=scoreFrere;
		}
		return min;
	}
}
int maxFils(Arbre arbre, Player p1, Player p2){
	if(arbre->frere==NULL){
		return arbre->item->score;
	}
	else{
		int max;
		int myscore = arbre->item->score;
		int scoreFrere = maxFils(arbre->frere, p1, p2);
		if(myscore>scoreFrere){
			max=myscore;
		}
		else{
			max=scoreFrere;
		}
		return max;
	}
}
void minimax(Arbre arbre, Player p1, Player p2, int n){
	if(arbre==NULL){}
	else{
		if(arbre->fils==NULL){
			arbre->item->score=calculateScore(arbre->item, n, p1, p2);
			minimax(arbre->frere, p1, p2, n);
		}
		else{
			if(p2->signe==36){
				minimax(arbre->fils, p2, p1, n);
				arbre->item->score = maxFils(arbre->fils, p1, p2);
				minimax(arbre->frere, p1, p2, n);
			}
			else{
				minimax(arbre->fils, p2, p1, n);
				arbre->item->score = minFils(arbre->fils, p1, p2);
				minimax(arbre->frere, p1, p2, n);
			}
		}
	}
}
int MINMAX(Arbre arbre, int profondeur, Player p1, Player p2, int n){
    produireFils(arbre, profondeur, p1, p2);
    minimax(arbre, p1, p2, n);
    
    // S'il n'y a pas d'enfants, il n'y a pas de coup possible
    if (arbre->fils == NULL) {
        return -1;
    }
    
    int l = arbre->item->board->column;
    int k = arbre->item->board->row;
    int i, j;
    Arbre fils = arbre->fils;
    Arbre bestFils = NULL;
    int bestScore = (p1->signe == 36) ? INT_MIN : INT_MAX; // $ = 36, valeur maximale pour le joueur 1
    int colonna = -1;
    
    // Trouve l'enfant avec le meilleur score
    while (fils != NULL) {
        int score = fils->item->score;
        
        // Pour le joueur 1 (maximiseur)
        if (p1->signe == 36 && score >= bestScore) {
            bestScore = score;
            bestFils = fils;
        }
        // Pour le joueur 2 (minimiseur)
        else if (p1->signe != 36 && score <= bestScore) {
            bestScore = score;
            bestFils = fils;
        }
        
        fils = fils->frere;
    }
    
    // Trouve la colonne qui correspond au meilleur coup
    if (bestFils != NULL) {
        Board OG = arbre->item->board;
        Board NOG = bestFils->item->board;
        
        // Pour chaque colonne
        for (j = 0; j < l; j++) {
            // Pour chaque ligne
            for (i = 0; i < k; i++) {
                if ((OG->board)[i][j] != (NOG->board)[i][j]) {
                    return j; // Trouvé la colonne du coup
                }
            }
        }
    }
    
    // Fallback: si nous ne trouvons pas de coup valide, prendre la première colonne jouable
    int* playable = playable_positions(arbre->item->board);
    for (j = 0; j < l; j++) {
        if (playable[j] == 1) {
            free(playable);
            return j;
        }
    }
    free(playable);
    
    return -1; // Aucun coup possible
}