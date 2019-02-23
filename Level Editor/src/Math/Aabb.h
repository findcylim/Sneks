#ifndef AABB_H
#define AABB_H
#pragma once
#include "../Math/HTVector2.h"

struct Aabb {
	HTVector2 min;
	HTVector2 max;
};

namespace AabbHelper {
	bool CheckAabbIntersect(Aabb*, Aabb*);
	bool CheckPointAabbIntersect(Aabb* a, HTVector2& pos);
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale);
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale);
	Aabb GetAabb(const HTVector2 position, const HTVector2 size, const float scale);
	char CalculateOrientation(Aabb& main, Aabb& orientationTo);
}
#endif //AABB_H