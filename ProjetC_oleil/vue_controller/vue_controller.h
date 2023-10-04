#pragma once
#include <SDL2/SDL.h>
#include <stdbool.h>

// Structure that contains all things related to handle the time of the game
typedef struct Clock_s {
    double DELTA_TIME;
    Uint32 startMillis, currentMillis;
} Clock;

// All objects that the game will contains (rectangle, planet, etc.)
typedef struct GameContent_s {
    char* gameTitleBuffer;
    SDL_Rect* rectangle;
} GameObjects;

// Structure that contains the game itself (the window, the rendering, etc.)
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // Renderering struct
    SDL_Event* event; // Allow to check if input from the user
    bool isGameRunning;
    GameObjects* gameObjects;
    Clock clock;
} Game;

// init the game (create the window and the rendering struct)
Game init();
void run(Game *game);
void update(Game *game);
void render(Game *game);