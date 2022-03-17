#ifndef PLAYER
#define PLAYER

#include <SDL2/SDL.h>
#include "vector.h"

#define PREDICTIONS 20

typedef struct{
	vec2f pos; // position of the centre of the player
	vec2f vel; // down + right is positive
	vec2f trueVel; // unscaled velocity
	SDL_FRect rect; // rect used for drawing
	float radius;
	vec2f posPredicts[PREDICTIONS];
}player_t;


void updatePlayer(player_t* p);
void initPlayer(player_t* p, float x, float y, float textureScale);
void drawPlayer(SDL_Renderer* r, player_t* p);

#endif
