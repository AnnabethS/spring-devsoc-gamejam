#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
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
		    }
	    }
	    // start update

	    updatePlayer(&player);

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

	    drawPlanet(renderer, currentLevel.planetListHead);
	    drawPlayer(renderer, &player);

	    // end render
        SDL_RenderPresent(renderer);
        SDL_Delay(timeLeft());
        next_tick += TICK_INTERVAL;
    }
}
