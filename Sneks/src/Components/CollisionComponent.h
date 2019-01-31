#pragma once
#include "../ECS/EntityManager.h"
#include <vector>

class CollisionComponent : public BaseComponent
{
public:
	std::vector<unsigned int> m_i_CollisionGroup;
	TransformComponent*		m_po_TransformComponent;


	CollisionComponent();
	~CollisionComponent();
	void Update(float dt);
	void AddObjectToCollisionGroup(unsigned int collisionGroup);

};