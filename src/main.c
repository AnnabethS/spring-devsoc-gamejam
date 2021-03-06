#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include "bullet.h"
#include "button.h"
#include "level.h"
#include "planet.h"
#include "sdl_util.h"
#include "textures.h"
#include "anna-layer.h"
#include "vector.h"
#include "player.h"
#include "screens.h"

#define SCREENWIDTH 1920
#define SCREENHEIGHT 1080

#define TITLETEXT "Literally just that bit from cowboy bebop where spike is floating around in space and he fires his gun and flies backwards towards the ship"

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

int main(int argc, char** argv){
	SDL_Window* window;
	SDL_Renderer* renderer;
	basicSetup(SCREENWIDTH, SCREENHEIGHT,
			   (SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER),
			   TITLETEXT, &window, &renderer);
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	TTF_Init();
    IMG_Init(IMG_INIT_PNG);
	loadTextures(renderer);

	font = TTF_OpenFont("res/Monoid-Regular.ttf", 20);

	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

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

	int levelAmount = 5;
	button_t menuButtons[levelAmount];

	int menuButtonOffset = (levelAmount*150)/2;

	for(int i=0; i < levelAmount; i++)
	{
		char text[10] = {0};
		sprintf(text, "Level %d", (i+1));
		initButton(&menuButtons[i], renderer, (SCREENWIDTH/2)-menuButtonOffset+(i*150), (SCREENHEIGHT/2)-(25), 100, 50, text);
	}


	button_t winButton;
	initButton(&winButton, renderer, SCREENWIDTH/2 - 150, SCREENHEIGHT/2 -50, 300, 100, "You win, press ENTER");

	button_t lossButton;
	initButton(&lossButton, renderer, SCREENWIDTH/2 - 300, SCREENHEIGHT/2 - 50, 600, 100, "Careful where you shoot! (R to restart)");

	SDL_Color titleColor = {255,255,255,255};
	SDL_Surface* titleSurface = TTF_RenderText_Solid(font, TITLETEXT, titleColor);
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
	SDL_FreeSurface(titleSurface);
	SDL_Rect titleRect;
	SDL_QueryTexture(titleTexture, NULL, NULL, &titleRect.w, &titleRect.h);
	titleRect.x = SCREENWIDTH/2 - (titleRect.w/2);
	titleRect.y = 50;

	while(running)
	{
		int mouseX;
		int mouseY;

		SDL_GetMouseState(&mouseX, &mouseY);
		mousePos.x = (float) mouseX;
		mousePos.y = (float) mouseY;
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
					if(screen == SCREEN_GAME)
					{
						unloadLevel();
						screen = SCREEN_MENU;
					}
					else
					{
						running = 0;
					}
					break;
				case SDL_SCANCODE_R:
					unloadLevel();
					loadLevel(currentLevel.levelNum);
					break;
				case SDL_SCANCODE_Q:
					running = 0;
					break;
				case SDL_SCANCODE_RETURN:
					if(currentLevel.levelWon)
					{
						unloadLevel();
						screen = SCREEN_MENU;
					}
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(event.button.button)
				{
				case SDL_BUTTON_LEFT:
					switch(screen)
					{
					case SCREEN_GAME:
						if(!currentLevel.realPause && currentLevel.currentBullets > 0)
						{
							gunFired = 1;
							vec2f force;
							vec2fUnitVectorFromAngleDegrees(&force, mouseAngleToPlayer);
							vec2fScalarProduct(&force, &force, -3);
							vec2fAdd(&currentLevel.player.trueVel, &currentLevel.player.trueVel, &force);
							currentLevel.currentBullets--;
							bullet_t* bptr = currentLevel.bulletListHead;
							initBullet(&currentLevel.bulletListHead, currentLevel.player.pos.x, currentLevel.player.pos.y, mouseAngleToPlayer);
							currentLevel.bulletListHead->next = bptr;
							break;
						}
						break;
					case SCREEN_MENU:
						for(int i=0; i < levelAmount; i++)
						{
							if(menuButtons[i].hovered)
							{
								loadLevel(i);
								screen = SCREEN_GAME;
								break;
							}
						}
						break;
					}
					break;
				case SDL_BUTTON_RIGHT:
					if(screen == SCREEN_GAME)
					{
						currentLevel.gameSpeed = 0.2;
						currentLevel.gameSlowed = 1;
					}
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if(screen == SCREEN_GAME &&
				   event.button.button == SDL_BUTTON_RIGHT)
				{
					currentLevel.gameSpeed = 1;
					currentLevel.gameSlowed = 0;
				}
			}
		}
		switch(screen)
		{
		case SCREEN_GAME:
		{
			if(!currentLevel.realPause)
			{
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
				mouseAngleToPlayer = vec2fAngleDegrees(&currentLevel.player.pos, &mousePos);

				// start update

				updatePlayer(&currentLevel.player);

				vec2f gunPos = {0};
				vec2f gunOffset = {0};
				vec2fUnitVectorFromAngleDegrees(&gunOffset, mouseAngleToPlayer);
				vec2fScalarProduct(&gunOffset, &gunOffset, 20);
				vec2fAdd(&gunPos, &gunOffset, &currentLevel.player.pos);
				gunRect.x = gunPos.x - gunCentre.x;
				gunRect.y = gunPos.y - gunCentre.y;


				if(currentLevel.playerHoldingDog)
				{
					currentLevel.dogRect.w = currentLevel.dogRect.h = 16;
					currentLevel.dogRect.x = currentLevel.player.pos.x - 8;
					currentLevel.dogRect.y = currentLevel.player.pos.y - 8;
				}

				bullet_t* bptr_prev = NULL;
				bullet_t* bptr = currentLevel.bulletListHead;
				while(bptr != NULL)
				{
					char result = updateBullet(bptr);
					/* printf("result: %d\n", result); */
					if(result == 1)
					{ // planet hit
						if(bptr_prev == NULL)
						{
							bullet_t* bnext = bptr->next;
							free(bptr);
							currentLevel.bulletListHead = bnext;
							bptr = bnext;
							continue;
						}
						else
						{
							bullet_t* bnext = bptr->next;
							free(bptr);
							bptr_prev->next = bnext;
							bptr = bnext;
							continue;
						}
					}
					else if (result == 2)
					{ // dog hit :(
						currentLevel.realPause = 1;
						currentLevel.levelLost = 1;
					}
					bptr_prev = bptr;
					bptr = bptr->next;
				}

				// end update
			}
				// start render
			SDL_RenderClear(renderer);

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
			drawPlayer(renderer, &currentLevel.player);

			SDL_RendererFlip gunflip = SDL_FLIP_NONE;
			if(mouseAngleToPlayer <= -90 || mouseAngleToPlayer >= 90)
				gunflip = SDL_FLIP_VERTICAL;

			if(gunFired)
				SDL_RenderCopyEx(renderer, textures.sheet, &textures.gunFiredRect,
								&gunRect, mouseAngleToPlayer, &gunCentre, gunflip);
			else
				SDL_RenderCopyEx(renderer, textures.sheet, &textures.gunNeutralRect,
								&gunRect, mouseAngleToPlayer, &gunCentre, gunflip);


			SDL_RenderCopy(renderer, textures.sheet, &textures.dogRect, &currentLevel.dogRect);
			SDL_RenderCopy(renderer, textures.sheet, &textures.flagRect, &currentLevel.flagRect);

			bullet_t* bptr = currentLevel.bulletListHead;
			while(bptr != NULL)
			{
				drawBullet(renderer, bptr);
				bptr = bptr->next;
			}

			// begin UI rendering

			SDL_Rect bulletRect = {32,32,32,32};

			for(int i=0; i < currentLevel.currentBullets; i++)
			{
				SDL_RenderCopy(renderer, textures.sheet, &textures.bulletRect, &bulletRect);
				bulletRect.x += 40;
			}

			SDL_Rect speedRect = {SCREENWIDTH-138, 10, 128, 128};
			if(currentLevel.gameSlowed)
				SDL_RenderCopy(renderer, textures.sheet, &textures.slowSpeedIndicator, &speedRect);
			else
				SDL_RenderCopy(renderer, textures.sheet, &textures.normalSpeedIndicator, &speedRect);

			if(currentLevel.levelWon)
				drawButton(renderer, &winButton);
			if(currentLevel.levelLost)
				drawButton(renderer, &lossButton);
			break;
		}
		case SCREEN_MENU:
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			for(int i=0; i < levelAmount; i++)
			{
				updateButton(&menuButtons[i], mousePos.x, mousePos.y);
			}
			for(int i=0; i < levelAmount; i++)
			{
				drawButton(renderer, &menuButtons[i]);
			}
			SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
			break;
		}
		}

		// end render
		SDL_RenderPresent(renderer);
		SDL_Delay(timeLeft());
		next_tick += TICK_INTERVAL;
	}
}
