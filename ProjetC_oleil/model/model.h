#pragma once

#include <SDL2/SDL.h>
#include <math.h>

/// @brief Structure to store a coordinate
/// @param x The x coordinate
/// @param y The y coordinate
typedef struct Coord_s {
    float x;
    float y;
} Coord;

/// @brief Structure to store a vector
/// @param force The force of the vector
/// @param angle The angle of the vector
typedef struct Vector_s {
    int force;
    int angle;
    Coord vector;
} Vector;

/// @brief Structure to store a planet
/// @param pos The position of the planet
/// @param radius The radius of the planet
/// @param orbit The orbit of the planet
typedef struct Planet_s {
    Coord pos;
    int radius;
    int orbit;
    float angle;
    float distance_to_star;
} Planet;

/// @brief Structure to store a star
/// @param pos The position of the star
/// @param radius The radius of the star
typedef struct Star_s {
    Coord pos;
    int radius;
} Star;

/// @brief Structure to store a solar system
/// @param star The star of the solar system
/// @param nbPlanet The number of planet in the solar system
/// @param planets The planets of the solar system
typedef struct SolarSystem_s {
    Star star;
    int nbPlanet;
    Planet *planets;
} SolarSystem;

/// @brief Structure to store a ship
/// @param pos The position of the ship
/// @param radius The radius of the ship
/// @param speed The speed of the ship
/// @param angle The angle of the ship
/// @param vectors The vectors of the ship
typedef struct Ship_s {
    Coord pos;
    int radius;
    int speed;
    float angle;
    Vector directionGravityVector;
    Vector directionVector;
    Vector *vectors;
    SDL_FRect rectShip;

} Ship;

/// @brief Structure to store a game
/// @param solarSystem The solar system of the game
/// @param nbSolarSystem The number of solar system in the game
/// @param ship The ship of the game
/// @param start The start position of the ship
/// @param end The position of the end of game
typedef struct Universe_s {
    SolarSystem *solarSystem;
    int nbSolarSystem;
    Ship ship;
    Coord start;
    SDL_Rect rectStart;
    Coord end;
    SDL_Rect rectEnd;
    Coord WINSIZE;
} Universe;

/// @brief Function to destroy a game
/// @param game The game to destroy
void destroyGame(Universe *game);

/// @brief Function to print a game in the terminal
/// @param game The game to print
void universePrint(Universe *game);

/// @brief Function to initialize a game
/// @return The game initialized
Universe *initUniverse();

/// @brief Function that calculate next coordinate for planet rotation
/// @param objectCoordToRotate The planet to rotate
/// @param objectToRotateAround The star around which the planet rotate
/// @param angle The angle of rotation
void rotateObjectArroundAnother(Planet *objectCoordToRotate, Star *objectToRotateAround, float *angle);

/// @brief Function that calculate next coordinate for ship direction vector
/// @param objectCoordToRotate The vector point that rotate
/// @param objectToRotateAround The ship around which the vector rotate
/// @param angle The angle of rotation
void rotatePoint(Coord *objectCoordToRotate, SDL_FRect *objectToRotateAround, float angle);

/// @brief Function that calculate an angle in radian from degrees
/// @param degrees The angle in degrees
float getAngleInRadian(int degrees);

/// @brief
/// @param
float gravityStar(Ship ship, Star star);

/// @brief
/// @param
float gravityPlanet(Ship ship, Planet planet);

/// @brief Function to add vectors from ship and a Planet 1 and a Planet 2
/// @param ship The ship
/// @param planet1 The first planet
/// @param planet2 The second planet
/// This function try to return a vector that represent the sum of the vectors from the ship and the
/// two planets and the vector will be nearest the planet with the greatest force
Vector additionVectorWithGravityAndAngle(Ship ship, Planet planet);
/// @brief Function to add a additionate a list of vector to have one final vector.
/// @param vectors The list of vectors
/// @param size The size of the list
Vector vectorSum(Vector vectors[], int size);
