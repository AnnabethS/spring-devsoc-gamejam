#include "button.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

TTF_Font* font;

char isButtonHovered(button_t* b, int x, int y)
{
	SDL_Point p = {x,y};
	return SDL_PointInRect(&p, &b->rect);
}

void updateButton(button_t* b, int mouseX, int mouseY)
{
	b->hovered = isButtonHovered(b, mouseX, mouseY);
}

void initButton(button_t* b, SDL_Renderer* r, int x, int y, int w, int h, char* text)
{
	b->rect.x = x;
	b->rect.y = y;
	b->rect.w = w;
	b->rect.h = h;
	b->hovered = 0;
	SDL_Color fg1 = {255, 255, 255, 255};
	SDL_Color fg2 = {120, 120, 120, 255};
	SDL_Color bg = {40, 40, 40, 255};
	SDL_Surface* s = TTF_RenderText_Solid(font, text, fg1);
	b->hoveredTexture = SDL_CreateTextureFromSurface(r, s);
	SDL_FreeSurface(s);
	s = TTF_RenderText_Solid(font, text, fg2);
	b->notHoveredTexture = SDL_CreateTextureFromSurface(r, s);
	SDL_FreeSurface(s);
	SDL_QueryTexture(b->hoveredTexture, NULL, NULL, &b->textRect.w, &b->textRect.h);
	b->textRect.x = b->rect.x + (b->rect.w/2) - (b->textRect.w/2);
	b->textRect.y = b->rect.y + (b->rect.h/2) - (b->textRect.h/2);
}

void drawButton(SDL_Renderer* r, button_t *b)
{
	SDL_SetRenderDrawColor(r, 40, 40, 40, 255);
	SDL_RenderFillRect(r, &b->rect);
	if(b->hovered)
	{
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(r, 120, 120, 120, 255);
	}
	SDL_RenderDrawRect(r, &b->rect);
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	if(b->hovered)
		SDL_RenderCopy(r, b->hoveredTexture, NULL, &b->textRect);
	else
		SDL_RenderCopy(r, b->notHoveredTexture, NULL, &b->textRect);
}
