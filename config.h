#ifndef CONFIG_H
#define CONFIG_H

// Constantes pour l'interface graphique
#define CELL_SIZE 80
#define BOARD_OFFSET_X 100
#define BOARD_OFFSET_Y 200
#define SCREEN_PADDING 80
#define BUTTON_SIZE 40

// États du jeu
typedef enum {
    CONFIG,
    PLAY,
    GAME_OVER
} GameState;

#endif //CONFIG_H