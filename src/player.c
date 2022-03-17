#include "anna-layer.h"
#include "planet.h"
#include "player.h"
#include "textures.h"
#include "vector.h"
#include "physics.h"
#include "level.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdio.h>

#define RESTITUTION 0.5
#define MINVELSNAP 0.02
#define FRAMESPERPREDICTION 15

local void accelerateRelativeToGravity(player_t* p, planet_t* planet)
{
	vec2f diff;
	vec2fDiff(&diff, &p->pos, &planet->pos);
	float dist = vec2fMagnitude(&diff);
	double force = ((double)planet->mass / (double)pow(dist, 2));
	vec2f acceleration;
	vec2fScalarProduct(&acceleration, &diff, force);
	vec2fAdd(&p->trueVel, &p->trueVel, &acceleration);
}

local void recalculateRect(player_t* p)
{
	p->rect.x = p->pos.x - (p->rect.w/2);
	p->rect.y = p->pos.y - (p->rect.h/2);
}

void updatePlayer(player_t* p)
{
	planet_t* ptr = currentLevel.planetListHead;
	while(ptr!= NULL)
	{
		accelerateRelativeToGravity(p, ptr);
		ptr = ptr->next;
	}
	vec2f xOnlyVel = {p->trueVel.x, 0};
	vec2f yOnlyVel = {0, p->trueVel.y};
	vec2f xLoc;
	vec2f yLoc;
	vec2f xyLoc;
	vec2fAdd(&xLoc, &p->pos, &xOnlyVel);
	vec2fAdd(&yLoc, &p->pos, &yOnlyVel);
	vec2fAdd(&xyLoc, &p->pos, &p->trueVel);
	ptr = currentLevel.planetListHead;
	while(ptr != NULL)
	{
		char bounced = 0;
		if(isColliding(&xLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			p->vel.x *= -RESTITUTION;
			p->trueVel.x *= -RESTITUTION;
			if(p->trueVel.x >= -MINVELSNAP && p->trueVel.x <= MINVELSNAP)
				p->trueVel.x = 0;
		}
		if(isColliding(&yLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			p->vel.y *= -RESTITUTION;
			p->trueVel.y *= -RESTITUTION;
			if(p->trueVel.y >= -MINVELSNAP && p->trueVel.y <= MINVELSNAP)
				p->trueVel.y = 0;
		}
		if(!bounced && isColliding(&xyLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			vec2fScalarProduct(&p->trueVel, &p->trueVel, -RESTITUTION);
			vec2fScalarProduct(&p->trueVel, &p->trueVel, -RESTITUTION);
			if(vec2fMagnitude(&p->trueVel) <= MINVELSNAP)
			{
				p->trueVel.x = 0;
				p->trueVel.y = 0;
			}
		}
		ptr = ptr->next;
		if(bounced)
			break;
	}
	p->vel.x = p->trueVel.x;
	p->vel.y = p->trueVel.y;
	vec2fScalarProduct(&p->vel, &p->vel, currentLevel.gameSpeed);
	vec2fAdd(&p->pos, &p->pos, &p->vel);
	if(!currentLevel.playerHoldingDog)
	{
		if(isColliding(&p->pos, p->radius, &currentLevel.dogLocation, currentLevel.dogRadius))
			currentLevel.playerHoldingDog = 1;
	}
	else
	{
		if(isColliding(&p->pos, p->radius, &currentLevel.flagLocation, currentLevel.flagRadius))
		{
			currentLevel.levelWon = 1;
			currentLevel.realPause = 1;
		}
		
	}
	recalculateRect(p);

	// predict next X positions
	player_t fakePlayer = {0};
	fakePlayer.pos.x = p->pos.x;
	fakePlayer.pos.y = p->pos.y;
	fakePlayer.trueVel.x = p->trueVel.x;
	fakePlayer.trueVel.y = p->trueVel.y;


	for(int i=0; i < PREDICTIONS; i++)
	{
		for(int j=0; j < FRAMESPERPREDICTION; j++)
		{
			ptr = currentLevel.planetListHead;
			while(ptr != NULL)
			{
				accelerateRelativeToGravity(&fakePlayer, ptr);
				ptr = ptr->next;
			}
			ptr = currentLevel.planetListHead;
			vec2fAdd(&(fakePlayer.pos), &(fakePlayer.pos), &(fakePlayer.trueVel));
		}

		//TODO: check against all planets, not just the 1st one
		char hitPlanet = 0;
		while(ptr != NULL)
		{
			if(pointInCircle(&(ptr->pos), ptr->radius, &fakePlayer.pos))
			{
				hitPlanet = 1;
				break;
			}
			ptr = ptr->next;
		}
		if(hitPlanet)
		{
			p->posPredicts[i].x = -100;
			p->posPredicts[i].y = -100;
			break;
		}
		else
		{
			p->posPredicts[i].x = fakePlayer.pos.x;
			p->posPredicts[i].y = fakePlayer.pos.y;
		}
	}
}

void initPlayer(player_t* p, float x, float y, float textureScale)
{
	p->pos.x = x;
	p->pos.y = y;
	p->vel.x = 0;
	p->vel.y = 0;
	p->trueVel.x = 0;
	p->trueVel.y = 0;
	p->rect.w = textures.playerRect.w * textureScale;
	p->rect.h = textures.playerRect.h * textureScale;
	p->radius = p->rect.w / 2;
	p->posPredicts[0].x = -100;
	p->posPredicts[0].y = -100;
}

void drawPlayer(SDL_Renderer* r, player_t* p)
{
	SDL_Rect playerRect = {(float)p->rect.x, (float)p->rect.y};
	char onScreen = playerRect.x+playerRect.w >= 0 && playerRect.x <= 1920 &&
		playerRect.y+playerRect.h >= 0 && playerRect.y <= 1080;
	SDL_RenderCopyF(r, textures.sheet, &textures.playerRect, &p->rect);
	float indicatorMultiplier = 1;
	if(!onScreen)
	{
		SDL_Rect indicatorRect = {0};
		indicatorRect.w = indicatorRect.h = 32;
		float rotate = -1;
		if (p->pos.x >= 0 && p->pos.x <= 1920)
		{
			if(p->pos.y <= 0)
			{//top
				indicatorRect.x = p->pos.x - (indicatorRect.w/(float)2);
				indicatorRect.y = 0;
				rotate = -90;
			}
			else if (p->pos.y >= 1080)
			{//bot
				indicatorRect.x = p->pos.x - (indicatorRect.w/(float)2);
				indicatorRect.y = 1080 - indicatorRect.w;
				rotate = 90;
			}
		}
		else if (p->pos.y >= 0 && p->pos.y <= 1080)
		{
			if(p->pos.x <= 0)
			{//left
				indicatorRect.x = 0;
				indicatorRect.y = p->pos.y - (indicatorRect.h/(float)2);
				rotate = 180;
			}
			else if(p->pos.x >= 1920)
			{//right
				indicatorRect.x = 1920 - (indicatorRect.w);
				indicatorRect.y = p->pos.y - (indicatorRect.h/(float)2);
				rotate = 0;
			}
		}
		else if (p->pos.x <= 0 && p->pos.y <= 0)
		{ // top left
			indicatorRect.x = 0;
			indicatorRect.y = 0;
			rotate = -135;
		}
		else if (p->pos.x >= 1920 && p->pos.y <= 0)
		{ // top right
			indicatorRect.x = 1920-indicatorRect.w;
			indicatorRect.y = 0;
			rotate = -45;
		}
		else if (p->pos.x <= 0 && p->pos.y >= 1080)
		{ // bot left
			indicatorRect.x = 0;
			indicatorRect.y = 1080-indicatorRect.h;
			rotate = 135;
		}
		else if (p->pos.x >= 1920 && p->pos.y >= 1080)
		{ // bot right
			indicatorRect.x = 1920-indicatorRect.w;
			indicatorRect.y = 1080-indicatorRect.h;
			rotate = 45;
		}

		if(rotate != -1)
		{
			SDL_Point centre = {indicatorRect.w/2, indicatorRect.h/2};
			SDL_RenderCopyEx(r, textures.sheet, &textures.offScreenIndicator,
							&indicatorRect, rotate, &centre, SDL_FLIP_NONE);
		}
	}
	SDL_Rect indicatorPos = {0,0,8,8};
	for(int i=0; i < PREDICTIONS; i++)
	{
		if(!(p->posPredicts[i].x <= -10 || p->posPredicts[i].y <= -10))
		{
			indicatorPos.x = p->posPredicts[i].x - (float)indicatorPos.w/2;
			indicatorPos.y = p->posPredicts[i].y - (float)indicatorPos.h/2;
			/* SDL_RenderDrawPoint(r, p->posPredicts[i].x, p->posPredicts[i].y); */
			SDL_RenderCopy(r, textures.sheet, &textures.circleIndicator, &indicatorPos);
		}
		else {
			break;
		}
	}
}
