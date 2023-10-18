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
    SDL_Color *HALF_WHITE = malloc(sizeof(SDL_Color));

    *RED = (SDL_Color) {255, 0, 0, 255};
    *GREEN = (SDL_Color) {0, 255, 0, 255};
    *BLUE = (SDL_Color) {0, 0, 255, 255};
    *WHITE = (SDL_Color) {255, 255, 255, 255};
    *BLACK = (SDL_Color) {0, 0, 0, 255};
    *YELLOW = (SDL_Color) {255, 255, 0, 255};
    *HALF_WHITE = (SDL_Color) {255, 255, 255, 128};

    color->RED = RED;
    color->GREEN = GREEN;
    color->BLUE = BLUE;
    color->WHITE = WHITE;
    color->BLACK = BLACK;
    color->YELLOW = YELLOW;
    color->HALF_WHITE = HALF_WHITE;

}

/**
* This function initialize some values related to the "Game" structure
*
* @return a game structure
*/
Game *init() {
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

    game->WINDOW_LENGHT = game->gameObjects->universe->WINSIZE.x;
    game->WINDOW_HEIGHT = game->gameObjects->universe->WINSIZE.y;


    return game;
}

/**
* This function is called before "the render function" each 16.7MS(60FPS), all content of the game have to be updated here(like for movement, collisions, etc.)
* 
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void update(Game *game) {
    static Vector vector[100];
    static int planetCount = 0;
    static Vector vectorPlanetSum;
    static float lenght;

    static Vector totalGravityShipVector;
    totalGravityShipVector.vector.x = 0;
    totalGravityShipVector.vector.y = 0;

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
                        // Rotate the ship to the left
                    case SDLK_LEFT:
                        game->gameObjects->universe->ship.angle -= .2;
                        break;
                        // Rotate the ship to the right
                    case SDLK_RIGHT:
                        game->gameObjects->universe->ship.angle += .2;
                        break;
                }

        }
    }
    ///////////////////////
    // Change the title of the window with FPS and Score
    if (game->clock.currentMillis - game->clock.startMillis > 0) {

        // Replace "%f" with the FPS, the function change the value of the char gameTitleBuffer
        snprintf(game->gameObjects->gameTitleBuffer,
                 sizeof(char[1024]),
                 "ProjetC_oleil | FPS : %.02f | Lol",
                 (1000.0 / (game->clock.currentMillis - game->clock.startMillis))
        );
    }

    ///////////////////////
    // Part that handle the rotation of the planet arround Star
        planetCount = 0;
        for (int i = 0; i < game->gameObjects->universe->nbSolarSystem; i++) {
            for (int j = 0; j < game->gameObjects->universe->solarSystem[i].nbPlanet; j++) {
                rotateObjectArroundAnother(&game->gameObjects->universe->solarSystem[i].planets[j],
                                           &game->gameObjects->universe->solarSystem[i].star,
                                           &game->gameObjects->universe->solarSystem[i].planets[j].angle);
                if(checkCollision(&game->gameObjects->universe->ship, &game->gameObjects->universe->solarSystem[i].planets[j])){
                    printf("test");
                    game->isGameRunning = false;
                    return;
                }
                if (j + 1 <= game->gameObjects->universe->solarSystem[i].nbPlanet) {
                    Vector gravityShipVector = additionVectorWithGravityAndAngle(game->gameObjects->universe->ship,
                                                                                 game->gameObjects->universe->solarSystem[i].planets[j]);
                    totalGravityShipVector.vector.x += gravityShipVector.vector.x;
                    totalGravityShipVector.vector.y += gravityShipVector.vector.y;

                    planetCount++;
                }

            }
        }

        // This part handle the movement of the ship
        if (planetCount != 0) {

            game->gameObjects->universe->ship.rectShip.x += totalGravityShipVector.vector.x * 0.07;
            game->gameObjects->universe->ship.rectShip.y += totalGravityShipVector.vector.y * 0.07;

            game->gameObjects->universe->ship.directionGravityVector = totalGravityShipVector;




        }
    game->gameObjects->universe->ship.directionVector.vector.x = cos(game->gameObjects->universe->ship.angle);
    game->gameObjects->universe->ship.directionVector.vector.y = sin(game->gameObjects->universe->ship.angle);

        game->gameObjects->universe->ship.rectShip.x +=
                game->gameObjects->universe->ship.directionVector.vector.x * 10 * .1;
        game->gameObjects->universe->ship.rectShip.y +=
                game->gameObjects->universe->ship.directionVector.vector.y* 10 * .1;
        ////////////////////
        // This part handle the max X and max Y of the ship
        if (game->gameObjects->universe->ship.rectShip.x > game->WINDOW_LENGHT) {
            game->gameObjects->universe->ship.rectShip.x = 0;
        }
        if (game->gameObjects->universe->ship.rectShip.x < 0) {
            game->gameObjects->universe->ship.rectShip.x = game->WINDOW_LENGHT;
        }
        if (game->gameObjects->universe->ship.rectShip.y > game->WINDOW_HEIGHT) {
            game->gameObjects->universe->ship.rectShip.y = 0;
        }
        if (game->gameObjects->universe->ship.rectShip.y < 0) {
            game->gameObjects->universe->ship.rectShip.y = game->WINDOW_HEIGHT;
        }



}

/**
* This function is called at the end of the loop, it handle all thing related to SDL for rendering the window, there must be nothing here that edit some values.
*
* @param game The game structure that contains all necessary elements for allow the game to run
*/
void render(Game *game) {
    static Planet actualPlanetLine;

    // Change the actual "pen" color for drawing something
    SDL_SetRenderDrawColor(game->render, 0, 0, 0, 0);

    //Clear all elements in the actual frame and set the backgrond in black.
    SDL_RenderClear(game->render);

    // Pen color in white
    SDL_SetRenderDrawColor(game->render, 255, 255, 255, 255);

    // Draw the start square
    SDL_RenderFillRect(game->render, &(game->gameObjects->universe->rectStart));

    //Draw the end square
    SDL_RenderFillRect(game->render, &(game->gameObjects->universe->rectEnd));


    // Pen color in red
    SDL_SetRenderDrawColor(game->render, 0, 255, 0, 255);

    // Draw the ship square
    SDL_RenderFillRectF(game->render, &(game->gameObjects->universe->ship.rectShip));


    SDL_SetRenderDrawColor(game->render, 255, 0, 0, 255);

    // Change the window title to show game name, fps , etc.)
    SDL_SetWindowTitle(game->window, game->gameObjects->gameTitleBuffer);


    // Render the planets and the star
    for (int i = 0; i < game->gameObjects->universe->nbSolarSystem; i++) {
        drawStar(game->render, game->gameObjects->universe->solarSystem[i].star, 1, game->color->YELLOW);
        for (int j = 0; j < game->gameObjects->universe->solarSystem[i].nbPlanet; j++) {
            drawPlanet(game->render, game->gameObjects->universe->solarSystem[i].planets[j], 1, game->color->BLUE);

            actualPlanetLine = game->gameObjects->universe->solarSystem[i].planets[j];
            actualPlanetLine.pos.x = game->gameObjects->universe->solarSystem[i].star.pos.x;
            actualPlanetLine.pos.y = game->gameObjects->universe->solarSystem[i].star.pos.y;
            actualPlanetLine.radius = game->gameObjects->universe->solarSystem[i].planets[j].distance_to_star;
            drawPlanet(game->render, actualPlanetLine, 0, game->color->HALF_WHITE);
        }
    }

    SDL_SetRenderDrawColor(game->render, 255, 0, 0, 255);

    drawVector(game->render, game->gameObjects->universe->ship.angle, 30, game->gameObjects->universe->ship.rectShip);

    SDL_SetRenderDrawColor(game->render, 0, 255, 0, 255);

    drawVector(game->render, atan2(game->gameObjects->universe->ship.directionGravityVector.vector.y,
                                   game->gameObjects->universe->ship.directionGravityVector.vector.x),
               30, game->gameObjects->universe->ship.rectShip);

    // Update the screen

    SDL_RenderPresent(game->render);


}

