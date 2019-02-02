#ifndef COLLISION_GROUP_H
#define COLLISION_GROUP_H
#pragma once
#include <vector>
#include "../Math/Aabb.h"
#include "../Components/CollisionComponent.h"

struct CollisionGroup final
{
public:
	std::vector<CollisionComponent*> objects;
	std::vector<Aabb*>					objectsHitBoxes;

	void UpdateHitBoxes();

};

struct CollisionGroupPairing final
{
	CollisionGroup* groupA;
	CollisionGroup* groupB;
};
#endif //COLLISION_GROUP_H

