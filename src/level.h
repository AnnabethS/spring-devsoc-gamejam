#ifndef LEVEL
#define LEVEL

#include "planet.h"
#include "player.h"
#include "bullet.h"

typedef struct{
	planet_t* planetListHead;
	int maxBullets;
	int currentBullets;
	bullet_t* bulletListHead;
	vec2f dogLocation;
	SDL_Rect dogRect;
	float dogRadius;
	char playerHoldingDog;
	vec2f flagLocation;
	SDL_Rect flagRect;
	float flagRadius;
	char levelWon;
	char paused; //used when the player is stopping time
	float gameSpeed; // used to slow down the game
	char realPause; // used when the game is done
	player_t player;
	int levelNum;
}level_t;

extern level_t currentLevel;

void loadLevel(int levelNum);
void unloadLevel();

#endif
