
#include "CollisionSystem.h"
#include "../Components/DrawComponent.h"


CollisionSystem::CollisionSystem()
{

}

CollisionSystem::~CollisionSystem()
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
				cameraShake->AddShake(20.0f);
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
		i_CollisionGroup.UpdateHitBoxes();
	}

	//Check collisions between all objects in one group
	// and another
	for (auto i_CollisionPair : m_vx_CollisionsPairings)
	{
		for (auto i_ObjectGroupA : i_CollisionPair.groupA->objectsHitBoxes)
		{
			for (auto i_ObjectGroupB : i_CollisionPair.groupB->objectsHitBoxes)
			{
				//Don't check collision with self
				if (i_ObjectGroupB != i_ObjectGroupA) {
					if (AabbHelper::CheckAabbIntersect(i_ObjectGroupA, i_ObjectGroupB))
					{
						//DO SOMETHING?
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
	while (m_xo_ComponentsPerGroup.size() - 1 < collisionGroup)
	{
		m_xo_ComponentsPerGroup.emplace_back();
	}
	m_xo_ComponentsPerGroup[collisionGroup].objects.push_back(collisionComponent);
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