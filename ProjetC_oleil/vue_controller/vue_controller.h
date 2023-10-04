#pragma once

#define true 1
#define false 0

#define RED {255, 0, 0, 255}
#define GREEN {0, 255, 0, 255}
#define BLUE {0, 0, 255, 255}
#define WHITE {255, 255, 255, 255}
#define BLACK {0, 0, 0, 255}
#define YELLOW {255, 255, 0, 255}

#include <SDL2/SDL.h>
#include "../model/model.h"

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
} GameObjects;

// Structure that contains the game itself (the window, the rendering, etc.)
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // Renderering struct
    SDL_Event* event; // Allow to check if input from the user
    int isGameRunning;
    GameObjects* gameObjects;
    Clock clock;
} Game;

// init the game (create the window and the rendering struct)
Game init();
void run(Game *game);
void update(Game *game);
void render(Game *game);
int drawPlanet(SDL_Renderer *render, Planet *planet, int filled, SDL_Color *color);