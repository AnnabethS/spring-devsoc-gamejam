#include "anna-layer.h"
#include "planet.h"
#include "player.h"
#include "textures.h"
#include "vector.h"
#include "physics.h"
#include "level.h"

#define RESTITUTION 0.75
#define MINVELSNAP 0.02

local void accelerateRelativeToGravity(player_t* p, planet_t* planet)
{
	vec2f diff;
	vec2fDiff(&diff, &p->pos, &planet->pos);
	float dist = vec2fMagnitude(&diff);
	double force = 0.005 * ((double)planet->mass / (double)dist);
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
	SDL_RenderCopyF(r, textures.sheet, &textures.playerRect, &p->rect);
}
