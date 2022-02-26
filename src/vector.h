#ifndef VECTORS
#define VECTORS

#define PI 3.14152
#define RAD2DEG 180 / PI
#define DEG2RAD PI / 180

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

static void vec2fDiff(vec2f* dest, vec2f* fromVector, vec2f* toVector)
{
	dest->x = toVector->x - fromVector->x;
	dest->y = toVector->y - fromVector->y;
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

static float vec2fAngleRadians(vec2f* fromVector, vec2f* toVector)
{
	vec2f diff;
	vec2fDiff(&diff, fromVector, toVector);
	return atan2(diff.y, diff.x);
}

static float vec2fAngleDegrees(vec2f* fromVector, vec2f* toVector)
{
	float rads = vec2fAngleRadians(fromVector, toVector);
	return rads * RAD2DEG;
}

static void vec2fNormalise(vec2f* dest, vec2f* src)
{
	float length = vec2fMagnitude(src);
	dest->x = src->x / length;
	dest->y = src->y / length;
}

static void vec2fUnitVectorFromAngleDegrees(vec2f* dest, float angle)
{
	dest->x = cos(angle * DEG2RAD);
	dest->y = sin(angle * DEG2RAD);
}

#endif
