#include "textures.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include "anna-layer.h"

ts textures;

local void setRect(SDL_Rect* r, int x, int y, int w, int h);

char loadTextures(SDL_Renderer *r)
{
	SDL_Surface* s = IMG_Load("res/spritesheet.png");
	if(s == NULL)
	{
		printf("failed loading image into surface: %s\n", SDL_GetError());
		return 1;
	}
	textures.sheet = SDL_CreateTextureFromSurface(r, s);
	SDL_FreeSurface(s);
	if(textures.sheet == NULL)
	{
		printf("failed loading texture\n");
		return 1;
	}

	setRect(&textures.playerRect, 0, 0, 16, 16);
	setRect(&textures.planetRect, 16, 0, 16, 16);
	setRect(&textures.gunNeutralRect, 32, 0, 16, 16);
	setRect(&textures.gunFiredRect, 48, 0, 16, 16);
	setRect(&textures.bgRect, 64, 0, 16, 16);
	setRect(&textures.bulletRect, 80, 0, 16, 16);
	setRect(&textures.dogRect, 96, 0, 16, 16);
	setRect(&textures.flagRect, 208, 0, 16, 16);
	setRect(&textures.offScreenIndicator, 176, 0, 16, 16);
	setRect(&textures.normalSpeedIndicator, 128, 0, 16, 16);
	setRect(&textures.slowSpeedIndicator, 160, 0, 16, 16);
	setRect(&textures.circleIndicator, 224, 0, 16, 16);
	return 0;
}


local void setRect(SDL_Rect* r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}
