/* Start Header ***************************************************************/
/*!
\file Tint.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: XX/XX/2019
\date Modified: XX/XX/2019
\brief This file contains the template to copy paste when creating new systems.

\par Contribution (hours): CY - 0.5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#include "TintSystem.h"
#include "../Components/TintComponent.h"
#include "CollisionSystem.h"
#include "../Components/FollowComponent.h"


void TintSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	CollisionGroupName collGroup1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName collGroup2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupPairing colPairing ={ collGroup1, collGroup2 };

	if (colPairing == CollisionGroupPairing{ kCollGroupSnek2Head, kCollGroupSnek1Body } ||
		 colPairing == CollisionGroupPairing{ kCollGroupSnek1Head, kCollGroupSnek2Body } )
	{
		auto victimHeadComp = eventData.object2->GetComponent<FollowComponent>()->
									 m_po_ParentEntity->GetComponent<SnekHeadComponent>();
		victimHeadComp->GetComponent<TintComponent>()->m_f_TintDuration = 0.5f;
		for (auto& bodyParts : victimHeadComp->m_x_BodyParts)
		{
			bodyParts->GetComponent<TintComponent>()->m_f_TintDuration = 0.5f;
		}
	}

	if (colPairing == CollisionGroupPairing{ kCollGroupSnek1Head, kCollGroupSnek2Head })
	{
		for (auto victim = eventData.object2;; victim = eventData.object1)
		{
			auto victimHeadComp = victim->GetComponent<SnekHeadComponent>();

			victimHeadComp->GetComponent<TintComponent>()->m_f_TintDuration = 0.5f;
			for (auto& bodyParts : victimHeadComp->m_x_BodyParts)
			{
				bodyParts->GetComponent<TintComponent>()->m_f_TintDuration = 0.5f;
			}
			if (victim == eventData.object1)
				break;
		}
	}
}

TintSystem::TintSystem()
{
}

TintSystem::~TintSystem()
{
}

void TintSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}

void TintSystem::Update(float dt)
{
	m_po_ComponentManager->Each<TintComponent>([&](TintComponent* tintComponent)
	{
		if (tintComponent->m_f_TintDuration <= 0)
			return;
		auto drawComp = tintComponent->GetComponent<DrawComponent>();

		if (tintComponent->m_f_CurrentTintTimer < tintComponent->m_f_TintDuration)
		{
			tintComponent->m_x_TintColor.blue = tintComponent->m_f_CurrentTintTimer / tintComponent->m_f_TintDuration;
			tintComponent->m_x_TintColor.green = tintComponent->m_f_CurrentTintTimer / tintComponent->m_f_TintDuration;
			drawComp->m_f_RgbaColor = tintComponent->m_x_TintColor;
			tintComponent->m_f_CurrentTintTimer += dt;
		}
		else
		{
			tintComponent->m_f_CurrentTintTimer = 0;
			tintComponent->m_f_TintDuration = 0;
			drawComp->m_f_RgbaColor = {1,1,1,1};
		}

	},kComponentTint);
}