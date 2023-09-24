#pragma once
#include <SDL.h>
#include <stdbool.h>

// Structure that contains all things related to handle the time of the game
typedef struct Clock_s {
    float DELTA_TIME;
    Uint32 startMillis, currentMillis;
} Clock;

// All objects that the game will contains (rectangle, planet, etc.)
typedef struct GameContent_s {
    char *gameTitleBuffer[1024];
    SDL_Rect* rectangle;
} GameObjects;

// Structure that contains the game itself (the window, the rendering, etc.)
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // i don't know what is this
    bool isGameRunning;
    GameObjects* gameObjects;
    Clock clock;
} Game;

// init the game (create the window and the rendering struct)
Game init();

