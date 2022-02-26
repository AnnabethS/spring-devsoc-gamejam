#ifndef VECTORS
#define VECTORS

#include <math.h>

typedef struct{
	int x;
	int y;
}vec2;

typedef struct{
	float x;
	float y;
}vec2f;

static void vec2fAdd(vec2f* dest, vec2f* src1, vec2f* src2);
static float vec2fMagnitude(vec2f* v);
static void vec2fDiff(vec2f* dest, vec2f* v1, vec2f* v2);
static float vec2fDist(vec2f* v1, vec2f* v2);

static void vec2fAdd(vec2f* dest, vec2f* v1, vec2f* v2)
{
	dest->x = v1->x + v2->x;
	dest->y = v1->y + v2->y;
}

static float vec2fDist(vec2f* v1, vec2f* v2)
{
	vec2f diffVec;
	vec2fDiff(&diffVec, v1, v2);
	return vec2fMagnitude(&diffVec);
}

static void vec2fDiff(vec2f* dest, vec2f* v1, vec2f* v2)
{
	dest->x = v2->x - v1->x;
	dest->y = v2->y - v1->y;
}

static float vec2fMagnitude(vec2f* v)
{
	return sqrt(pow(v->x, 2) + pow(v->y, 2));
}

static void vec2fScalarProduct(vec2f* dest, vec2f* v1, float scalar)
{
	dest->x = v1->x * scalar;
	dest->y = v1->y * scalar;
}

#endif
