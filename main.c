#include "raylib.h"
#include "config.h"
#include "board.h"
#include "ui.h"
#include "player.h"
#include "item.h"
#include "arbre.h"
#include "algorithm.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <limits.h>

int main() {
    // Initialisation
    srand(time(NULL));
    SetTraceLogLevel(LOG_DEBUG);

    bool aiThinking = false;
    float aiStartTime = 0;
    
    // Configuration initiale du jeu
    int connectN = 4;  
    int rows = 6;
    int cols = 7;
    bool useAI = false;
    
    // Dimensions de la fenetre
    int screenWidth = 1280;
    int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Puissance N");
    SetTargetFPS(60);
    
    // États du jeu
    GameState currentState = CONFIG;
    
    // Valeurs de configuration
    int configConnectN = 4;
    int configRows = 6;
    int configCols = 7;
    
    // Mise en page de l'interface
    int paramX = 80;
    int paramY = 150;
    int paramSpacing = 50;
    
    // Paramètres configurables
    Parameter connectNParam = {
        &configConnectN, 3, 10, "Jetons à connecter",
        {paramX + 300, paramY, BUTTON_SIZE, BUTTON_SIZE},
        {paramX + 300 + BUTTON_SIZE + 10, paramY, BUTTON_SIZE, BUTTON_SIZE}
    };
    
    Parameter rowsParam = {
        &configRows, 4, 20, "Lignes",
        {paramX + 300, paramY + paramSpacing, BUTTON_SIZE, BUTTON_SIZE},
        {paramX + 300 + BUTTON_SIZE + 10, paramY + paramSpacing, BUTTON_SIZE, BUTTON_SIZE}
    };
    
    Parameter colsParam = {
        &configCols, 4, 20, "Colonnes",
        {paramX + 300, paramY + paramSpacing * 2, BUTTON_SIZE, BUTTON_SIZE},
        {paramX + 300 + BUTTON_SIZE + 10, paramY + paramSpacing * 2, BUTTON_SIZE, BUTTON_SIZE}
    };
    
    // Éléments de l'interface
    Rectangle aiCheckbox = {paramX, paramY + paramSpacing * 3 + 10, 30, 30};
    Rectangle startButton = {screenWidth/2 - 150, screenHeight - 120, 300, 70};
    Rectangle restartButton = {screenWidth/2 - 150, screenHeight - 120, 300, 70};
    
    // Joueurs
    Player player1 = new_player("Joueur 1", 36); // 36 = '$' pour le joueur X
    Player player2 = new_player("Joueur 2", 35); // 35 = '#' pour le joueur O
    
    // Ajoute des couleurs aux joueurs pour l'affichage
    player1->color = RED;
    player2->color = YELLOW;
    
    Player currentPlayer = player1;
    
    // Plateau de jeu
    Board board = NULL;
    
    // Variables d'état
    bool gameOver = false;
    bool winner = false;
    int lastMoveRow = -1;
    int lastMoveCol = -1;
    int hoverColumn = -1;
    
    // Boucle principale
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        // Gestion des états du jeu
        switch (currentState) {
            case CONFIG:
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    // Gestion des paramètres
                    handleParameterClick(&connectNParam, mousePos);
                    handleParameterClick(&rowsParam, mousePos);
                    handleParameterClick(&colsParam, mousePos);
                    
                    // Gestion de la case à cocher IA
                    if (CheckCollisionPointRec(mousePos, aiCheckbox)) {
                        useAI = !useAI;
                        
                        // Utilise une chaîne statique pour le nom du joueur
                        static char player2NameBuffer[50];
                        strcpy(player2NameBuffer, useAI ? "Ordinateur" : "Joueur 2");
                        player2->name = player2NameBuffer;
                    }
                    
                    // Démarrage du jeu
                    if (isMouseOverButton(startButton)) {
                        // Valide la configuration
                        if (configConnectN > configRows && configConnectN > configCols) {
                            configConnectN = (configRows > configCols) ? configRows : configCols;
                        }
                        
                        // Initialise les paramètres
                        connectN = configConnectN;
                        rows = configRows;
                        cols = configCols;
                        
                        // Crée la grille
                        if (board != NULL) {
                            freeBoard(board);
                        }
                        board = create_new_Board(rows, cols);
                        
                        if (board == NULL) {
                            printf("Erreur: Impossible d'allouer de la mémoire pour la grille\n");
                            break;
                        }
                        
                        // Démarre le jeu
                        currentState = PLAY;
                        currentPlayer = player1;
                        gameOver = false;
                        winner = false;
                    }
                }
                break;
                
            case PLAY:
                if (board == NULL) {
                    currentState = CONFIG;
                    break;
                }
                
                // Détermine la colonne sous la souris
                if (mousePos.y >= BOARD_OFFSET_Y && 
                    mousePos.y <= BOARD_OFFSET_Y + board->row * CELL_SIZE && 
                    mousePos.x >= BOARD_OFFSET_X && 
                    mousePos.x <= BOARD_OFFSET_X + board->column * CELL_SIZE) {
                    hoverColumn = (mousePos.x - BOARD_OFFSET_X) / CELL_SIZE;
                    if (hoverColumn < 0 || hoverColumn >= board->column) {
                        hoverColumn = -1;
                    }
                } else {
                    hoverColumn = -1;
                }
                
                // Gestion de l'IA
                if (useAI && currentPlayer->signe == player2->signe) {
                    if (!aiThinking) {
                        aiThinking = true;
                        aiStartTime = GetTime();
                    } else if (GetTime() - aiStartTime >= 0.5f) {
                        // CONTROLE IMMÉDIAT POUR VICTOIRES OU BLOCAGES
                        int immediateMove = -1;
                        
                        // Vérifie d'abord si l'IA peut gagner en un coup
                        for (int j = 0; j < board->column; j++) {
                            if (board->board[0][j] == '.') { // Colonne jouable
                                // Trouve la ligne où tomberait le jeton
                                int i = -1;
                                for (int row = board->row - 1; row >= 0; row--) {
                                    if (board->board[row][j] == '.') {
                                        i = row;
                                        break;
                                    }
                                }
                                
                                if (i >= 0) {
                                    // Simule le coup
                                    board->board[i][j] = player2->signe;
                                    
                                    // Vérifie si c'est un coup gagnant
                                    if (checkWin(board, connectN)) {
                                        immediateMove = j; // Coup gagnant trouvé !
                                    }
                                    
                                    // Annule le coup
                                    board->board[i][j] = '.';
                                    
                                    if (immediateMove != -1) break; // Nous avons trouvé un coup gagnant
                                }
                            }
                        }
                        
                        // S'il n'y a pas de coup gagnant, vérifie s'il faut bloquer
                        if (immediateMove == -1) {
                            for (int j = 0; j < board->column; j++) {
                                if (board->board[0][j] == '.') { // Colonne jouable
                                    // Trouve la ligne où tomberait le jeton
                                    int i = -1;
                                    for (int row = board->row - 1; row >= 0; row--) {
                                        if (board->board[row][j] == '.') {
                                            i = row;
                                            break;
                                        }
                                    }
                                    
                                    if (i >= 0) {
                                        // Simule le coup de l'adversaire
                                        board->board[i][j] = player1->signe;
                                        
                                        // Vérifie si ce serait un coup gagnant pour l'adversaire
                                        if (checkWin(board, connectN)) {
                                            immediateMove = j; // Coup bloquant trouvé !
                                        }
                                        
                                        // Annule le coup
                                        board->board[i][j] = '.';
                                        
                                        if (immediateMove != -1) break; // Nous avons trouvé un coup bloquant
                                    }
                                }
                            }
                        }
                        
                        // Si nous avons trouvé un coup immédiat, utilisons-le
                        if (immediateMove != -1) {
                            printf("L'IA a trouvé un coup immédiat: %d\n", immediateMove);
                            lastMoveRow = play(board, immediateMove, player2);
                            lastMoveCol = immediateMove;
                            
                            // Vérifie victoire ou match nul
                            if (checkWin(board, connectN)) {
                                gameOver = true;
                                winner = true;
                                currentState = GAME_OVER;
                            } else if (!check_playable(board)) {
                                gameOver = true;
                                winner = false;
                                currentState = GAME_OVER;
                            } else {
                                currentPlayer = player1;
                            }
                            
                            aiThinking = false;
                        } else {
                            // Crée une copie profonde de la grille pour l'IA
                            Board boardCopy = copieBoard(board);

                            if (boardCopy != NULL) {
                                // Crée un nouvel item avec la copie de la grille
                                Item rootItem = create_empty_Item();
                                
                                if (rootItem != NULL) {
                                    // IMPORTANT: D'abord nous définissons la grille dans l'item
                                    setItem(rootItem, boardCopy, 0);  // La valeur 0 sera écrasée immédiatement
                                    
                                    // Calcule le score initial basé sur la position actuelle
                                    // C'est le point clé: nous calculons le score AVANT de créer l'arbre
                                    int initialScore = calculateScore(rootItem, connectN, player1, player2);
                                    rootItem->score = initialScore;  // Nous définissons le score calculé
                                    
                                    printf("Score initial calculé: %d\n", initialScore);
                                    
                                    // Crée l'arbre et définit l'item
                                    Arbre arbre = create_empty_arbre();
                                    
                                    if (arbre != NULL) {
                                        // Nous définissons l'item dans l'arbre
                                        setArbre(arbre, rootItem);
                                        
                                        // Adapte la profondeur en fonction du nombre d'espaces vides sur la grille
                                        int aiDepth = 6; // Valeur par défaut
                                        
                                        // Compte les espaces vides
                                        int emptySpaces = 0;
                                        for (int i = 0; i < board->row; i++) {
                                            for (int j = 0; j < board->column; j++) {
                                                if (board->board[i][j] == '.') {
                                                    emptySpaces++;
                                                }
                                            }
                                        }
                                        
                                        // Ajuste la profondeur en fonction de l'état du jeu
                                        if (board->column >= 10 || board->row >= 10) {
                                            // Les grilles très grands nécessitent moins de profondeur
                                            aiDepth = 4;
                                        } else if (emptySpaces < 15) {
                                            // Fin de partie: augmente la profondeur
                                            aiDepth = 7;
                                        } else if (emptySpaces > board->row * board->column - 10) {
                                            // Début de partie: profondeur moyenne
                                            aiDepth = 5;
                                        }
                                        
                                        // Debug: affiche l'état de l'arbre avant MINMAX
                                        printf("Score de l'arbre avant MINMAX: %d\n", arbre->item->score);
                                        printf("Profondeur MINMAX: %d\n", aiDepth);
                                        
                                        // Exécute minimax avec la profondeur adaptée
                                        int aiMove = MINMAX(arbre, aiDepth, player1, player2, connectN);
                                        
                                        // Debug: montre le coup choisi par l'IA et son score final
                                        printf("L'IA a choisi la colonne %d avec un score final de %d\n", aiMove, arbre->item->score);
                                        
                                        // Vérifie que l'arbre a des enfants
                                        if (arbre->fils != NULL) {
                                            int countFils = 0;
                                            Arbre fils = arbre->fils;
                                            while (fils != NULL) {
                                                printf("Enfant %d: colonne ? avec score %d\n", countFils, fils->item->score);
                                                countFils++;
                                                fils = fils->frere;
                                            }
                                            printf("Nombre d'enfants: %d\n", countFils);
                                        } else {
                                            printf("ATTENTION: L'arbre n'a pas d'enfants!\n");
                                        }
                                        
                                        // Libère l'arbre
                                        freeArbre(arbre);
                                        
                                        // Exécute le coup sur le plateau original
                                        if (aiMove >= 0 && aiMove < board->column && 
                                            board->board[0][aiMove] == '.') {
                                            // Exécute le coup
                                            lastMoveRow = play(board, aiMove, player2);
                                            lastMoveCol = aiMove;
                                            
                                            // Vérifie victoire ou match nul
                                            if (checkWin(board, connectN)) {
                                                gameOver = true;
                                                winner = true;
                                                currentState = GAME_OVER;
                                            } else if (!check_playable(board)) {
                                                gameOver = true;
                                                winner = false;
                                                currentState = GAME_OVER;
                                            } else {
                                                currentPlayer = player1;
                                            }
                                        } else {
                                            // Solution de repli: choisir la première colonne disponible
                                            printf("Coup de l'IA non valide, utilisation de la solution de repli\n");
                                            for (int j = 0; j < board->column; j++) {
                                                if (board->board[0][j] == '.') {
                                                    lastMoveRow = play(board, j, player2);
                                                    lastMoveCol = j;
                                                    
                                                    if (checkWin(board, connectN)) {
                                                        gameOver = true;
                                                        winner = true;
                                                        currentState = GAME_OVER;
                                                    } else {
                                                        currentPlayer = player1;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    } else {
                                        // Erreur lors de la création de l'arbre
                                        freeItem(rootItem); // Libère rootItem et boardCopy
                                        printf("Erreur: impossible de créer l'arbre\n");
                                    }
                                } else {
                                    // Erreur lors de la création de l'item
                                    freeBoard(boardCopy);
                                    printf("Erreur: impossible de créer l'item\n");
                                }
                            } else {
                                printf("Erreur: impossible de copier le plateau\n");
                            }
                            
                            aiThinking = false;  // réinitialisation pour la prochaine fois
                        }
                    }
                }
                // Coup du joueur humain
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (hoverColumn >= 0 && hoverColumn < board->column &&
                        board->board[0][hoverColumn] == '.') {
                        // Exécute le coup
                        lastMoveRow = play(board, hoverColumn, currentPlayer);
                        lastMoveCol = hoverColumn;
                        
                        // Vérifie victoire ou match nul en utilisant la fonction originale
                        if (checkWin(board, connectN)) {
                            gameOver = true;
                            winner = true;
                            currentState = GAME_OVER;
                        } else if (!check_playable(board)) {
                            gameOver = true;
                            winner = false;
                            currentState = GAME_OVER;
                        } else if (useAI) {
                            currentPlayer = player2;  // Passe à l'IA
                        } else {
                            // Alterne entre les deux joueurs humains
                            currentPlayer = (currentPlayer == player1) ? player2 : player1;
                        }
                    }
                }
                break;
                
            case GAME_OVER:
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseOverButton(restartButton)) {
                    currentState = CONFIG;
                }
                break;
        }
        
        // Dessin de l'interface
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            switch (currentState) {
                case CONFIG:
                    {
                        Parameter params[] = {connectNParam, rowsParam, colsParam};
                        drawConfigScreen(screenWidth, params, 3, startButton, useAI, aiCheckbox);
                    }
                    break;
                
                case PLAY:
                    drawGameScreen(board, currentPlayer, connectN, hoverColumn);
                    break;
                
                case GAME_OVER:
                    drawGameOverScreen(board, currentPlayer, !winner, connectN, restartButton);
                    break;
            }
        EndDrawing();
    }
    
    // Nettoyage
    if (board != NULL) {
        freeBoard(board);
    }
    free(player1);
    free(player2);
    CloseWindow();
    
    return 0;
}