#include "Collisions.h"



Collisions::Collisions()
{
}


Collisions::~Collisions()
{
}

void Collisions::Update(float dt)
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
	for (auto i_CollisionGroup : m_vx_CollisionGroups)
	{
		i_CollisionGroup.UpdateHitboxes();
	}

	//Check collisions between all objects in one group
	// and another
	for (auto i_CollisionPair : m_vx_CollisionsPairings)
	{
		for (auto i_ObjectGroupA : i_CollisionPair.groupA->objectsHitboxes)
		{
			for (auto i_ObjectGroupB : i_CollisionPair.groupB->objectsHitboxes)
			{
				//Don't check collision with self
				if (i_ObjectGroupB != i_ObjectGroupA) {
					if (CheckAabbIntersect(i_ObjectGroupA, i_ObjectGroupB))
					{
						//DO SOMETHING?
					}
				}
			}
		}
	}
}

void Collisions::AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup)
{
	while (m_vx_CollisionGroups.size()<collisionGroup)
	{
		m_vx_CollisionGroups.push_back(CollisionGroup());
	}
}
