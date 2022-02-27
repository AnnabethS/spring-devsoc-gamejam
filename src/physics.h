#ifndef PHYSICS
#define PHYSICS

#include "vector.h"

static char isColliding(vec2f* c1Pos, float c1Rad, vec2f* c2Pos, float c2Rad)
{
	float* biggestRadius;
	if(c1Rad >= c2Rad)
		biggestRadius = &c1Rad;
	else
		biggestRadius = &c2Rad;

	return vec2fDist(c1Pos, c2Pos) <= *biggestRadius;
}

static char pointInCircle(vec2f* circlePos, float circleRadius, vec2f* point)
{
	return vec2fDist(circlePos, point) <= circleRadius;
}

#endif