/**
* This function launch the game, initialize necessary values for allow the game to run and start the loop.
* 
* @param game The game structure that contains all necessary elements for allow the game to run 
*/
void run(Game *game) {

    // Set the delta time to 60 FPS, so each update will be set each 16,7Ms
    game->clock.DELTA_TIME = 1000 / 60.0;

    // Get for how many time SDL start running for determine the time
    game->clock.startMillis = SDL_GetTicks();

    // Game loop
    while (game->isGameRunning) {
        // Get for how many times the game run
        game->clock.currentMillis = SDL_GetTicks();

        // Each 16,7ms (so to allow the game to run at 60FPS) we process to an update
        if (game->clock.currentMillis - game->clock.startMillis >= game->clock.DELTA_TIME) {

            update(game);


            // Render all elements
            render(game);

            // Reset startMillis to check the time
            game->clock.startMillis = SDL_GetTicks();

        }

        // This line allow to not use 100% of the core cause each ms the loop will be executed too many times and the game will be laggy
        SDL_Delay(1);

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

void drawVector(SDL_Renderer *renderer, float angle, float length, SDL_FRect ship) {
    // Calcule les coordonnées finales du vecteur en fonction de l'angle et de la longueur
    float x2 = ship.x + length * cos(angle);
    float y2 = ship.y + length * sin(angle);

    // Dessine le vecteur
    SDL_RenderDrawLine(renderer, (int)ship.x, (int)ship.y, (int)x2, (int)y2);
}

bool checkCollision(Ship *ship, Planet *planet) {
    // Calcul des bords du vaisseau (rectangle)
    float shipLeft = ship->rectShip.x;
    float shipRight = ship->rectShip.x + ship->rectShip.w;
    float shipTop = ship->rectShip.y;
    float shipBottom = ship->rectShip.y + ship->rectShip.h;

    // Bords du carré de la planète
    float planetLeft = planet->pos.x - planet->radius;
    float planetRight = planet->pos.x + planet->radius;
    float planetTop = planet->pos.y - planet->radius;
    float planetBottom = planet->pos.y + planet->radius;

    // Vérifie s'il y a une collision
    if (shipRight < planetLeft || shipLeft > planetRight || shipBottom < planetTop || shipTop > planetBottom) {
        return false; // Aucune collision
    }

    // Collision détectée
    return true;
}




