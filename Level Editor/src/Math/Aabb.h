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
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
	Aabb GetAabb(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
	char CalculateOrientation(HTVector2 main, HTVector2 orientationTo);
}
#endif //AABB_H