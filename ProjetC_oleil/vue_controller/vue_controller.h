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
 * This structure contains all the necessary values for the clock and handle time
 */
typedef struct Clock_s {
    double DELTA_TIME;
    Uint32 startMillis, currentMillis;
} Clock;

/**
 * This structure contains all game object (like the planet, star, universe, etc...)
 */
typedef struct GameContent_s {
    char* gameTitleBuffer; // The buffer that will contains the title of the game with FPS and Score
    Universe* universe; // The universe (solar system, ship, etc...)
    int score; // The score of the player
} GameObjects;

/**
 * This structure contains all the necessary values for the game
 * This struct allow to pass all necessary values to the functions without passing all values one by one
 */
typedef struct Game_s {
    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // Renderering struct
    SDL_Event* event; // Allow to check if input from the user
    int isGameRunning; // int, 1 if the game is running, 0 neither
    GameObjects* gameObjects; // All the objects of the game that we will use
    Color* color; // The color struct that contains all the color that we will use in the game
    Clock clock; // Clock struct that handle the time spent for the game (manage Framerate, update and rendering concept)
    int WINDOW_HEIGHT;
    int WINDOW_LENGHT;
} Game;

/**
* This function initialize some values related to the "Game" structure
*
* @return a game structure
*/
Game* init();
/**
 * This function initialize the color struct
 * @param color
 */
void initColor(Color *color);
/**
* This function launch the game, initialize necessary values for allow the game to run and start the loop.
*
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void run(Game *game);
/**
* This function is called before "the render function" each 16.7MS(60FPS), all content of the game have to be updated here(like for movement, collisions, etc.)
*
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void update(Game *game);
/**
* This function is called at the end of the loop, it handle all thing related to SDL for rendering the window, there must be nothing here that edit some values.
*
* @param game The game structure that contains all necessary elements for allow the game to run
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
/**
 * This function draw a star
 * @param render SDL render
 * @param star The star wa have to draw
 * @param filled If the star has to be filled or not
 * @param color The color of the star
 */
int drawStar(SDL_Renderer *render, Star star, int filled, SDL_Color *color);
/**
 * This function draw a line (a vector), it's used for draw the direction vector of the ship
 * @param renderer SDL Render
 * @param angle The angle of the vector we want to draw
 * @param ship The ship from the vector will be draw
 */
void drawVector(SDL_Renderer *renderer, float angle, SDL_FRect ship);
/**
 * This function check collisions between ship and planets
 * The function simulate that the planet is a rectangle and check if the ship is in the rectangle
 * @param ship The ship
 * @param planet The planet
 * @return Boolean, true if there collisions, false neither
 */
bool checkCollision(Ship *ship, Planet *planet);
