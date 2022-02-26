#include "level.h"
#include "planet.h"
#include "anna-layer.h"

level_t currentLevel;

// loads a new planet into the global level struct at the head of the linked list
local void loadNewPlanet(int x, int y, int mass)
{
	planet_t* oldHead = currentLevel.planetListHead;
	initPlanet(&currentLevel.planetListHead, x, y, mass);
	currentLevel.planetListHead->next = oldHead;
}

void loadLevel(int levelNum)
{
	switch(levelNum)
	{
	case 0:
		loadNewPlanet(800, 450, 8);
		loadNewPlanet(300, 700, 4);
		currentLevel.maxBullets = 5;
		break;
	}
	currentLevel.currentBullets = currentLevel.maxBullets;
}

void unloadLevel()
{
	
}
