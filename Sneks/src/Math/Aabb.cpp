#include "Aabb.h"


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
