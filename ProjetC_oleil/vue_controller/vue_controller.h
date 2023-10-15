#pragma once

#define true 1
#define false 0

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../model/model.h"
#include <stdio.h>
#include <SDL2/SDL2_gfxPrimitives.h>

/**
 * This structure contains all the color that we will use in the game
 */
typedef struct Color {
    SDL_Color* RED;
    SDL_Color* GREEN;
    SDL_Color* BLUE;
    SDL_Color* WHITE;
    SDL_Color* BLACK;
    SDL_Color* YELLOW;
    SDL_Color* CYAN;
    SDL_Color* HALF_WHITE;
} Color;
/**
 * This structure contains all the necessary values for the clock and time managment
 */
typedef struct Clock_s {
    double DELTA_TIME;
    Uint32 startMillis, currentMillis;
} Clock;

/**
 * This structure contains all game object (like the planet, star, universe, etc...)
 */
typedef struct GameContent_s {
    char* gameTitleBuffer;
    SDL_Rect* rectangle;
    Planet *planet1;
    Planet *planet2;
    Universe* universe;
} GameObjects;

/**
 * This structure contains all the necessary values for the game
 * This struct allow to pass all necessary values to the functions without passing all values one by one
 */
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // Renderering struct
    SDL_Event* event; // Allow to check if input from the user
    int isGameRunning;
    GameObjects* gameObjects;
    Color* color;
    Clock clock;
} Game;

/**
 * This function launch the game, initialize necessary values for allow the game to run and start the loop.
 * @param color
 */
Game* init();
/**
 * This function initialize the color struct
 * @param color
 */
void initColor(Color *color);
/**
 * Thus function launch the game (the loop and initialize all necessary values like planet ,etc..)
 * @param game
 */
void run(Game *game);
/**
 * This function update the game (update the position of the ship, etc..)
 * @param game
 */
void update(Game *game);
/**
 * This function render the game (render the ship, the planet, etc..)
 * @param game
 */
void render(Game *game);
/**
 * This function draw a planet
 * @param render SDL Render
 * @param planet The planet to draw
 * @param filled If the planet is filled or not
 * @param color The color of the planet (Use the struct Color for call it by name)
 */
int drawPlanet(SDL_Renderer *render, Planet planet, int filled, SDL_Color *color);

int drawStar(SDL_Renderer *render, Star star, int filled, SDL_Color *color);
