#include "anna-layer.h"
#include "planet.h"
#include "player.h"
#include "textures.h"
#include "vector.h"
#include "physics.h"
#include "level.h"
#include <SDL2/SDL_render.h>
#include <math.h>

#define RESTITUTION 0.5
#define MINVELSNAP 0.02

local void accelerateRelativeToGravity(player_t* p, planet_t* planet)
{
	vec2f diff;
	vec2fDiff(&diff, &p->pos, &planet->pos);
	float dist = vec2fMagnitude(&diff);
	double force = ((double)planet->mass / (double)pow(dist, 2));
	vec2f acceleration;
	vec2fScalarProduct(&acceleration, &diff, force);
	vec2fAdd(&p->vel, &p->vel, &acceleration);
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
	vec2f xOnlyVel = {p->vel.x, 0};
	vec2f yOnlyVel = {0, p->vel.y};
	vec2f xLoc;
	vec2f yLoc;
	vec2f xyLoc;
	vec2fAdd(&xLoc, &p->pos, &xOnlyVel);
	vec2fAdd(&yLoc, &p->pos, &yOnlyVel);
	vec2fAdd(&xyLoc, &p->pos, &p->vel);
	ptr = currentLevel.planetListHead;
	while(ptr != NULL)
	{
		char bounced = 0;
		if(isColliding(&xLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			p->vel.x *= -RESTITUTION;
			if(p->vel.x >= -MINVELSNAP && p->vel.x <= MINVELSNAP)
				p->vel.x = 0;
		}
		if(isColliding(&yLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			p->vel.y *= -RESTITUTION;
			if(p->vel.y >= -MINVELSNAP && p->vel.y <= MINVELSNAP)
				p->vel.y = 0;
		}
		if(!bounced && isColliding(&xyLoc, 8, &ptr->pos, ptr->radius))
		{
			bounced = 1;
			vec2fScalarProduct(&p->vel, &p->vel, -RESTITUTION);
			if(vec2fMagnitude(&p->vel) <= MINVELSNAP)
			{
				p->vel.x = 0;
				p->vel.y = 0;
			}
		}
		ptr = ptr->next;
		if(bounced)
			break;
	}
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
}

void initPlayer(player_t* p, float x, float y, float textureScale)
{
	p->pos.x = x;
	p->pos.y = y;
	p->vel.x = 0;
	p->vel.y = 0;
	p->rect.w = textures.playerRect.w * textureScale;
	p->rect.h = textures.playerRect.h * textureScale;
	p->radius = p->rect.w / 2;
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
}
