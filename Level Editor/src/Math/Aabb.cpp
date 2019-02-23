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

	HTVector2 GetMin(const HTVector2 position, const HTVector2 size, const HTVector2 scale)
	{
		HTVector2 min ={};
		min.x = position.x - size.x * scale.x / 2;
		min.y = position.y - size.y * scale.y / 2;
		return min;
	}

	HTVector2 GetMax(const HTVector2 position, const HTVector2 size, const HTVector2 scale)
	{
		HTVector2 max ={};
		max.x = position.x + size.x * scale.x / 2;
		max.y = position.y + size.y * scale.y / 2;
		return max;
	}

	Aabb GetAabb(const HTVector2 position, const HTVector2 size, const HTVector2 scale)
	{
		return { GetMin(position,size,scale),GetMax(position,size,scale) };
	}

	/*
		1- Left
		2- Top
		3- Right
		4- Bottom
	*/
	char CalculateOrientation(HTVector2 main, HTVector2 orientationTo)
	{
		float xDiff = main.x - orientationTo.x;
		float yDiff = main.y - orientationTo.y;
		
		if (xDiff < 0)
		{
			if (yDiff > 0)
				if (MathHT::Abs(xDiff) > yDiff)
					return 1;
				else
					return 2;
			if (yDiff < 0)
				if (yDiff < xDiff)
					return 4;
				else
					return 1;
			return 1;
		}
		else if (xDiff > 0)
		{
			if (yDiff > 0)
				if (xDiff > yDiff)
					return 3;
				else
					return 2;
			if (yDiff < 0)
				if (MathHT::Abs(yDiff) > xDiff)
					return 4;
				else
					return 3;
			return 3;
		}
		else
		{
			if (yDiff > 0)
				return 2;
			else
				return 4;
		}
	}

}