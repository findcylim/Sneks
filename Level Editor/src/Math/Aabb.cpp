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

	bool CheckPointAabbIntersect(Aabb* a, HTVector2& pos)
	{
		if (pos.x > a->max.x)
			return false;
		if (pos.y > a->max.y)
			return false;
		if (a->min.x > pos.x)
			return false;
		if (a->min.y > pos.y)
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

	Aabb GetAabb(const HTVector2 position, const HTVector2 size, const float scale)
	{
		return { GetMin(position,size,scale),GetMax(position,size,scale) };
	}

	/*
		1- Left
		2- Top
		3- Right
		4- Bottom
	*/
	char CalculateOrientation(Aabb& main, Aabb& orientationTo)
	{
		if (main.min.x > orientationTo.max.x)
			return 3; // Right
		if (main.min.y > orientationTo.max.y)
			return 2; //Top
		if (orientationTo.min.x > main.max.x)
			return 1; // Left
		if (orientationTo.min.y > main.max.y)
			return 4; //Bottom
		return -1;
	}

}