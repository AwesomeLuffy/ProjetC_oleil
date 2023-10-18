#include <stdio.h>
#include <stdlib.h>
#include "model.h"

// This coefficient is to reduce the orbit speed of the planet cause if we set 130 for example it's too big
#define COEFFICIENT_PLANET_SPEED_SLOWDOWN 60
#define GRAVITY_CONSTANT 1000
#define MASS_COEFFICIENT 200

Universe *initUniverse() {

    FILE *file = fopen("../config.txt", "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier config.txt\n");
        exit(1);
    }

    Universe *game = (Universe *) calloc(1, sizeof(Universe));


    //Take the window size from the config.txt file
    Coord WinSize;
    char line[16] = {0};
    fscanf(file, "%s %f %f", line, &WinSize.x, &WinSize.y);
    game->WINSIZE = WinSize;
    //Take the start position from the config.txt file
    fscanf(file, "%s %f %f", line, &game->start.x, &game->start.y);
    if (game->start.x < 0 || game->start.x > WinSize.x || game->start.y < 0 || game->start.y > WinSize.y) {
        printf("Error in the config.txt file, closing program.\n");
        free(game);
        return NULL;
    }
    game->rectStart.x = game->start.x;
    game->rectStart.y = game->start.y;
    game->rectStart.w = 10;
    game->rectStart.h = 10;

    game->ship.pos = game->start;
    game->ship.speed = 0;
    game->ship.angle = 0;
    game->ship.radius = 2;
    game->ship.vectors = NULL;
    game->ship.rectShip.x = game->ship.pos.x;
    game->ship.rectShip.y = game->ship.pos.y;
    game->ship.rectShip.w = 10;
    game->ship.rectShip.h = 10;

    fscanf(file, "%s %f %f", line, &game->end.x, &game->end.y);
    if (game->end.x < 0 || game->end.x > WinSize.x || game->end.y < 0 || game->end.y > WinSize.y) {
        printf("Error in the config.txt file.\n");
        free(game);
        return NULL;
    }
    game->rectEnd.x = game->end.x;
    game->rectEnd.y = game->end.y;
    game->rectEnd.w = 10;
    game->rectEnd.h = 10;


    int nbSolarSystem = 0;
    fscanf(file, "%s %d", line, &nbSolarSystem);

    if (nbSolarSystem < 0) {
        printf("Error in the config.txt file.\n");
        free(game);
        return NULL;
    }

    game->solarSystem = (SolarSystem *) calloc(nbSolarSystem, sizeof(SolarSystem));
    if (!game->solarSystem) {
        printf("Error while allocating memory for the solar system.\n");
        free(game);
        return NULL;
    }

    game->nbSolarSystem = nbSolarSystem;
    for (int i = 0; i < nbSolarSystem; i++) {
        fscanf(file, "%s %f %f", line, &game->solarSystem[i].star.pos.x, &game->solarSystem[i].star.pos.y);
        fscanf(file, "%s %d", line, &game->solarSystem[i].star.radius);

        int nbPlanet = 0;
        fscanf(file, "%s %d", line, &nbPlanet);
        if (nbPlanet < 0) {
            printf("Error in the config.txt file.\n");
            free(game);
            return NULL;
        }

        game->solarSystem[i].planets = (Planet *) calloc(nbPlanet, sizeof(Planet));
        if (!game->solarSystem[i].planets) {
            printf("Error while allocating memory for the planets.\n");
            free(game);
            return NULL;
        }
        game->solarSystem[i].nbPlanet = nbPlanet;
        for (int j = 0; j < nbPlanet; j++) {
            fscanf(file, "%s %d", line, &game->solarSystem[i].planets[j].radius);
            if (game->solarSystem[i].planets[j].radius < 0) {
                printf("Error in the config.txt file.\n");
                free(game);
                return NULL;
            }

            fscanf(file, "%s %d", line, &game->solarSystem[i].planets[j].orbit);

            game->solarSystem[i].planets[j].angle = getAngleInRadian(game->solarSystem[i].planets[j].radius);


            game->solarSystem[i].planets[j].pos.x = game->solarSystem[i].star.pos.x;
            if (game->solarSystem[i].planets[j].orbit < 0)
                game->solarSystem[i].planets[j].pos.y =
                        game->solarSystem[i].star.pos.y - game->solarSystem[i].planets[j].orbit;
            else
                game->solarSystem[i].planets[j].pos.y =
                        game->solarSystem[i].star.pos.y + game->solarSystem[i].planets[j].orbit;
            game->solarSystem[i].planets[j].distance_to_star =
                    game->solarSystem[i].planets[j].pos.y - game->solarSystem[i].star.pos.y;

        }


    }

    fclose(file);

    return game;
}

