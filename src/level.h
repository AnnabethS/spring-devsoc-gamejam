#ifndef LEVEL
#define LEVEL

#include "planet.h"

typedef struct{
	planet_t* planetListHead;
	int maxBullets;
	int currentBullets;
}level_t;

extern level_t currentLevel;

void loadLevel(int levelNum);

#endif
