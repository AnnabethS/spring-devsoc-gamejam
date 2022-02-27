#ifndef BULLET
#define BULLET

#include "vector.h"
#include <SDL2/SDL.h>

typedef struct bullet_s{
	vec2f initPos;
	vec2f pos;
	vec2f vel;
	struct bullet_s* next;
}bullet_t;

void initBullet(bullet_t** b, float x, float y, float angle);

char updateBullet(bullet_t* b);

void drawBullet(SDL_Renderer* r, bullet_t* b);

#endif
