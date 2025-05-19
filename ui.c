#include "ui.h"
#include "config.h"
#include <stdio.h>

// Fonction pour dessiner du texte
void drawText(const char* text, int x, int y, int fontSize, Color color) {
    if (text == NULL) {
        printf("Erreur: text est NULL dans drawText\n");
        return;
    }

    DrawText(text, x, y, fontSize, color);
}

// Fonction pour dessiner un bouton
void drawButton(Rectangle rect, const char* text, Color color, bool isHovered) {
    if (text == NULL) {
        printf("Erreur: text est NULL dans drawButton\n");
        return;
    }

    Color buttonColor = isHovered ? ColorBrightness(color, 0.2f) : color;
    DrawRectangleRec(rect, buttonColor);
    DrawRectangleLinesEx(rect, 2, ColorBrightness(buttonColor, -0.2f));

    int textWidth = MeasureText(text, 24);
    DrawText(text, rect.x + (rect.width - textWidth)/2, rect.y + rect.height/2 - 12, 24, WHITE);
}

// Fonction pour vérifier si la souris est sur un bouton
bool isMouseOverButton(Rectangle rect) {
    if (!IsWindowReady()) {
        printf("Debug: Fenetre non prête dans isMouseOverButton\n");
        return false;  // La fenetre n'est pas prête
    }

    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, rect);
}

// Fonction pour dessiner un paramètre avec des flèches
void drawParameter(Parameter param, int x, int y) {
    // Dessine l'étiquette et la valeur actuelle
    drawText(TextFormat("%s: %d", param.label, *(param.value)), x, y + 10, 24, BLACK);

    // Dessine le bouton pour augmenter
    DrawRectangleRec(param.upButton, DARKBLUE);
    
    // Calcule la position centrée du texte dans le bouton
    int upTextWidth = MeasureText("+", 24);
    DrawText("+", 
             param.upButton.x + (param.upButton.width - upTextWidth) / 2, 
             param.upButton.y + (param.upButton.height - 24) / 2, 
             24, WHITE);

    // Dessine le bouton pour diminuer
    DrawRectangleRec(param.downButton, DARKBLUE);
    
    // Calcule la position centrée du texte dans le bouton
    int downTextWidth = MeasureText("-", 24);
    DrawText("-", 
             param.downButton.x + (param.downButton.width - downTextWidth) / 2, 
             param.downButton.y + (param.downButton.height - 24) / 2, 
             24, WHITE);
             
    // Ajoute un texte qui indique la plage de valeurs disponibles
    drawText(TextFormat("(min: %d, max: %d)", param.min, param.max), 
             x + 400, y + 10, 18, DARKGRAY);
}

// Fonction pour gérer le clic sur les boutons des paramètres
bool handleParameterClick(Parameter* param, Vector2 mousePos) {
    // Vérifie si le clic est sur le bouton UP
    if(CheckCollisionPointRec(mousePos, param->upButton)) {
        if(*(param->value) < param->max) {
            (*(param->value))++;
            return true;
        }
    }
    // Vérifie si le clic est sur le bouton DOWN
    if (CheckCollisionPointRec(mousePos, param->downButton)){
        if(*(param->value) > param->min) {
            (*(param->value))--;
            return true;
        }
    }
    
    return false;
}

// Fonction pour dessiner le titre centré
void drawTitle(const char* text, int screenWidth) {
    drawText(text, screenWidth/2 - MeasureText(text, 60)/2, 30, 60, DARKBLUE);
}

// Fonction pour dessiner l'écran de configuration
void drawConfigScreen(int screenWidth, Parameter* params, int paramCount, Rectangle startButton, bool useAI, Rectangle aiCheckbox) {
    // Dessine le titre
    drawTitle("PUISSANCE N", screenWidth);

    // Dessine les instructions de configuration
    drawText("Configurez votre jeu:", screenWidth/2 - MeasureText("Configurez votre jeu:", 40)/2, 100, 40, BLACK);

    // Dessine les controles de configuration avec boutons
    int paramX = 80;
    int paramY = 150;
    int paramSpacing = 50;
    
    for (int i = 0; i < paramCount; i++) {
        drawParameter(params[i], paramX, paramY + i * paramSpacing);
    }

    // Dessine la case à cocher pour l'IA
    DrawRectangleLines(aiCheckbox.x, aiCheckbox.y, aiCheckbox.width, aiCheckbox.height, BLACK);
    if (useAI) {
        DrawRectangle(aiCheckbox.x + 5, aiCheckbox.y + 5, 
                     aiCheckbox.width - 10, aiCheckbox.height - 10, BLACK);
    }
    drawText("Jouer contre l'IA", aiCheckbox.x + 40, aiCheckbox.y + 5, 24, BLACK);

    // Dessine le bouton de démarrage
    drawButton(startButton, "COMMENCER", DARKBLUE, isMouseOverButton(startButton));
}

