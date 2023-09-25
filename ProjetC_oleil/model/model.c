#include <stdio.h>
#include <stdlib.h>
#include "model.h"

Universe* initUniverse()
{
    printf("Game Initialisation\n");

    FILE* file = fopen("../config.txt", "r");
    if(file == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier config.txt\n");
        exit(1);
    }

    Universe* game = (Universe*)calloc(1, sizeof(Universe));

    //Take the window size from the config.txt file
    Coord WinSize;
    char line[14] = {0};
    fscanf(file, "%s %d %d", line, &WinSize.x, &WinSize.y);

    //Take the start position from the config.txt file
    fscanf(file, "%s %d %d", line, &game->start.x, &game->start.y);
    if(game->start.x < 0 || game->start.x > WinSize.x || game->start.y < 0 || game->start.y > WinSize.y)
    {
        printf("Error in the config.txt file, closing program.\n");
        free(game);
        return NULL;
    }

    game->ship.pos = game->start;
    game->ship.speed = 0;
    game->ship.angle = 0;
    game->ship.radius = 2;
    game->ship.vectors = NULL;

    fscanf(file, "%s %d %d", line, &game->end.x, &game->end.y);
    if(game->end.x < 0 || game->end.x > WinSize.x || game->end.y < 0 || game->end.y > WinSize.y)
    {
        printf("Error in the config.txt file.\n");
        free(game);
        return NULL;
    }

    int nbSolarSystem = 0;
    fscanf(file, "%s %d", line, &nbSolarSystem);
    if(nbSolarSystem < 0)
    {
        printf("Error in the config.txt file.\n");
        free(game);
        return NULL;
    }

    game->solarSystem = (SolarSystem*)calloc(nbSolarSystem, sizeof(SolarSystem));
    if(!game->solarSystem)
    {
        printf("Error while allocating memory for the solar system.\n");
        free(game);
        return NULL;
    }

    game->nbSolarSystem = nbSolarSystem;
    for(int i = 0; i < nbSolarSystem; i++)
    {
        fscanf(file, "%s %d %d", line, &game->solarSystem[i].star.pos.x, &game->solarSystem[i].star.pos.y);
        fscanf(file, "%s %d", line, &game->solarSystem[i].star.radius);

        int nbPlanet = 0;
        fscanf(file, "%s %d", line, &nbPlanet);
        if(nbPlanet < 0)
        {
            printf("Error in the config.txt file.\n");
            free(game);
            return NULL;
        }

        game->solarSystem[i].planets = (Planet*)calloc(nbPlanet, sizeof(Planet));
        if(!game->solarSystem[i].planets)
        {
            printf("Error while allocating memory for the planets.\n");
            free(game);
            return NULL;
        }

        game->solarSystem[i].nbPlanet = nbPlanet;
        for(int j = 0; j < nbPlanet; j++)
        {
            fscanf(file, "%s %d", line, &game->solarSystem[i].planets[j].radius);
            if(game->solarSystem[i].planets[j].radius < 0)
            {
                printf("Error in the config.txt file.\n");
                free(game);
                return NULL;
            }

            fscanf(file, "%s %d", line, &game->solarSystem[i].planets[j].orbit);
            game->solarSystem[i].planets[j].pos.x = game->solarSystem[i].star.pos.x;
            if(game->solarSystem[i].planets[j].orbit < 0)
                game->solarSystem[i].planets[j].pos.y = game->solarSystem[i].star.pos.y - game->solarSystem[i].planets[j].orbit;
            else 
                game->solarSystem[i].planets[j].pos.y = game->solarSystem[i].star.pos.y + game->solarSystem[i].planets[j].orbit;
        }
    }

    fclose(file);
    return game;
}

void destroyGame(Universe* game)
{
    for(int i=0; i < game->nbSolarSystem; i++)
    {
        free(game->solarSystem[i].planets);
    }
    if(game->ship.vectors)
        free(game->ship.vectors);
    free(game->solarSystem);
    free(game);
}

void universePrint(Universe* game)
{
    printf("\n\nGame :\n");
    for(int i = 0; i < game->nbSolarSystem; i++)
    {
        printf("\nSolar System %d :\n", i+1);
        printf("Star pos : %d %d\n", game->solarSystem[i].star.pos.x, game->solarSystem[i].star.pos.y);
        printf("Star radius : %d\n", game->solarSystem[i].star.radius);
        printf("Nb Planet : %d\n", game->solarSystem[i].nbPlanet);
        for(int j = 0; j < game->solarSystem[i].nbPlanet; j++)
        {
            printf("  Planet %d :\n", j+1);
            printf("  Planet radius : %d\n", game->solarSystem[i].planets[j].radius);
            printf("  Planet orbit : %d\n", game->solarSystem[i].planets[j].orbit);
            printf("  Planet pos : %d %d\n", game->solarSystem[i].planets[j].pos.x, game->solarSystem[i].planets[j].pos.y);
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