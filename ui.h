#ifndef UI_H
#define UI_H

#include "raylib.h"
#include "board.h"
#include "player.h"
#include <stdbool.h>

// Structure pour les paramètres modifiables avec des flèches
typedef struct {
    int* value;           // Pointeur vers la valeur à modifier
    int min;              // Valeur minimale
    int max;              // Valeur maximale
    const char* label;    // Étiquette du paramètre
    Rectangle upButton;   // Bouton pour incrémenter
    Rectangle downButton; // Bouton pour décrémenter
} Parameter;

// Fonctions de base pour l'interface utilisateur
void drawText(const char* text, int x, int y, int fontSize, Color color);
void drawButton(Rectangle rect, const char* text, Color color, bool isHovered);
bool isMouseOverButton(Rectangle rect);

// Fonctions pour gérer les paramètres
void drawParameter(Parameter param, int x, int y);
bool handleParameterClick(Parameter* param, Vector2 mousePos);

// Fonctions de mise en page simplifiées
void drawTitle(const char* text, int screenWidth);
void drawConfigScreen(int screenWidth, Parameter* params, int paramCount, Rectangle startButton, bool useAI, Rectangle aiCheckbox);
void drawGameScreen(Board board, Player currentPlayer, int connectN, int hoverColumn);
void drawGameOverScreen(Board board, Player winner, bool isDraw, int connectN, Rectangle restartButton);

#endif // UI_H