#ifndef COLLISIONS_H
#define COLLISIONS_H
#pragma once
#include <vector>
#include "CollisionGroup.h"

class Collisions
{
private:
	std::vector<CollisionGroup>		  m_vx_CollisionGroups;
	std::vector<CollisionGroupPairing> m_vx_CollisionsPairings;

public:
	Collisions();
	~Collisions();
	void Update(float dt);
	void AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup);

};

#endif //COLLISIONS_H