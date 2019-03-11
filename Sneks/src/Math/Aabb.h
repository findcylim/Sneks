#ifndef AABB_H
#define AABB_H
#pragma once
#include "../Math/HTVector2.h"

struct Aabb{
	HTVector2 min;
	HTVector2 max;

	bool operator ==(const Aabb& rhs) const
	{
		return (min == rhs.min && max == rhs.max);
	}
	bool operator !=(const Aabb& rhs) const
	{
		return !(*this == rhs);
	}

};

namespace AabbHelper {
	bool CheckAabbIntersect(const Aabb&, const Aabb&);
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const float scale);
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const float scale);
	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const HTVector2 scale);
	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const HTVector2 scale);


}
#endif //AABB_H