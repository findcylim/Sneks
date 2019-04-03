
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
		/*for (auto objectHitBox : group->objectsHitBoxes)
		{
			delete objectHitBox;
		}*/
		delete group;
	}
	m_po_EventManagerPtr->RemoveListener<Events::EV_ENTITY_POOL_CHANGED>(this);
}

void CollisionSystem::Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	//UpdateComponentsPerGroup();
	//UpdateAllHitBoxes();
}

void CollisionSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	UpdateComponentsPerGroup();

	//Update Aabb positions
	UpdateAllHitBoxes();

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
			auto objectA = objectsInGroupA->objects[i_ObjectA];
			// if any object has collision disabled
			if (!objectA->enabled || !objectA->m_b_IsActive || !objectA->m_po_OwnerEntity->m_b_IsActive ||
				objectA->m_i_CollisionsThisFrame >= objectA->m_i_NumCollisionsAllowedPerFrame)
				continue;

			// if group is empty
			if (i_CollisionPair.groupB >= m_xo_ComponentsPerGroup.size())
				continue;
			auto objectsInGroupB = m_xo_ComponentsPerGroup[i_CollisionPair.groupB];
			for (unsigned int i_ObjectB = 0; i_ObjectB < objectsInGroupB->objects.size(); i_ObjectB++)
			{
				auto objectB = objectsInGroupB->objects[i_ObjectB];
				// if any object has collision disabled
				if (!objectB->enabled || !objectB->m_b_IsActive || !objectB->m_po_OwnerEntity->m_b_IsActive ||
					 objectB->m_i_CollisionsThisFrame >= objectB->m_i_NumCollisionsAllowedPerFrame)
					continue;

				auto hitBoxA = objectsInGroupA->objectsHitBoxes[i_ObjectA];
				auto hitBoxB = objectsInGroupB->objectsHitBoxes[i_ObjectB];

				// if hit box is self
				if (hitBoxB != hitBoxA) {
					if (AabbHelper::CheckAabbIntersect(hitBoxA, hitBoxB))
					{
						++objectA->m_i_CollisionsThisFrame;
						++objectB->m_i_CollisionsThisFrame;

						Events::EV_PLAYER_COLLISION collEvent = { objectA,
							objectB
						};
						/*if (!objectA->m_b_Colliding )
						{
							objectA->m_b_Colliding = true;
							Events::EV_PLAYER_COLLISION_ON_ENTER collEventEnter
							{ objectA };
							m_po_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_ENTER>(collEventEnter);
						}
						if (!objectB->m_b_Colliding)
						{
							objectB->m_b_Colliding = true;
							Events::EV_PLAYER_COLLISION_ON_ENTER collEventEnter
							{ objectB };
							m_po_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_ENTER>(collEventEnter);
						}*/
						m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_COLLISION>(collEvent);
						break;
					}
					/*else
					{
						if (objectA->m_b_Colliding)
						{
							objectA->m_b_Colliding = false;
							Events::EV_PLAYER_COLLISION_ON_EXIT collEventEnter
							{ objectA };
							m_po_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_EXIT>(collEventEnter);
						}
						if (objectB->m_b_Colliding)
						{
							objectB->m_b_Colliding = false;
							Events::EV_PLAYER_COLLISION_ON_EXIT collEventEnter
							{ objectB };
							m_po_EventManagerPtr->EmitEvent < Events::EV_PLAYER_COLLISION_ON_EXIT>(collEventEnter);
						}
					}*/
				}
			}
		}
	}
}

void CollisionSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_ENTITY_POOL_CHANGED>(this, this);
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

	m_po_ComponentManager->Each<CollisionComponent>([&](CollisionComponent* i_CollisionComponent)->void
	{
		i_CollisionComponent->m_i_CollisionsThisFrame = 0;
		if (i_CollisionComponent->m_b_IsActive)
			for (auto i_CollisionGroup : i_CollisionComponent->m_i_CollisionGroupVec)
			{
				//Add it to the group
				AddComponentToCollisionGroup(i_CollisionComponent, i_CollisionGroup);
			}
	}, kComponentCollision);


	
}


HTVector2 CollisionSystem::GetMin(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMin(drawComponent->m_po_TransformComponent->m_x_Position, 
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->GetScale());
}

HTVector2 CollisionSystem::GetMax(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMax(drawComponent->m_po_TransformComponent->m_x_Position,
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->GetScale());
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

	collisionGroup->objectsHitBoxes.clear();


	for (unsigned int i_Objects = 0; i_Objects < collisionGroup->objects.size(); i_Objects++)
	{
		Aabb aabb ={
		CollisionSystem::GetMin(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent),
		CollisionSystem::GetMax(collisionGroup->objects.at(i_Objects)->m_po_DrawComponent) 
		};
		collisionGroup->objectsHitBoxes.push_back(aabb);
	}

	AE_ASSERT(collisionGroup->objects.size() == collisionGroup->objectsHitBoxes.size() && "hitbox count wrong");
}
