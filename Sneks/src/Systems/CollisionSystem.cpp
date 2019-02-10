
#include "CollisionSystem.h"
#include "../Components/DrawComponent.h"
#include <iostream>


CollisionSystem::CollisionSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{

}

void CollisionSystem::Update(float dt)
{


	/*Aabb snekHeadAabb ={};
		snekHeadAabb.min = snek->m_po_Head->GetMin();
		snekHeadAabb.max = snek->m_po_Head->GetMax();
		Aabb snekHeadAabb2 ={};
		snekHeadAabb2.min = snek2->m_po_Head->GetMin();
		snekHeadAabb2.max = snek2->m_po_Head->GetMax();

		//Head on head action
		if (CheckAabbIntersect(&snekHeadAabb, &snekHeadAabb2))
		{
			//check iframes
			if (snek->m_po_Head->GetInvulnerable() > 0 || snek2->m_po_Head->GetInvulnerable() > 0){}
			else {
				snek->m_po_Head->SetInvulnerable(1.0f);
				snek2->m_po_Head->SetInvulnerable(1.0f);
				if (snek->m_v_BodyParts.empty()) {
					auto chars = new s8[100];
					sprintf_s(chars, 100, "PLAYER 1 WINS");

					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					AEGfxSetBlendMode(AE_GFX_BM_BLEND);

					//AEGfxPrint(winFont, chars, 0, 0, 0, 0, 1);
					MessageBox(nullptr, "PLAYER 1 WINS", "ENDGAME", MB_OK);
					return 1;
					winner = 1;
				}
				else if (snek2->m_v_BodyParts.empty())
				{
					auto chars = new s8[100];
					sprintf_s(chars, 100, "PLAYER 2 WINS");

					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					AEGfxSetBlendMode(AE_GFX_BM_BLEND);

					//AEGfxPrint(winFont, chars, 0, 0, 1, 0, 0);
					MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);
					return 1;
					winner = 2;

				}
				snek2->m_v_BodyParts.pop_back();
				snek->m_v_BodyParts.pop_back();
				snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
				snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
			}
		}*/
		//Update Aabb positions
	for (auto i_CollisionGroup : m_xo_ComponentsPerGroup)
	{
		UpdateHitBoxes(i_CollisionGroup);
	}

	//Check collisions between all objects in one group
	// and another
	for (auto i_CollisionPair : m_vx_CollisionsPairings)
	{
		auto objectsInGroupA = m_xo_ComponentsPerGroup[i_CollisionPair.groupA];
		for (int i_ObjectA = 0; i_ObjectA < objectsInGroupA->objects.size(); i_ObjectA++)
		{
			if (!objectsInGroupA->objects[i_ObjectA]->enabled)
				continue;
			auto objectsInGroupB = m_xo_ComponentsPerGroup[i_CollisionPair.groupB];
			for (int i_ObjectB = 0; i_ObjectB < objectsInGroupB->objects.size(); i_ObjectB++)
			{
				if (!objectsInGroupB->objects[i_ObjectB]->enabled)
					continue;
				auto hitBoxA = objectsInGroupA->objectsHitBoxes[i_ObjectA];
				auto hitBoxB = objectsInGroupB->objectsHitBoxes[i_ObjectB];
				//Don't check collision with self
				if (hitBoxB != hitBoxA) {
					if (AabbHelper::CheckAabbIntersect(hitBoxA, hitBoxB))
					{
						Events::Ev_PLAYER_COLLISION collEvent { objectsInGroupA->objects[i_ObjectA],
							objectsInGroupB->objects[i_ObjectB]
						};
						m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER_COLLISION>(collEvent);
					}
				}
			}
		}
	}
	
}

void CollisionSystem::Initialize()
{
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

void CollisionSystem::UpdateHitBoxes(CollisionGroup* collisionGroup)
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