// Fonction pour vérifier si un coup est valide (implémentée localement)
bool isValidMove(Board board, int col) {
    if (board == NULL) return false;
    if (col < 0 || col >= board->column) return false;
    return board->board[0][col] == '.';
}

// Fonction pour dessiner le plateau
void drawBoard(Board board) {
    if (board == NULL) {
        printf("Erreur: board est NULL dans drawBoard\n");
        return;
    }

    // Dessine l'arrière-plan du plateau
    DrawRectangle(
        BOARD_OFFSET_X - 10,
        BOARD_OFFSET_Y - 10,
        board->column * CELL_SIZE + 20,
        board->row * CELL_SIZE + 20,
        DARKBLUE
    );

    // Dessine les cellules
    for (int r = 0; r < board->row; r++) {
        for (int c = 0; c < board->column; c++) {
            // Calcule la position du centre de la cellule
            int centerX = BOARD_OFFSET_X + c * CELL_SIZE + CELL_SIZE/2;
            int centerY = BOARD_OFFSET_Y + r * CELL_SIZE + CELL_SIZE/2;

            // Dessine l'arrière-plan de la cellule
            DrawCircle(centerX, centerY, CELL_SIZE/2 - 5, RAYWHITE);

            // Dessine les jetons
            if (board->board[r][c] == '$' || board->board[r][c] == 36) {
                DrawCircle(centerX, centerY, CELL_SIZE/2 - 5, RED);
            } else if (board->board[r][c] == '#' || board->board[r][c] == 35) {
                DrawCircle(centerX, centerY, CELL_SIZE/2 - 5, YELLOW);
            }
        }
    }

    // Dessine les étiquettes des colonnes
    for (int c = 0; c < board->column; c++) {
        DrawText(
            TextFormat("%d", c + 1),
            BOARD_OFFSET_X + c * CELL_SIZE + CELL_SIZE/2 - 5,
            BOARD_OFFSET_Y - 30,
            20,
            DARKGRAY
        );
    }
}

// Fonction pour dessiner l'écran de jeu
void drawGameScreen(Board board, Player currentPlayer, int connectN, int hoverColumn) {
    // Dessine le titre
    drawText(TextFormat("PUISSANCE %d", connectN), 30, 30, 40, DARKBLUE);

    // Dessine les informations sur le tour et le nombre de pièces à connecter
    drawText(TextFormat("Tour: %s", currentPlayer->name), 30, 80, 30, currentPlayer->color);
    drawText(TextFormat("Pièces à connecter: %d", connectN), 30, 120, 24, DARKBLUE);

    // Dessine l'aperçu du coup
    if (hoverColumn >= 0 && hoverColumn < board->column && isValidMove(board, hoverColumn)) {
        DrawCircle(
            BOARD_OFFSET_X + hoverColumn * CELL_SIZE + CELL_SIZE/2,
            BOARD_OFFSET_Y - 40,
            CELL_SIZE/2 - 5,
            Fade(currentPlayer->color, 0.5f)
        );
    }
    
    // Dessine le plateau
    drawBoard(board);
}

// Fonction pour dessiner l'écran de fin de jeu
void drawGameOverScreen(Board board, Player winner, bool isDraw, int connectN, Rectangle restartButton) {
    // Dessine le titre
    drawText(TextFormat("PUISSANCE %d", connectN), 30, 30, 40, DARKBLUE);

    // État GAME_OVER
    if (!isDraw) {
        drawText(TextFormat("%s a gagné!", winner->name), 30, 80, 36, winner->color);
    } else {
        drawText("Match nul!", 30, 80, 36, DARKGRAY);
    }

    // Dessine le bouton de redémarrage
    drawButton(restartButton, "REJOUER", DARKBLUE, isMouseOverButton(restartButton));
    
    // Dessine le plateau
    drawBoard(board);
}