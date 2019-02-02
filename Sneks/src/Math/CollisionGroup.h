#ifndef COLLISION_GROUP_H
#define COLLISION_GROUP_H
#pragma once
#include <vector>
#include "../Graphics/DrawObject.h"
#include "../Math/Aabb.h"
#include "../Components/CollisionComponent.h"
#include "../Systems/CollisionSystem.h"

struct CollisionGroup final
{
	std::vector<CollisionComponent*> objects;
	std::vector<Aabb*>					objectsHitBoxes;

	void UpdateHitBoxes()
	{
		if (objects.size() == objectsHitBoxes.size())
		{
			for (int i_Objects = 0; i_Objects < objects.size(); i_Objects++)
			{
				objectsHitBoxes.at(i_Objects)->min = CollisionSystem::GetMin(objects.at(i_Objects)->m_po_DrawComponent);
				objectsHitBoxes.at(i_Objects)->max = CollisionSystem::GetMax(objects.at(i_Objects)->m_po_DrawComponent);
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

