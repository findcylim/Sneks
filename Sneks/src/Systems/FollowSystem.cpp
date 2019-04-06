/* Start Header****************************************************************/
/*!
\file FollowSystem.h
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
#include "FollowSystem.h"
#include "../Components/FollowComponent.h"
#include "CollisionSystem.h"


void FollowSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	CollisionGroupName collGroup1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName collGroup2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupPairing colPairing ={ collGroup1, collGroup2 };

	if(collGroup1 == kCollGroupSnek1Head &&
		collGroup2 == kCollGroupSnek2Body)
	{ 
	  //Do something
	}
}

FollowSystem::FollowSystem()
{
}

FollowSystem::~FollowSystem()
{
}

void FollowSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}

void FollowSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_po_ComponentManager->Each<FollowComponent>([&](FollowComponent* comp)
	{

		FaceReference(comp->m_po_FolloweeTransform->GetComponent<TransformComponent>(), comp->GetComponent<TransformComponent>());
		MoveTowardsReference(comp);

	},kComponentFollow);
}

void FollowSystem::FaceReference(const TransformComponent* reference, TransformComponent* toChange)
{
	float referenceEdgeX = (reference->m_x_Position.x);// +refHead->GetRotatedOffsetXx());
	float referenceEdgeY = (reference->m_x_Position.y);// +refHead->GetRotatedOffsetXy());

	float distanceX = toChange->m_x_Position.x - referenceEdgeX;
	float distanceY = toChange->m_x_Position.y - referenceEdgeY;

	auto newRot = static_cast<float>(atan2(distanceY, distanceX));

	toChange->SetRotation(newRot);
}

void FollowSystem::MoveTowardsReference(FollowComponent* follow) const
{
	AEVec2 rotation;
	auto toChangeTrans = follow->GetComponent<TransformComponent>();
	auto refTrans      = follow->m_po_FolloweeTransform->GetComponent<TransformComponent>();

	AEVec2FromAngle(&rotation, toChangeTrans->GetRotation());

	auto stretchThreshold = follow->m_f_StretchThreshold; //Any faster than this speed the snek will start stretching
	auto scaleMultiplier = follow->m_f_ScaleFactorMultiplier;
	auto stretchFactor  = 100.0f / stretchThreshold;

	if (stretchFactor < 0.15f)
		stretchFactor = 0.15f;

	auto scaleFactor = (stretchFactor - 1.0f) * scaleMultiplier + 1.0f;

	if (follow->m_po_ParentEntity == follow->m_po_FolloweeTransform->m_po_OwnerEntity)
	{
		stretchFactor = 0.3f;
	}

	toChangeTrans->m_f_ScaleMultiplier.x = scaleFactor;

	toChangeTrans->m_x_Position.x = refTrans->m_x_Position.x +
		rotation.x * refTrans->m_f_Scale.x / 3 * stretchFactor;
	toChangeTrans->m_x_Position.y = refTrans->m_x_Position.y +
		rotation.y * refTrans->m_f_Scale.y / 3 * stretchFactor;
}



//
//void SnekSystem::MoveTowardsReference2(DrawComponent* reference, DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const
//{
//	float distanceX = toChange->m_po_TransformComponent->m_x_Position.x -
//		 reference->m_po_TransformComponent->m_x_Position.x;
//	float distanceY = toChange->m_po_TransformComponent->m_x_Position.y -
//		 reference->m_po_TransformComponent->m_x_Position.y;
//
//	auto headBodyAllowance = 0.83f;
//	auto headBodyClosenessMultiplier = 0.4f;
//	auto stretchFactor = headPhysicsComponent->m_f_Speed / 300.0f;
//	if (stretchFactor > 1.0f)
//		stretchFactor = 1.0f;
//
//	if (headPhysicsComponent->m_po_OwnerEntity == reference->m_po_OwnerEntity) {
//		toChange->m_po_TransformComponent->m_x_Position.x =
//			reference->m_po_TransformComponent->m_x_Position.x + distanceX
//			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
//
//		toChange->m_po_TransformComponent->m_x_Position.y =
//			reference->m_po_TransformComponent->m_x_Position.y + distanceY
//			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
//	}
//	else
//	{
//		headBodyAllowance = 0.95f;
//		headBodyClosenessMultiplier = 0.4f;// *reference->m_po_TransformComponent->m_f_Scale;
//		stretchFactor = headPhysicsComponent->m_f_Speed / 900.0f;
//
//		toChange->m_po_TransformComponent->m_x_Position.x =
//			reference->m_po_TransformComponent->m_x_Position.x + distanceX 
//			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
//
//		toChange->m_po_TransformComponent->m_x_Position.y =
//			reference->m_po_TransformComponent->m_x_Position.y + distanceY 
//			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
//	}
//}
