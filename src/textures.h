#ifndef TEXTURES
#define TEXTURES

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

typedef struct{
	SDL_Texture* sheet;
	SDL_Rect playerRect;
	SDL_Rect planetRect;
	SDL_Rect gunNeutralRect;
	SDL_Rect gunFiredRect;
	SDL_Rect bgRect;
	SDL_Rect bulletRect;
}ts;

extern ts textures;

char loadTextures(SDL_Renderer* r);

#endif
