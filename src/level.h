#ifndef LEVEL
#define LEVEL

#include "planet.h"

typedef struct{
	planet_t* planetListHead;
}level_t;

extern level_t currentLevel;

void loadLevel(int levelNum);

#endif
