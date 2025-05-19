#include "board.h"

Board create_new_Board(int n, int m){
	Board g = malloc(sizeof(struct zBoard));
	
	g->row = n;
	g->column = m;
	
	char ** res = malloc(n*sizeof(char*));
	int i, j;
	for(i=0; i<n; i++){
		res[i] = malloc(m*sizeof(char));
		for(j=0; j<m; j++){
			res[i][j] = 46;
		}
	}
	
	g->board = res;
	g->libre = n*m;
	
	return g;
}
int isChar(char c){
	return(c==36 || c==35);
}
int check_winTAB(char * tab, int p){
	int i;
	i=0;
	int pareil=1;
	char temp = tab[0];
	if(!isChar(temp)){
		return 0;
	}
	else{
		while(i<p && pareil){
			if(temp!=tab[i]){
				pareil=0;
			}
			else{
				i++;	
			}
		}
		if(i==p){
			return 1;
		}
		else{
			return 0;
		}	
	}
}
int check_winNN(Board board, int i, int j, int p){
	char temp[p];
	int k, l;
	int WIN=0;
	
	//Lignes
	for(k=0;k<p;k++){
		for(l=0;l<p;l++){
			temp[l] = (board->board)[i+k][j+l];
		}
		WIN = check_winTAB(temp, p);
		if(WIN){
			break;
		}
	}
	if(WIN){
		return 1;
	}
	else{
		//Colomnes
		for(l=0;l<p;l++){
			for(k=0;k<p;k++){
				temp[k] = (board->board)[i+k][j+l];
			}
			WIN = check_winTAB(temp, p);
			
			if(WIN){
				break;
			}
		}
		if(WIN){
			return 1;
		}
		else{
			//Diagonales
			for(k=0; k<p; k++){
				temp[k] = (board->board)[i+k][j+k];
			}
			WIN = check_winTAB(temp, p);
			if(WIN){
				return 1;
			}
			else{
				for(k=0; k<p; k++){
					temp[k] = (board->board)[i+k][p+j-1-k];
				}
				return(check_winTAB(temp, p));
			}
		}
	}	
}

int checkWin(Board board, int p) {
    	int n = board->row;
    	int m = board->column;
    	int WIN = 0;
    	int i, j;
    	
    	for(i=0; i<n-p+1; i++){
    		for(j=0; j<m-p+1; j++){
    			WIN = check_winNN(board, i, j, p);
    			if(WIN){
    				i=n;
    				j=m;
    			}
    		}
    	}
    	return WIN;

    
}

int check_playable(Board g){
	return (g->libre>0);
}
Board copieBoard(Board g){
	Board res = create_new_Board(g->row, g->column);
	res->row=g->row;
	res->column=g->column;
	res->libre=g->libre;
	int i, j;
	for (i=0; i<g->row; i++){
		for(j=0; j<g->column; j++){
			res->board[i][j] = g->board[i][j];
		}
	}
	return res;
}
int * playable_positions(Board g){
	int n = g->column;
	int * res = malloc(n*sizeof(int));
	int i;
	for(i=0; i<n; i++){
		if(g->board[0][i] == 46){
			res[i] = 1;
		}
		else{
			res[i] = 0;
		}
	}
	return res;
}
void print_Board(Board g){
	int n = g->row;
	int m = g->column;
	int i, j;
	for(i=0; i<n; i++){
		for(j=0; j<m; j++){
			printf("| %c |", (g->board)[i][j]);
		}
		printf("\n");
		for(j=0; j<m; j++){
			printf("-----");
		}
		printf("\n");
	} 
}
void freeBoard(Board board){
	char ** temp = board->board;
	int i, j;
	for(i=0; i<board->row; i++){
		free(temp[i]);
	}
	free(board);
}
void print_tab(int * t, int n){
	int i;
	for(i=0; i<n; i++){
		printf("%d ", *(t+i));
	}
	printf("\n");
}
