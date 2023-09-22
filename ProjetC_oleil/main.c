#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include "model/model.h"
#include <stdbool.h>


int main(int argc, char *argv[ ])
{
    const float DELTA_TIME = 1000 / 60.0; //60FPS
    const int VELOCITY = -2;

    SDL_Window* window; // Actual Window that will contains the size and other parameters
    SDL_Renderer* render; // i don't know what is this
    SDL_Event event; // Allow to check if input from the user

    Uint32 startMillis, currentMillis; // To handle time
    SDL_Rect rectangle = { 200, 200, 100, 100 }; // Definition of a test rectangle

    bool isGameRunning = true; // Bool to check if game is running

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Initialize the startMillis
    startMillis = SDL_GetTicks();

    window = SDL_CreateWindow("Game test",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        800,
        SDL_WINDOW_SHOWN);

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    while (isGameRunning)
    {
        // Get for how many time the game run
        currentMillis = SDL_GetTicks();

        //Part to handle user interaction
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type) {
                case SDL_QUIT:
                case SDLK_ESCAPE:
                case SDLK_q:
                    isGameRunning = false;
                    break;
            }

        }

        // Each 16,7ms (so to allow the game to run at 60FPS) we process to an update
        if (currentMillis - startMillis >= DELTA_TIME) {

            SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

            SDL_RenderClear(render);
            rectangle.x += 2 * VELOCITY;

            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

            SDL_RenderFillRect(render, &rectangle);

            SDL_RenderPresent(render);

            // This part is for printing FPS on the top of the window
            if (currentMillis - startMillis > 0) {
                char buffer[1024];

                // Replace "%f" with the FPS, the function change the value of the char buffer
                snprintf(buffer, sizeof(buffer), "ProjetC_oleil | FPS : %f | Lol", (1000.0 / (currentMillis - startMillis)));

                // Change the window title
                SDL_SetWindowTitle(window, buffer);

            }

            // Reset startMillis to check the time
            startMillis = SDL_GetTicks();

        }


    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();

    return 0;
}