#ifndef AABB_H
#define AABB_H
#pragma once

typedef struct {
	float x;
	float y;
} Vector2;

typedef struct {
	Vector2 min;
	Vector2 max;
} Aabb;

bool CheckAabbIntersect(Aabb*, Aabb*);

#endif //AABB_H