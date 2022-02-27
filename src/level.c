#include "level.h"
#include "bullet.h"
#include "planet.h"
#include "anna-layer.h"
#include "player.h"
#include "textures.h"

level_t currentLevel;

// loads a new planet into the global level struct at the head of the linked list
local void loadNewPlanet(int x, int y, int mass)
{
	planet_t* oldHead = currentLevel.planetListHead;
	initPlanet(&currentLevel.planetListHead, x, y, mass);
	currentLevel.planetListHead->next = oldHead;
}

local void setDog(float x, float y, float scale)
{
	currentLevel.dogLocation.x = x;
	currentLevel.dogLocation.y = y;
	currentLevel.dogRect.w = textures.dogRect.w*scale;
	currentLevel.dogRect.h = textures.dogRect.h*scale;
	currentLevel.dogRect.x = x - currentLevel.dogRect.w / (float) 2;
	currentLevel.dogRect.y = y - currentLevel.dogRect.h / (float) 2;
	currentLevel.dogRadius = currentLevel.dogRect.w / (float) 2;
}

local void setFlag(float x, float y, float scale)
{
	currentLevel.flagLocation.x = x;
	currentLevel.flagLocation.y = y;
	currentLevel.flagRect.w = textures.flagRect.w*scale;
	currentLevel.flagRect.h = textures.flagRect.h*scale;
	currentLevel.flagRect.x = x - currentLevel.flagRect.w / (float) 2;
	currentLevel.flagRect.y = y - currentLevel.flagRect.h / (float) 2;
	currentLevel.flagRadius = currentLevel.flagRect.w / (float) 2;
}

void loadLevel(int levelNum)
{
	switch(levelNum)
	{
	case 0:
		loadNewPlanet(800, 450, 8);
		//loadNewPlanet(300, 700, 4);
		currentLevel.maxBullets = 10;
		setDog(1000, 450, 2);
		setFlag(300, 450, 4);
		initPlayer(&currentLevel.player, 200, 200, 2);
		break;
	case 1:
		loadNewPlanet(800, 450, 16);
		currentLevel.maxBullets = 8;
		setDog(800, 700, 2);
		setFlag(200, 500, 4);
		initPlayer(&currentLevel.player, 50, 50, 2);
		break;
	case 2:
		loadNewPlanet(400, 800, 6);
		loadNewPlanet(800, 400, 6);
		setDog(800, 800, 2);
		setFlag(900, 420, 4);
		currentLevel.maxBullets = 6;
		initPlayer(&currentLevel.player, 150, 150, 2);
		break;
	case 3:
		loadNewPlanet(600, 500, 10);
		loadNewPlanet(350, 800, 6);
		setDog(600, 400, 2);
		setFlag(450,900,4);
		currentLevel.maxBullets = 6;
		initPlayer(&currentLevel.player, 50, 50, 2);
		break;
	case 4:
		loadNewPlanet(800, 450, 8);
		loadNewPlanet(300, 700, 4);
		currentLevel.maxBullets = 5;
		setDog(1000, 450, 2);
		setFlag(300, 450, 4);
		initPlayer(&currentLevel.player, 200, 200, 2);
		break;
	default:
		printf("cannot load level %d, does not exist yet\n"
		       "program will likely now segfault or something\n", levelNum);
		break;
	}
	currentLevel.playerHoldingDog = 0;
	currentLevel.currentBullets = currentLevel.maxBullets;
	currentLevel.paused = 1;
	currentLevel.gameSpeed = 1;
	currentLevel.levelNum = levelNum;
	currentLevel.levelWon = 0;
	currentLevel.realPause = 0;
	currentLevel.bulletListHead = NULL;
}

void unloadLevel()
{
	planet_t* ptr = currentLevel.planetListHead;
	while(ptr != NULL)
	{
		planet_t* next = ptr->next;
		free(ptr);
		ptr = next;
	}
	bullet_t* bptr = currentLevel.bulletListHead;
	while(bptr != NULL)
	{
		bullet_t* next = bptr->next;
		free(bptr);
		bptr = next;
	}
	currentLevel.bulletListHead = NULL;
	currentLevel.planetListHead = NULL;
	currentLevel.currentBullets = 0;
}
