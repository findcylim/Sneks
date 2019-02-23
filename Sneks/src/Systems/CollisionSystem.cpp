
#include "CollisionSystem.h"
#include "../Components/DrawComponent.h"
#include <iostream>


CollisionSystem::CollisionSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{
}

CollisionSystem::~CollisionSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::EV_ENTITY_POOL_CHANGED>(this);
}

void CollisionSystem::receive(const Events::EV_ENTITY_POOL_CHANGED& eventData)
{
	UpdateComponentsPerGroup();
}

void CollisionSystem::Update(float dt)
{
	//	UpdateComponentsPerGroup();

		//Update Aabb positions
	for (auto i_CollisionGroup : m_xo_ComponentsPerGroup)
	{
		UpdateHitBoxes(i_CollisionGroup);
	}

	//Check collisions between all objects in one group
	// and another
	for (auto i_CollisionPair : m_vx_CollisionsPairings)
	{
		//if group is empty
		if (i_CollisionPair.groupA >= m_xo_ComponentsPerGroup.size())
			continue;
		auto objectsInGroupA = m_xo_ComponentsPerGroup[i_CollisionPair.groupA];
		for (int i_ObjectA = 0; i_ObjectA < objectsInGroupA->objects.size(); i_ObjectA++)
		{
			// if any object has collision disabled
			if (!objectsInGroupA->objects[i_ObjectA]->enabled)
				continue;
			if (i_CollisionPair.groupB >= m_xo_ComponentsPerGroup.size())
				continue;
			// if group is empty
			auto objectsInGroupB = m_xo_ComponentsPerGroup[i_CollisionPair.groupB];
			for (int i_ObjectB = 0; i_ObjectB < objectsInGroupB->objects.size(); i_ObjectB++)
			{
				// if any object has collision disabled
				if (!objectsInGroupB->objects[i_ObjectB]->enabled)
					continue;
				auto hitBoxA = objectsInGroupA->objectsHitBoxes[i_ObjectA];
				auto hitBoxB = objectsInGroupB->objectsHitBoxes[i_ObjectB];
				// if hit box is self
				if (hitBoxB != hitBoxA) {
					if (AabbHelper::CheckAabbIntersect(hitBoxA, hitBoxB))
					{
						Events::EV_PLAYER_COLLISION collEvent{ objectsInGroupA->objects[i_ObjectA],
							objectsInGroupB->objects[i_ObjectB]
						};
						m_o_EventManagerPtr->EmitEvent<Events::EV_PLAYER_COLLISION>(collEvent);
					}
				}
			}
		}
	}
}

void CollisionSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_ENTITY_POOL_CHANGED>(this);
	UpdateComponentsPerGroup();
}

void CollisionSystem::AddComponentToCollisionGroup(CollisionComponent* collisionComponent, unsigned int collisionGroup)
{
	while (m_xo_ComponentsPerGroup.size() < collisionGroup + 1)
	{
		m_xo_ComponentsPerGroup.push_back(new CollisionGroup);
	}
	m_xo_ComponentsPerGroup[collisionGroup]->objects.push_back(collisionComponent);
}

void CollisionSystem::UpdateComponentsPerGroup()
{
	m_xo_ComponentsPerGroup.clear();
	auto i_CollisionComponent = static_cast<CollisionComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentCollision));

	while (i_CollisionComponent)
	{
		for (auto i_CollisionGroup : i_CollisionComponent->m_i_CollisionGroupVec)
		{
			//Add it to the group
			AddComponentToCollisionGroup(i_CollisionComponent, i_CollisionGroup);
		}
		//Iterate
		i_CollisionComponent = static_cast<CollisionComponent*>(i_CollisionComponent->m_po_NextComponent);
	}
}


HTVector2 CollisionSystem::GetMin(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMin(drawComponent->m_po_TransformComponent->m_x_Position, 
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->m_f_Scale);
}

HTVector2 CollisionSystem::GetMax(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMax(drawComponent->m_po_TransformComponent->m_x_Position,
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->m_f_Scale);
}

void CollisionSystem::UpdateHitBoxes(CollisionGroup* collisionGroup) const
{
	while (collisionGroup->objects.size() > collisionGroup->objectsHitBoxes.size())
	{
		collisionGroup->objectsHitBoxes.push_back(new Aabb);
	}
	if (collisionGroup->objects.size() == collisionGroup->objectsHitBoxes.size())
	{
		for (int i_Objects = 0; i_Objects < collisionGroup->objects.size(); i_Objects++)
		{
			collisionGroup->objectsHitBoxes.at(i_Objects)->min = CollisionSystem::GetMin(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent);
			collisionGroup->objectsHitBoxes.at(i_Objects)->max = CollisionSystem::GetMax(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent);
		}
	}
}
