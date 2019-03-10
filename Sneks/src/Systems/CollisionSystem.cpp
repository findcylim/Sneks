
#include "CollisionSystem.h"
#include "../Components/DrawComponent.h"
#include <iostream>


CollisionSystem::CollisionSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{
}

CollisionSystem::~CollisionSystem()
{
	for (auto group : m_xo_ComponentsPerGroup)
	{
		for (auto objectHitBox : group->objectsHitBoxes)
		{
			delete objectHitBox;
		}
		delete group;
	}
	m_o_EventManagerPtr->RemoveListener<Events::EV_ENTITY_POOL_CHANGED>(this);
}

void CollisionSystem::Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	UpdateComponentsPerGroup();
	UpdateAllHitBoxes();
}

void CollisionSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	UpdateComponentsPerGroup();

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
		for (unsigned int i_ObjectA = 0; i_ObjectA < objectsInGroupA->objects.size(); i_ObjectA++)
		{
			// if any object has collision disabled
			if (!objectsInGroupA->objects[i_ObjectA]->enabled || !objectsInGroupA->objects[i_ObjectA]->m_b_IsActive 
			 || !objectsInGroupA->objects[i_ObjectA]->m_po_OwnerEntity->m_b_IsActive)
				continue;

			if (i_CollisionPair.groupB >= m_xo_ComponentsPerGroup.size())
				continue;
			// if group is empty
			auto objectsInGroupB = m_xo_ComponentsPerGroup[i_CollisionPair.groupB];
			for (unsigned int i_ObjectB = 0; i_ObjectB < objectsInGroupB->objects.size(); i_ObjectB++)
			{
				// if any object has collision disabled
				if (!objectsInGroupB->objects[i_ObjectB]->enabled || !objectsInGroupB->objects[i_ObjectB]->m_b_IsActive 
				 || !objectsInGroupB->objects[i_ObjectB]->m_po_OwnerEntity->m_b_IsActive)
					continue;

				auto hitBoxA = objectsInGroupA->objectsHitBoxes[i_ObjectA];
				auto hitBoxB = objectsInGroupB->objectsHitBoxes[i_ObjectB];

				// if hit box is self
				if (hitBoxB != hitBoxA) {
					if (AabbHelper::CheckAabbIntersect(hitBoxA, hitBoxB))
					{
						Events::EV_PLAYER_COLLISION collEvent = { objectsInGroupA->objects[i_ObjectA],
							objectsInGroupB->objects[i_ObjectB]
						};
						/*if (!objectsInGroupA)
						{
							Events::EV_PLAYER_COLLISION_ON_ENTER collEventEnter{ objectsInGroupA->objects[i_ObjectA],
							objectsInGroupB->objects[i_ObjectB]
							};
							//objectsInGroupA->objects[i_ObjectA]->m_b_OnEnter = true;
							m_o_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_ENTER>(collEventEnter);
						}*/
						m_o_EventManagerPtr->EmitEvent<Events::EV_PLAYER_COLLISION>(collEvent);
					}
					/*else
					{
						/* if (objectsInGroupA->objects[i_ObjectA]->m_b_OnEnter)
						{
							Events::EV_PLAYER_COLLISION_ON_EXIT collEventEnter{ objectsInGroupA->objects[i_ObjectA],
							objectsInGroupB->objects[i_ObjectB]
							};
							objectsInGroupA = false;
							m_o_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_EXIT>(collEventEnter);
						}
					}*/
				}
			}
		}
	}
}

void CollisionSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_ENTITY_POOL_CHANGED>(this, this);
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
	for (auto cpg : m_xo_ComponentsPerGroup)
	{
		cpg->objects.clear();
	}
	//m_xo_ComponentsPerGroup.clear();

	auto i_CollisionComponent =
		m_po_ComponentManager->GetFirstComponentInstance<CollisionComponent>(kComponentCollision);

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

void CollisionSystem::UpdateAllHitBoxes()
{
	for (auto i_CollisionGroup : m_xo_ComponentsPerGroup)
	{
		UpdateHitBoxes(i_CollisionGroup);
	}
}

void CollisionSystem::UpdateHitBoxes(CollisionGroup* collisionGroup) const
{
	for (auto aabb : collisionGroup->objectsHitBoxes)
	{
		delete aabb;
	}
	collisionGroup->objectsHitBoxes.clear();
	while (collisionGroup->objects.size() > collisionGroup->objectsHitBoxes.size())
	{
		collisionGroup->objectsHitBoxes.push_back(new Aabb);
	}
	if (collisionGroup->objects.size() == collisionGroup->objectsHitBoxes.size())
	{
		for (unsigned int i_Objects = 0; i_Objects < collisionGroup->objects.size(); i_Objects++)
		{
			collisionGroup->objectsHitBoxes.at(i_Objects)->min = CollisionSystem::GetMin(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent);
			collisionGroup->objectsHitBoxes.at(i_Objects)->max = CollisionSystem::GetMax(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent);
		}
	}
}
