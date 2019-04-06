/* Start Header****************************************************************/
/*!
\file CollisionSystem.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 10

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "CollisionSystem.h"
#include "../Components/DrawComponent.h"
#include <iostream>

std::vector<CollisionGroupPairing> CollisionSystem::m_vx_CollisionsPairings =
{ {kCollGroupSnek1Head,kCollGroupSnek2Head},
 {kCollGroupSnek1Head,kCollGroupSnek2Body}, //Snek Head and Other Head
 {kCollGroupSnek1Head,kCollGroupBuilding },
 {kCollGroupSnek1Head,kCollGroupPowerUp },
 {kCollGroupSnek2Head,kCollGroupSnek1Body},
 {kCollGroupSnek2Head,kCollGroupBuilding },
 {kCollGroupSnek2Head,kCollGroupPowerUp },
 {kCollGroupMoon		 ,kCollGroupSnek1Head}, //Moon and Other Head
 {kCollGroupMoon		 ,kCollGroupSnek1Body}, //Moon and Other Body
 {kCollGroupMoon		 ,kCollGroupSnek2Head}, //Moon and Other Head
 {kCollGroupMoon		 ,kCollGroupSnek2Body}, //Moon and Other Body
 {kCollGroupMoon		 ,kCollGroupBuilding },  //Moon and Buildings
 {kCollGroupMouse    ,kCollGroupUIButton}
};

CollisionSystem::CollisionSystem()
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

//MARK
HTVector2 CollisionSystem::GetMin(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMin(drawComponent->m_po_TransformComponent->m_x_Position, 
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->GetDrawScale());
}

HTVector2 CollisionSystem::GetMax(DrawComponent* drawComponent) 
{
	return AabbHelper::GetMax(drawComponent->m_po_TransformComponent->m_x_Position,
		drawComponent->m_x_MeshSize, drawComponent->m_po_TransformComponent->GetDrawScale());
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
