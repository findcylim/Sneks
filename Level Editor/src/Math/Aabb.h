#ifndef AABB_H
#define AABB_H
#pragma once
#include "../Math/HTVector2.h"



typedef struct {
	HTVector2 min;
	HTVector2 max;
} Aabb;

bool CheckAabbIntersect(Aabb*, Aabb*);
bool CheckPointAabbIntersect(Aabb* a, HTVector2& pos);
#endif //AABB_H