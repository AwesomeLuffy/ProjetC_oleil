#pragma once

#define true 1
#define false 0

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../model/model.h"

typedef struct Color {
    SDL_Color* RED;
    SDL_Color* GREEN;
    SDL_Color* BLUE;
    SDL_Color* WHITE;
    SDL_Color* BLACK;
    SDL_Color* YELLOW;
    SDL_Color* CYAN;
} Color;
// Structure that contains all things related to handle the time of the game
typedef struct Clock_s {
    double DELTA_TIME;
    Uint32 startMillis, currentMillis;
} Clock;

// All objects that the game will contains (rectangle, planet, etc.)
typedef struct GameContent_s {
    char* gameTitleBuffer;
    SDL_Rect* rectangle;
    Planet *planet1;
    Universe* universe;
} GameObjects;

// Structure that contains the game itself (the window, the rendering, etc.)
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // Renderering struct
    SDL_Event* event; // Allow to check if input from the user
    int isGameRunning;
    GameObjects* gameObjects;
    Color* color;
    Clock clock;
} Game;

// init the game (create the window and the rendering struct)
Game* init();
void initColor(Color *color);
void run(Game *game);
void update(Game *game);
void render(Game *game);
int drawPlanet(SDL_Renderer *render, Planet *planet, int filled, SDL_Color *color);