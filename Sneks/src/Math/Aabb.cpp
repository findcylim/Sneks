#include "Aabb.h"

namespace AabbHelper {
	bool CheckAabbIntersect(Aabb* a, Aabb* b)
	{
		if (b->min.x > a->max.x)
			return false;
		if (b->min.y > a->max.y)
			return false;
		if (a->min.x > b->max.x)
			return false;
		if (a->min.y > b->max.y)
			return false;
		return true;
	}

	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale)
	{
		HTVector2 min ={};
		min.x = position.x - size.x * scale / 2;
		min.y = position.y - size.y * scale / 2;
		return min;
	}

	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale)
	{
		HTVector2 max ={};
		max.x = position.x + size.x * scale / 2;
		max.y = position.y + size.y * scale / 2;
		return max;
	}

}