void destroyGame(Universe *game) {
    for (int i = 0; i < game->nbSolarSystem; i++) {
        free(game->solarSystem[i].planets);
    }
    if (game->ship.vectors)
        free(game->ship.vectors);
    free(game->solarSystem);
    free(game);
}

void universePrint(Universe *game) {
    if (!game) {
        printf("Error while printing the game, the game is NULL.\n");
        return;
    }
    printf("\n\nGame :\n");

    for (int i = 0; i < game->nbSolarSystem; i++) {
        printf("\nSolar System %d :\n", i + 1);
        printf("Star pos : %f %f\n", game->solarSystem[i].star.pos.x, game->solarSystem[i].star.pos.y);
        printf("Star radius : %d\n", game->solarSystem[i].star.radius);
        printf("Nb Planet : %d\n", game->solarSystem[i].nbPlanet);
        for (int j = 0; j < game->solarSystem[i].nbPlanet; j++) {
            printf("  Planet %d :\n", j + 1);
            printf("  Planet radius : %d\n", game->solarSystem[i].planets[j].radius);
            printf("  Planet orbit : %d\n", game->solarSystem[i].planets[j].orbit);
            printf("  Planet pos : %f %f\n", game->solarSystem[i].planets[j].pos.x,
                   game->solarSystem[i].planets[j].pos.y);
        }
    }
}

// int main()
// {
//     Universe* game = initUniverse();
//     if(!game) { printf("Error while during the config file extraction, closing program.\n"); return 0; }
//     printf("Game created\n");
//     universePrint(game);
//     destroyGame(game);
//     printf("Done\n");
//     return 0;
// }

void rotateObjectArroundAnother(Planet *objectCoordToRotate, Star *objectToRotateAround, float *angle) {
    // To not recreate the variable each time
    static double distanceX;
    static double distanceY;

    // Get the distance between the planet and the star
    distanceX = objectCoordToRotate->pos.x - objectToRotateAround->pos.x;
    distanceY = objectCoordToRotate->pos.y - objectToRotateAround->pos.y;

    // Rotate the planet arround the star
    objectCoordToRotate->pos.x = distanceX * cos(*angle) - distanceY * sin(*angle) + objectToRotateAround->pos.x;
    objectCoordToRotate->pos.y = distanceX * sin(*angle) + distanceY * cos(*angle) + objectToRotateAround->pos.y;
}

void rotatePoint(Coord* objectCoordToRotate, SDL_FRect *objectToRotateAround, float angle) {
    // To not recreate the variable each time
    static double distanceX;
    static double distanceY;

    // Get the distance between the planet and the star
    distanceX = objectCoordToRotate->x - objectToRotateAround->x;
    distanceY = objectCoordToRotate->y - objectToRotateAround->y;

    // Rotate the planet arround the star
    objectCoordToRotate->x = distanceX * cos(angle) - distanceY * sin(angle) + objectToRotateAround->x;
    objectCoordToRotate->y = distanceX * sin(angle) + distanceY * cos(angle) + objectToRotateAround->y;
}

float getAngleInRadian(int radius) {
    return 4 * M_PI / (60 * radius);
}

//float gravityStar(Ship ship, Star star) {
//    return 2000.0 * (star.radius / (pow(ship.pos.x - star.pos.x, 2) + pow(ship.pos.y - star.pos.y, 2)));
//}
//
//float gravityPlanet(Ship ship, Planet planet) {
//    return 2000.0 * (planet.radius / (pow(ship.pos.x - planet.pos.x, 2) + pow(ship.pos.y - planet.pos.y, 2)));
//}

Vector additionVectorWithGravityAndAngle(Ship ship, Planet planet) {
    static Vector resultVector;
    // Caculate distance between the ship and the planet
    float distancePlanetShipX = planet.pos.x - ship.pos.x;
    float distancePlanetShipY = planet.pos.y - ship.pos.y;
    float distance = sqrt(pow(distancePlanetShipX, 2) + pow(distancePlanetShipY, 2));

    // Set the planet mass as the planet radius
    float planetMass = planet.radius * MASS_COEFFICIENT;

    // Calculate the graivity force
    float gravityForce = GRAVITY_CONSTANT * planetMass / pow(distance, 2);

    // Get the angle between the ship and the planet
    float angle = atan2(distancePlanetShipX, distancePlanetShipY);

    // Apply gravity force to the ship
    float forceX = gravityForce * cos(angle);
    float forceY = gravityForce * sin(angle);


    resultVector.force = gravityForce;
    resultVector.angle = angle;
    resultVector.vector.x = forceX;
    resultVector.vector.y = forceY;

    return resultVector;
}
