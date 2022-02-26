#ifndef PLANET
#define PLANET
#include "vector.h"
#include <SDL2/SDL.h>

typedef struct planet_t{
	vec2f pos; // centre of the planet
	float mass; // mass relative to the player
	SDL_Rect rect; // rect, x and y need calculating from pos on init
	float radius; // radius to use for collisions
	struct planet_t* next; // pointer to the next planet or NULL
}planet_t;

planet_t* initPlanet(planet_t **p, int x, int y, float mass);
void drawPlanet(SDL_Renderer* r, planet_t* p);

#endif
