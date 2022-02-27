#ifndef BUTTON
#define BUTTON

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font* font;

typedef struct{
	SDL_Rect rect;
	char hovered;
	SDL_Texture* hoveredTexture;
	SDL_Texture* notHoveredTexture;
	SDL_Rect textRect;
}button_t;

void initButton(button_t* b, SDL_Renderer* r, int x, int y, int w, int h, char* text);
void updateButton(button_t* b, int mouseX, int mouseY);
void drawButton(SDL_Renderer* r, button_t *b);
char isButtonHovered(button_t* b, int x, int y);

#endif
