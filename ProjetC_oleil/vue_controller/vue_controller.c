#include "vue_controller.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

#define VECTOR_LENGTH 30
#define FRAMERATE 60.0
#define SHIP_SPEED 8
#define GRAVITY_COEFFICIENT 0.07
#define SHIP_VECTOR_DIRECTION_COEFFICIENT 0.1

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

Game *init() {
    Game *game = malloc(sizeof(Game));

    // Initialize the event structure
    game->event = malloc(sizeof(SDL_Event));
    // To allow the loop to run we initialize it to true
    game->isGameRunning = true;

    // Set the score at 0
    // To not have a nullptr when we will want to use the gameObjects, we allocate memory for it
    game->gameObjects = malloc(sizeof(GameObjects));
    game->gameObjects->gameTitleBuffer = malloc(1024);

    game->color = malloc(sizeof(Color));
    initColor(game->color);
    game->gameObjects = (GameObjects *) calloc(2, sizeof(GameObjects));
    game->gameObjects->gameTitleBuffer = malloc(1024);
    game->gameObjects->universe = initUniverse();
    game->gameObjects->score = 0;


    // Get the windows size and set into the game structure.
    game->WINDOW_LENGHT = game->gameObjects->universe->WINSIZE.x;
    game->WINDOW_HEIGHT = game->gameObjects->universe->WINSIZE.y;

    // Create the window with the specified size, if we add "SDL_WINDOW_RESIZABLE" flags, it will allow to resize the window during the game
    game->window = SDL_CreateWindow("ProjetC_oleil", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    game->WINDOW_LENGHT, game->WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    // The renderer struct
    game->render = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);


    return game;
}

