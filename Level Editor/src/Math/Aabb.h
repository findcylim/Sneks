#ifndef AABB_H
#define AABB_H
#pragma once
#include "../Math/HTVector2.h"

typedef struct {
	HTVector2 min;
	HTVector2 max;
} Aabb;

namespace AabbHelper {
	bool CheckAabbIntersect(Aabb*, Aabb*);
	bool CheckPointAabbIntersect(Aabb* a, HTVector2& pos);
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale);
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale);

}
#endif //AABB_H