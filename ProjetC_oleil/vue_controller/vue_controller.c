#pragma once
#include "../model/model.h"
#include <SDL.h>
#include <stdio.h>

/**
* This function is called before "the render function" each 16.7MS(60FPS), all content of the game have to be updated here(like for movement, collisions, etc.)
* 
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void update(Game* game) {

    // Part to handle user interaction
    while (SDL_PollEvent(game->event) != 0)
    {
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
            sizeof(game->gameObjects->gameTitleBuffer),
            "ProjetC_oleil | FPS : %f | Lol",
            (1000.0 / (game->clock.currentMillis - game->clock.startMillis))
        );


    }
}

/**
* This function is called at the end of the loop, it handle all thing related to SDL for rendering the window, there must be nothing here that edit some values.
*
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void render(Game* game) {
    // Change the actual "pen" color for drawing something
    SDL_SetRenderDrawColor(game->render, 0, 0, 0, 0);

    //Clear all elements in the actual frame and set the backgrond in black.
    SDL_RenderClear(game->render);

    // Pen color in red
    SDL_SetRenderDrawColor(game->render, 255, 0, 0, 0);

    // Create a rectangle (that will be red cause of the previous line)
    SDL_RenderFillRect(game->render, game->gameObjects->rectangle);

    // Change the window title to show game name, fps , etc.)
    SDL_SetWindowTitle(game->window, game->gameObjects->gameTitleBuffer);

    // Update the screen
    SDL_RenderPresent(game->render);


}

/**
* This function launch the game, initialize necessary values for allow the game to run and start the loop.
* 
* @param game The game structure that contains all necessary elements for allow the game to run 
*/
void run(Game *game){


    SDL_Rect rectangle = { 300, 675, 10, 10 }; // Definition of a test rectangle
    // Set the rectangle as a gameObjects to update and render it
    game->gameObjects->rectangle = &rectangle;


    // Set the delta time to 60 FPS, so each update will be set each 16,7Ms
    game->clock.DELTA_TIME = 1000 / 60.0;

    // Get for how many time SDL start running for determine the time
    game->clock.startMillis = SDL_GetTicks();

    // Game loop
    while (game->isGameRunning)
    {
        // Get for how many time the game run
        game->clock.currentMillis = SDL_GetTicks();

        // Each 16,7ms (so to allow the game to run at 60FPS) we process to an update
        if (game->clock.currentMillis - game->clock.startMillis >= game->clock.DELTA_TIME) {

            update(game);

            // Reset startMillis to check the time
            game->clock.startMillis = SDL_GetTicks();

        }

        // Render all elements
        render(game);
    }

}

