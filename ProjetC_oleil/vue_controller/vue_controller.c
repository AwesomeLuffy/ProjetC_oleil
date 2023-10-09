#include "vue_controller.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>


void initColor(Color *color) {
    SDL_Color *RED = malloc(sizeof(SDL_Color));
    SDL_Color *GREEN = malloc(sizeof(SDL_Color));
    SDL_Color *BLUE = malloc(sizeof(SDL_Color));
    SDL_Color *WHITE = malloc(sizeof(SDL_Color));
    SDL_Color *BLACK = malloc(sizeof(SDL_Color));
    SDL_Color *YELLOW = malloc(sizeof(SDL_Color));

    *RED = (SDL_Color) {255, 0, 0, 255};
    *GREEN = (SDL_Color) {0, 255, 0, 255};
    *BLUE = (SDL_Color) {0, 0, 255, 255};
    *WHITE = (SDL_Color) {255, 255, 255, 255};
    *BLACK = (SDL_Color) {0, 0, 0, 255};
    *YELLOW = (SDL_Color) {255, 255, 0, 255};

    color->RED = RED;
    color->GREEN = GREEN;
    color->BLUE = BLUE;
    color->WHITE = WHITE;
    color->BLACK = BLACK;
    color->YELLOW = YELLOW;

}

/**
* This function initialize some values related to the "Game" structure
*
* @return a game structure
*/
Game* init() {
    Game *game = malloc(sizeof(Game));
    // Create the window with the specified size, if we add "SDL_WINDOW_RESIZABLE" flags, it will allow to resize the window during the game
    game->window = SDL_CreateWindow("Game test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 800,
                                    SDL_WINDOW_SHOWN);
    // The renderer struct
    game->render = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    // Initialize the event structure
    game->event = malloc(sizeof(SDL_Event));
    // To allow the loop to run we initialize it to true
    game->isGameRunning = true;
    // To not have a nullptr when we will want to use the gameObjects, we allocate memory for it
    game->gameObjects = malloc(sizeof(GameObjects));
    game->gameObjects->gameTitleBuffer = malloc(1024);

    game->color = malloc(sizeof(Color));
    initColor(game->color);
    game->gameObjects = (GameObjects *) calloc(2, sizeof(GameObjects));
    game->gameObjects->gameTitleBuffer = malloc(1024);
    game->gameObjects->universe = initUniverse();

    return game;
}

/**
* This function is called before "the render function" each 16.7MS(60FPS), all content of the game have to be updated here(like for movement, collisions, etc.)
* 
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void update(Game *game) {

    // Part to handle user interaction
    while (SDL_PollEvent(game->event) != 0) {
        switch (game->event->type) {
            case SDL_KEYDOWN:
                switch (game->event->key.keysym.sym) {
                    case SDL_QUIT:
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        game->isGameRunning = false;
                        break;
                }

        }
    }

    if (game->clock.currentMillis - game->clock.startMillis > 0) {

        // Replace "%f" with the FPS, the function change the value of the char gameTitleBuffer
        snprintf(game->gameObjects->gameTitleBuffer,
                 sizeof(char[1024]),
                 "ProjetC_oleil | FPS : %f | Lol",
                 (1000.0 / (game->clock.currentMillis - game->clock.startMillis))
        );
    }

    // Update the pos of the second planet
    rotateObjectArroundAnother(game->gameObjects->planet2, game->gameObjects->planet1, 1 * M_PI / 180);

}

/**
* This function is called at the end of the loop, it handle all thing related to SDL for rendering the window, there must be nothing here that edit some values.
*
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void render(Game *game) {

    // Change the actual "pen" color for drawing something
    SDL_SetRenderDrawColor(game->render, 0, 0, 0, 0);

    //Clear all elements in the actual frame and set the backgrond in black.
    SDL_RenderClear(game->render);

    // Pen color in white
    SDL_SetRenderDrawColor(game->render, 255, 255, 255, 0);

    // Draw the start square
    SDL_RenderFillRect(game->render, &(game->gameObjects->universe->rectStart));

    //Draw the end square
    SDL_RenderFillRect(game->render, &(game->gameObjects->universe->rectEnd));

    // Pen color in red
    SDL_SetRenderDrawColor(game->render, 0, 255, 0, 0);

    // Draw the ship square
    SDL_RenderFillRect(game->render, &(game->gameObjects->universe->ship.rectShip));

    // Change the window title to show game name, fps , etc.)
    SDL_SetWindowTitle(game->window, game->gameObjects->gameTitleBuffer);

    for(int i = 0; i < game->gameObjects->universe->nbSolarSystem; i++){
        drawStar(game->render, game->gameObjects->universe->solarSystem[i].star, 1, game->color->YELLOW);
        for(int j = 0; j < game->gameObjects->universe->solarSystem[i].nbPlanet; j++){
            drawPlanet(game->render, game->gameObjects->universe->solarSystem[i].planets[j], 1, game->color->BLUE);
        }
    }
    // Update the screen
    SDL_RenderPresent(game->render);

}

/**
* This function launch the game, initialize necessary values for allow the game to run and start the loop.
* 
* @param game The game structure that contains all necessary elements for allow the game to run 
*/
void run(Game *game) {


    SDL_Rect rectangle = {300, 675, 10, 10}; // Definition of a test rectangle
    // Set the rectangle as a gameObjects to update and render it
    game->gameObjects->rectangle = &rectangle;
    Coord planet1Coord = {500, 500};
    Planet planet1 = {planet1Coord, 10, 10};
    game->gameObjects->planet1 = &planet1;
    Coord planet2Coord = {500, 450};
    Planet planet2 = {planet2Coord, 10, 10};
    game->gameObjects->planet2 = &planet2;

    // Set the delta time to 60 FPS, so each update will be set each 16,7Ms
    game->clock.DELTA_TIME = 1000 / 60.0;

    // Get for how many time SDL start running for determine the time
    game->clock.startMillis = SDL_GetTicks();

    // Game loop
    while (game->isGameRunning) {
        // Get for how many time the game run
        game->clock.currentMillis = SDL_GetTicks();

        // Each 16,7ms (so to allow the game to run at 60FPS) we process to an update
        if (game->clock.currentMillis - game->clock.startMillis >= game->clock.DELTA_TIME) {

            update(game);


            // Render all elements
            render(game);

            // Reset startMillis to check the time
            game->clock.startMillis = SDL_GetTicks();

        }

    }

}

int drawPlanet(SDL_Renderer *render, Planet planet, int filled, SDL_Color *color) {
    if (filled) {
        return filledCircleRGBA(render, planet.pos.x,
                                planet.pos.y,
                                planet.radius, color->r, color->g, color->b, color->a);
    } else {
        return circleRGBA(render, planet.pos.x,
                          planet.pos.y,
                          planet.radius, color->r, color->g, color->b, color->a);
    }
}

int drawStar(SDL_Renderer *render, Star star, int filled, SDL_Color *color) {
    if (filled) {
        return filledCircleRGBA(render, star.pos.x,
                                star.pos.y,
                                star.radius, color->r, color->g, color->b, color->a);
    } else {
        return circleRGBA(render, star.pos.x,
                          star.pos.y,
                          star.radius, color->r, color->g, color->b, color->a);
    }
}
