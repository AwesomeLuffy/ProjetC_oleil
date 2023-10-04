#include <SDL2/SDL.h>
#include "vue_controller/vue_controller.h"

int main(int argc, char *argv[])
{

    Game *game;

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Init the window and the rendering
    game = init();

    // Run the game
    run(game);

    // When game ended, destroy all
    SDL_DestroyWindow(game->window);
    SDL_DestroyRenderer(game->render);
    SDL_Quit();

    return 0;
}

