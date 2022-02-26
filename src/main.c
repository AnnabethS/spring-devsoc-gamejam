#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include "level.h"
#include "planet.h"
#include "sdl_util.h"
#include "textures.h"
#include "anna-layer.h"
#include "vector.h"
#include "player.h"

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900

#define TICK_INTERVAL 15

static u32 next_tick;

u32 timeLeft()
{
	u32 now = SDL_GetTicks();
	if(next_tick <= now)
		return 0;
	else
		return next_tick - now;
}

int main(){
	SDL_Window* window;
	SDL_Renderer* renderer;
	basicSetup(SCREENWIDTH, SCREENHEIGHT,
			   (SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_TIMER),
			   "Literally just that bit from cowboy bebop where spike is floating "
			   "around in space and he shoots his gun and flies back",
			   &window, &renderer);

	//SDL_Surface* spriteSheetSurface = IMG_Load("res/spritesheet.png");
	loadTextures(renderer);

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	player_t player;
	initPlayer(&player, 200, 200, 2);
	loadLevel(0);

	char running = 1;
	next_tick = SDL_GetTicks() + TICK_INTERVAL;

	vec2f mousePos = {0};

	//in degrees
	float mouseAngleToPlayer = 0;

	SDL_Rect gunRect = {0,0,textures.gunNeutralRect.w*2,textures.gunNeutralRect.h*2};
	SDL_Point gunCentre = {gunRect.w/2, gunRect.h/2};
	char gunFired = 0;
	const u8 gunFiredFrames = 10;
	u8 framesSinceGunFired = 0;

	while(running)
	{
		// get input events
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
				case SDL_SCANCODE_Q:
					running = 0;
					break;
				default:
					break;
				}
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
				case SDL_BUTTON_LEFT:
					if(currentLevel.currentBullets > 0)
					{
						gunFired = 1;
						vec2f force;
						vec2fUnitVectorFromAngleDegrees(&force, mouseAngleToPlayer);
						vec2fScalarProduct(&force, &force, -3);
						vec2fAdd(&player.vel, &player.vel, &force);
						currentLevel.currentBullets--;
						break;
					}
				}
			}
		}
		if(gunFired)
		{
			if(framesSinceGunFired > gunFiredFrames)
			{// stop the gun fired anim
				gunFired = 0;
				framesSinceGunFired = 0;
			}
			else
			{
				framesSinceGunFired++;
			}
		}
		int mouseX;
		int mouseY;

		SDL_GetMouseState(&mouseX, &mouseY);
		mousePos.x = (float) mouseX;
		mousePos.y = (float) mouseY;
		mouseAngleToPlayer = vec2fAngleDegrees(&player.pos, &mousePos);

		// start update

		updatePlayer(&player);
		vec2f gunPos = {0};
		vec2f gunOffset = {0};
		vec2fUnitVectorFromAngleDegrees(&gunOffset, mouseAngleToPlayer);
		vec2fScalarProduct(&gunOffset, &gunOffset, 20);
		vec2fAdd(&gunPos, &gunOffset, &player.pos);
		gunRect.x = gunPos.x - gunCentre.x;
		gunRect.y = gunPos.y - gunCentre.y;

		// end update

		// start render

		for(int x=0; x < SCREENWIDTH; x+=32)
		{
			for(int y=0; y < SCREENHEIGHT; y+=32)
			{
				SDL_Rect tmp = {x,y,32,32};
				SDL_RenderCopy(renderer, textures.sheet, &textures.bgRect, &tmp);
			}
		}

		planet_t* ptr = currentLevel.planetListHead;
		while (ptr != NULL)
		{
			drawPlanet(renderer, ptr);
			ptr = ptr->next;
		}
		drawPlayer(renderer, &player);

		SDL_RendererFlip gunflip = SDL_FLIP_NONE;
		if(mouseAngleToPlayer <= -90 || mouseAngleToPlayer >= 90)
			gunflip = SDL_FLIP_VERTICAL;

		if(gunFired)
			SDL_RenderCopyEx(renderer, textures.sheet, &textures.gunFiredRect,
			                 &gunRect, mouseAngleToPlayer, &gunCentre, gunflip);
		else
			SDL_RenderCopyEx(renderer, textures.sheet, &textures.gunNeutralRect,
			                 &gunRect, mouseAngleToPlayer, &gunCentre, gunflip);

		// begin UI rendering

		SDL_Rect bulletRect = {32,32,32,32};

		for(int i=0; i < currentLevel.currentBullets; i++)
		{
			SDL_RenderCopy(renderer, textures.sheet, &textures.bulletRect, &bulletRect);
			bulletRect.x += 48;
		}

		// end render
		SDL_RenderPresent(renderer);
		SDL_Delay(timeLeft());
		next_tick += TICK_INTERVAL;
	}
}
