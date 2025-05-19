#include "arbre.h"

Arbre create_empty_arbre(){
	Arbre res = malloc(sizeof(struct zArbre));
	res->item = NULL;
	res->fils = NULL;
	res->frere = NULL;
	return res;
}
Item getItem(Arbre arbre){
	return arbre->item;
}
int getProfondeur(Arbre arbre){
	return arbre->profondeur;
}
Arbre getFils(Arbre arbre){
	return arbre->fils;
}
Arbre getFrere(Arbre arbre){
	return arbre->frere;
}
void addFils(Arbre pere, Arbre fils){
	fils->frere = pere->fils;
	pere->fils = fils;
	fils->profondeur = (pere->profondeur)+1;
}
void setArbre(Arbre arbre, Item item){
	arbre->item = item;
}

void produireFils(Arbre arbre, int profondeur, Player player1, Player player2){
	if((arbre==NULL) || (profondeur == 0)){}
	else{
		Board g = arbre->item->board; 
		int n = g->column;
		int * t = playable_positions(arbre->item->board);
		int i;
		for(i=0; i<n; i++){
			if(t[i] == 1){
				Board g1 = simulate(g, i, player1);
				
				Item item1 = create_empty_Item();
				setItem(item1, g1, 0);
				
				Arbre new_node1 = create_empty_arbre();
				setArbre(new_node1, item1);
				
				addFils(arbre, new_node1);
				
			}
			else{}
		}
		produireFils(arbre->frere, profondeur, player1, player2);
		produireFils(arbre->fils, profondeur-1, player2, player1);
	}
}


void freeArbre(Arbre arbre){
	freeItem(arbre->item);
	free(arbre);
}
Arbre copieArbre(Arbre arbre){
	Item new_item = copieItem(arbre->item);
	Arbre res = create_empty_arbre();
	res->item = new_item;
	res->profondeur = arbre->profondeur;
	return res;
}

void printArbre(Arbre arbre){
	if(arbre==NULL){
		
	}
	else{
		print_Board(arbre->item->board);
		printf("%d\n", arbre->item->score);
		printf("\n");
		printArbre(arbre->frere);
		printf("\n");
		printArbre(arbre->fils);
	}
}
