#ifndef COLLISION_GROUP_H
#define COLLISION_GROUP_H
#pragma once
#include <vector>
#include "DrawObject.h"

struct CollisionGroup final
{
	std::vector<DrawObject*> objects;
	std::vector<Aabb*>		 objectsHitBoxes;

	void UpdateHitBoxes()
	{
		if (objects.size() == objectsHitBoxes.size())
		{
			for (int i_Objects = 0; i_Objects < objects.size(); i_Objects++)
			{
				objectsHitBoxes.at(i_Objects)->min = objects.at(i_Objects)->GetMin();
				objectsHitBoxes.at(i_Objects)->max = objects.at(i_Objects)->GetMax();
			}
		}
	}
};

struct CollisionGroupPairing final
{
	CollisionGroup* groupA;
	CollisionGroup* groupB;
};
#endif //COLLISION_GROUP_H

