#include "level.h"
#include "planet.h"

level_t currentLevel;

void loadLevel(int levelNum)
{
	initPlanet(&currentLevel.planetListHead, 800, 450, 8);
}

void unloadLevel()
{
	
}