void update(Game *game) {
    // This vector will be the sum of all gravity vector from all planets we will apply to the ship
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
                        if (game->gameObjects->universe->ship.angle < 0) {
                            game->gameObjects->universe->ship.angle = 2 * M_PI;
                        }
                        break;
                        // Rotate the ship to the right
                    case SDLK_RIGHT:
                        game->gameObjects->universe->ship.angle += .2;
                        if (game->gameObjects->universe->ship.angle > 2 * M_PI) {
                            game->gameObjects->universe->ship.angle = 0;
                        }
                        break;
                }

        }
    }

    game->gameObjects->score++;
    ///////////////////////
    // Change the title of the window with FPS and Score
    if (game->clock.currentMillis - game->clock.startMillis > 0) {

        // Replace "%f" with the FPS, the function change the value of the char gameTitleBuffer
        snprintf(game->gameObjects->gameTitleBuffer,
                 256,
                 "ProjetC_oleil | FPS : %.02f | Score : %d",
                 (1000.0 / (game->clock.currentMillis - game->clock.startMillis)),
                    game->gameObjects->score
        );
    }

    ///////////////////////
    // Part that handle all the interactions between the ship and the planets
    for (int i = 0; i < game->gameObjects->universe->nbSolarSystem; i++) {
        for (int j = 0; j < game->gameObjects->universe->solarSystem[i].nbPlanet; j++) {
            // Rotate the planet around the star
            rotateObjectArroundAnother(&game->gameObjects->universe->solarSystem[i].planets[j],
                                       &game->gameObjects->universe->solarSystem[i].star,
                                       &game->gameObjects->universe->solarSystem[i].planets[j].angle);

            // Check if the ship is in collision with the planet
            if (checkCollision(&game->gameObjects->universe->ship,
                               &game->gameObjects->universe->solarSystem[i].planets[j])) {
                // If the ship is in collision with the planet, we stop the game
                game->isGameRunning = false;
                return;
            }
            // If the planet is the last of the solar system, we rotate the ship around the planet
            // The j+1 is to avoid to check an undefined index
            if (j + 1 <= game->gameObjects->universe->solarSystem[i].nbPlanet) {
                Vector gravityShipVector = additionVectorWithGravityAndAngle(game->gameObjects->universe->ship,
                                                                             game->gameObjects->universe->solarSystem[i].planets[j]);
                totalGravityShipVector.vector.x += gravityShipVector.vector.x;
                totalGravityShipVector.vector.y += gravityShipVector.vector.y;

            }

        }
    }

    // This part handle and apply the movement of the ship

    // First we apply the gravity vector to the ship, the coefficient is here to have coherent value
    // And not have the ship move too fast
    game->gameObjects->universe->ship.rectShip.x += totalGravityShipVector.vector.x * GRAVITY_COEFFICIENT;
    game->gameObjects->universe->ship.rectShip.y += totalGravityShipVector.vector.y * GRAVITY_COEFFICIENT;

    // We store it to draw the vector in the render part
    game->gameObjects->universe->ship.directionGravityVector = totalGravityShipVector;

    // Second we get the angle to the ship depending of the angle the ship have (we control it with the
    // arrow of the keyboard)
    game->gameObjects->universe->ship.directionVector.vector.x = cos(game->gameObjects->universe->ship.angle);
    game->gameObjects->universe->ship.directionVector.vector.y = sin(game->gameObjects->universe->ship.angle);

    // Finally we apply the direction vector to the ship to move it, we multiply it by a speed (here it's 8) and a coefficient
    // The coefficient allow to have coherent values of move, if we not do that the ship will move too fast
    game->gameObjects->universe->ship.rectShip.x +=
            game->gameObjects->universe->ship.directionVector.vector.x * SHIP_SPEED * SHIP_VECTOR_DIRECTION_COEFFICIENT;
    game->gameObjects->universe->ship.rectShip.y +=
            game->gameObjects->universe->ship.directionVector.vector.y * SHIP_SPEED * SHIP_VECTOR_DIRECTION_COEFFICIENT;

    ////////////////////
    // This part handle the max X and max Y of the ship and teleporte it into the opposite side of the window
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

            // This part create new circle arround the star depending of the distance of the planet to the star, it's the
            // orbit of the actual planet
            actualPlanetLine = game->gameObjects->universe->solarSystem[i].planets[j];
            actualPlanetLine.pos.x = game->gameObjects->universe->solarSystem[i].star.pos.x;
            actualPlanetLine.pos.y = game->gameObjects->universe->solarSystem[i].star.pos.y;
            actualPlanetLine.radius = game->gameObjects->universe->solarSystem[i].planets[j].distance_to_star;

            // Draw the orbit of the planet
            drawPlanet(game->render, actualPlanetLine, 0, game->color->HALF_WHITE);
        }
    }

    SDL_SetRenderDrawColor(game->render, 255, 0, 0, 255);

    // Draw the direction vector of the ship
    drawVector(game->render, game->gameObjects->universe->ship.angle, game->gameObjects->universe->ship.rectShip);

    SDL_SetRenderDrawColor(game->render, 0, 255, 0, 255);

    // Draw the gravity vector of the ship
    drawVector(game->render, atan2(game->gameObjects->universe->ship.directionGravityVector.vector.y,
                                   game->gameObjects->universe->ship.directionGravityVector.vector.x),
               game->gameObjects->universe->ship.rectShip);

    // Update the screen

    SDL_RenderPresent(game->render);


}

void run(Game *game) {

    // Set the delta time to 60 FPS, so each update will be set each 16,7Ms
    game->clock.DELTA_TIME = 1000 / FRAMERATE;

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

void drawVector(SDL_Renderer *renderer, float angle, SDL_FRect ship) {
    // Draw a line from vector
    // We get the position of ship, we add the lenght and from the angle of the ship we can draw a line
    float x2 = ship.x + VECTOR_LENGTH * cos(angle);
    float y2 = ship.y + VECTOR_LENGTH * sin(angle);

    // Draw an SDL line to show the vector
    SDL_RenderDrawLine(renderer, (int) ship.x, (int) ship.y, (int) x2, (int) y2);
}

bool checkCollision(Ship *ship, Planet *planet) {
    // Get rectangle point of the ship
    float shipPointLeft = ship->rectShip.x;
    float shipPointRight = ship->rectShip.x + ship->rectShip.w;
    float shipPointTop = ship->rectShip.y;
    float shipPointBottom = ship->rectShip.y + ship->rectShip.h;

    // Get rectangle point of the planet
    float planetPointLeft = planet->pos.x - planet->radius;
    float planetPointRight = planet->pos.x + planet->radius;
    float planetPointTop = planet->pos.y - planet->radius;
    float planetPointBottom = planet->pos.y + planet->radius;

    // If the ship is in the planet rectangle return true else return false
    return !(shipPointRight < planetPointLeft || shipPointLeft > planetPointRight || shipPointBottom < planetPointTop ||
             shipPointTop > planetPointBottom);
}




