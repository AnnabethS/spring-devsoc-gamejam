#include "planet.h"
#include "textures.h"
#include <SDL2/SDL_image.h>

planet_t* initPlanet(planet_t** p, int x, int y, float mass)
{
	*p = malloc(sizeof(planet_t));
	(*p)->pos.x = (float) x;
	(*p)->pos.y = (float) y;
	(*p)->mass = mass;
	float textureScale = mass;
	(*p)->rect.w = 16*textureScale;
	(*p)->rect.h = 16*textureScale;
	(*p)->rect.x = x - ((*p)->rect.w/2);
	(*p)->rect.y = y - ((*p)->rect.h/2);
	(*p)->radius = (*p)->rect.w / (float)2;
	(*p)->next = NULL;
	return *p;
}

void drawPlanet(SDL_Renderer* r, planet_t* p)
{
	SDL_RenderCopy(r, textures.sheet, &textures.planetRect, &p->rect);
}
