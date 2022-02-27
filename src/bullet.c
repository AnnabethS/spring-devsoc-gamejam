#include "bullet.h"
#include "level.h"
#include "planet.h"
#include "vector.h"
#include "physics.h"
#include <SDL2/SDL_render.h>

#define BULLET_SPEED 10
#define TAIL_LENGTH 50

void initBullet(bullet_t** b, float x, float y, float angle)
{
	*b = malloc(sizeof(bullet_t));
	(*b)->initPos.x = x;
	(*b)->initPos.y = y;
	(*b)->pos.x = x;
	(*b)->pos.y = y;
	vec2fUnitVectorFromAngleDegrees(&(*b)->vel, angle);
	vec2fScalarProduct(&(*b)->vel, &(*b)->vel, BULLET_SPEED);
	(*b)->next = NULL;
}

// return 0 on no hit
// return 1 on hit planet
// return 2 on hit dog
char updateBullet(bullet_t* b)
{
	vec2fAdd(&b->pos, &b->pos, &b->vel);
	if(b->pos.x < -50 || b->pos.x > 1970 || b->pos.y < -50 || b->pos.y > 1130)
		return 1;
	
	planet_t* ptr = currentLevel.planetListHead;
	while (ptr != NULL)
	{
		if(pointInCircle(&ptr->pos, ptr->radius, &b->pos))
			return 1;
		ptr = ptr->next;
	}

	if(!currentLevel.playerHoldingDog &&
	   pointInCircle(&currentLevel.dogLocation, currentLevel.dogRadius, &b->pos))
		return 2;

	return 0;
}

void drawBullet(SDL_Renderer* r, bullet_t* b)
{
	vec2f toInit;
	vec2fDiff(&toInit, &b->pos, &b->initPos);
	if(vec2fMagnitude(&toInit) <= TAIL_LENGTH)
	{
		SDL_RenderDrawLine(r, b->initPos.x, b->initPos.y, b->pos.x, b->pos.y);
	}
	else
	{
		vec2fUnitVectorFromVector(&toInit, &toInit);
		vec2fScalarProduct(&toInit, &toInit, TAIL_LENGTH);
		vec2fAdd(&toInit, &toInit, &b->pos);
		SDL_RenderDrawLine(r, toInit.x, toInit.y, b->pos.x, b->pos.y);
	}
}
