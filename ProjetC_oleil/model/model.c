#include "model.h"

/**
* This function initialize some values related to the "Game" structure
* 
* @return a game structure
*/
Game init() {
    Game game;
    // Create the window with the specified size, if we add "SDL_WINDOW_RESIZABLE" flags, it will allow to resize the window during the game
    game.window = SDL_CreateWindow("Game test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 800, SDL_WINDOW_SHOWN);
    // The renderer struct
    game.render = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
    // Initialize the event structure
    game.event = malloc(sizeof(SDL_Event));
    // To allow the loop to run we initialize it to true
    game.isGameRunning = true;
    // To not have a nullptr when we will want to use the gameObjects, we allocate memory for it
    game.gameObjects = malloc(sizeof(GameObjects));
    return game;
}