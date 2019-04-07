/* Start Header****************************************************************/
/*!
\file CameraSystem.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that manipulates the in game virtual camera 
system, which draws all objects relative to a camera entity.

\par Contribution : CY     - 54.55% (Everything else)
\par Contribution : Edwin  - 45.45% (Culling)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "CameraSystem.h"
#include "../Components/CameraComponent.h"
#include "../Math/Aabb.h"
#include "../Utility/GameStateManager.h"

void CameraSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (GameStateManager::ReturnCurrentState() == kStateGame)
	{
		if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMoon && eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek2Body)
			SetShake(0.4f);
		else if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMoon)
		{
		}
		else if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupSnek1Head &&
			eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek2Body ||
			eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupSnek2Head &&
			eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek1Body ||
			eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupSnek1Head &&
			eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek2Head) {
			SetShake(13.0f);
		}
		else if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMouse && eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupUIButton)
		{
		}
		else
			SetShake(3.0f);
	}
}

CameraSystem::CameraSystem(GraphicsSystem* graphicsSystem)
{
	m_po_CamShake = new CameraShake();
	SetShake(0);
	m_po_GraphicsSystem = graphicsSystem;
}

CameraSystem::~CameraSystem()
{
	delete m_po_CamShake;
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void CameraSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);

}

void CameraSystem::UpdateCamera(const float dt) const
{
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	//float x = m_po_EntityManager->GetFirstEntityInstance<CameraEntity>(kEntityCamera)->GetComponent<TransformComponent>()->m_x_Position.x;
	//printf("%f\n", x);
	m_po_CamShake->Update(cameraComponent, dt);

	if (cameraComponent) 
	{
		cameraComponent->m_x_CurrentViewDistance.x = cameraComponent->m_px_ScreenSize.x / cameraComponent->m_f_VirtualScale;
		cameraComponent->m_x_CurrentViewDistance.y = cameraComponent->m_px_ScreenSize.y / cameraComponent->m_f_VirtualScale;

		float lowestDistanceFromScreenEdgeX = -cameraComponent->m_x_CurrentViewDistance.x;
		float lowestDistanceFromScreenEdgeY = -cameraComponent->m_x_CurrentViewDistance.y;
		if (cameraComponent->m_b_TrackObjects)
		{
			//ZOOM OUT CHECKS
			for (auto i_Object : cameraComponent->m_v_EntitiesToTrack)
			{
				float distFromScreenEdgeX = fabsf(i_Object->m_x_Position.x + cameraComponent->m_f_VirtualOffset.x)
					- cameraComponent->m_x_CurrentViewDistance.x / 2;
				float distFromScreenEdgeY = fabsf(i_Object->m_x_Position.y + cameraComponent->m_f_VirtualOffset.y)
					- cameraComponent->m_x_CurrentViewDistance.y / 2;

				if ((distFromScreenEdgeX > -cameraComponent->m_f_DistanceOutTolerance.x / 2 * cameraComponent->m_x_CurrentViewDistance.x))
				{
					cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed;
				}
				if (distFromScreenEdgeY > -cameraComponent->m_f_DistanceOutTolerance.y / 2 * cameraComponent->m_x_CurrentViewDistance.y)
				{
					cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
				}
				if (distFromScreenEdgeX > -cameraComponent->m_f_DistanceOutTolerance.x * cameraComponent->m_x_CurrentViewDistance.x)
				{
					cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed;
				}
				if (distFromScreenEdgeY > -cameraComponent->m_f_DistanceOutTolerance.y * cameraComponent->m_x_CurrentViewDistance.y)
				{
					cameraComponent->m_f_ZoomVelocity -= cameraComponent->m_x_CameraAttributes.zoomOutBaseSpeed * 1.3f;
				}

				if (cameraComponent->m_f_VirtualScale < 0.5f)
					cameraComponent->m_f_ZoomVelocity = 0;

				//Record the object nearest to the edge
				if (distFromScreenEdgeX > lowestDistanceFromScreenEdgeX &&
					distFromScreenEdgeY > lowestDistanceFromScreenEdgeY)
				{
					lowestDistanceFromScreenEdgeX = distFromScreenEdgeX;
					lowestDistanceFromScreenEdgeY = distFromScreenEdgeY;
				}
			}


			//ZOOM IN CHECKS
			if (cameraComponent->m_f_VirtualScale < 1.0f && cameraComponent->m_f_ZoomVelocity >= 0)
			{
				if (-lowestDistanceFromScreenEdgeX >= cameraComponent->m_f_DistanceInTolerance.x * cameraComponent->m_x_CurrentViewDistance.x &&
					-lowestDistanceFromScreenEdgeY >= cameraComponent->m_f_DistanceInTolerance.y * cameraComponent->m_x_CurrentViewDistance.y) {
					//cameraComponent->m_i_CurrentStage = 1;
					cameraComponent->m_f_ZoomVelocity += cameraComponent->m_x_CameraAttributes.zoomInBaseSpeed;
				}

			}
		}

		if (cameraComponent->m_f_ZoomVelocity > cameraComponent->m_x_CameraAttributes.zoomInMaxSpeed)
			cameraComponent->m_f_ZoomVelocity = cameraComponent->m_x_CameraAttributes.zoomInMaxSpeed;
		else if (cameraComponent->m_f_ZoomVelocity < -cameraComponent->m_x_CameraAttributes.zoomOutMaxSpeed)
			cameraComponent->m_f_ZoomVelocity = -cameraComponent->m_x_CameraAttributes.zoomOutMaxSpeed;
		else if (fabsf(cameraComponent->m_f_ZoomVelocity) < 0.0005f) {
			cameraComponent->m_f_ZoomVelocity = 0;
		}


		if (AEInputCheckReleased(AEVK_V)) {
			cameraComponent->m_b_TrackObjects = !cameraComponent->m_b_TrackObjects;
		}

		if (cameraComponent->m_b_TrackObjects)
		{
			HTVector2 averagePosition ={
		(cameraComponent->m_v_EntitiesToTrack.front()->m_x_Position.x + cameraComponent->m_v_EntitiesToTrack.back()->m_x_Position.x)
			/ 2,
		(cameraComponent->m_v_EntitiesToTrack.front()->m_x_Position.y + cameraComponent->m_v_EntitiesToTrack.back()->m_x_Position.y)
			/ 2 };

			cameraComponent->m_f_TargetOffset.x = -averagePosition.x;
			cameraComponent->m_f_TargetOffset.y = -averagePosition.y;
		}

		cameraComponent->m_f_VirtualOffset += (cameraComponent->m_f_TargetOffset - cameraComponent->m_f_VirtualOffset) 
														* cameraComponent->m_f_PanningSpeed * dt;
		
			

		cameraComponent->m_f_VirtualScale *= 1 + cameraComponent->m_f_ZoomVelocity * 1.4f * dt;

		cameraComponent->m_f_ZoomVelocity *= cameraComponent->m_x_CameraAttributes.speedDecay;

		HTVector2 offset{ 0,0 };

		if (cameraComponent->m_f_VirtualOffset.x < -1920)
		{
			offset.x -= 1920;
			cameraComponent->m_f_VirtualOffset.x += 1920;
		}
		else if (cameraComponent->m_f_VirtualOffset.x > 1920)
		{
			offset.x += 1920;
			cameraComponent->m_f_VirtualOffset.x -= 1920;
		}

		if (cameraComponent->m_f_VirtualOffset.y < -1080)
		{
			offset.y -= 1080;
			cameraComponent->m_f_VirtualOffset.y += 1080;
		}
		else if (cameraComponent->m_f_VirtualOffset.y > 1080)
		{
			offset.y += 1080;
			cameraComponent->m_f_VirtualOffset.y -= 1080;
		}

		//CULLING SYSTEM::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
		Aabb cameraAABB = { {-cameraComponent->m_f_VirtualOffset.x - cameraComponent->m_x_CurrentViewDistance.x,
			-cameraComponent->m_f_VirtualOffset.y - cameraComponent->m_x_CurrentViewDistance.y},
			{-cameraComponent->m_f_VirtualOffset.x + cameraComponent->m_x_CurrentViewDistance.x,
			-cameraComponent->m_f_VirtualOffset.y + cameraComponent->m_x_CurrentViewDistance.y} };

		auto transformComponent =
			m_po_ComponentManager->GetFirstComponentInstance<TransformComponent>(kComponentTransform);

		while(transformComponent)
		{
			transformComponent->m_x_Position += offset;

			if (transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityBackground ||
				transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityStaticObject)
			{
				if (transformComponent->m_x_Position.x < -1920 * 2.5f)
				{
					transformComponent->m_x_Position.x += 1920 * 5;

					if(transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityStaticObject)
						ReviveBuildingOnShift(transformComponent);
				}
				else if (transformComponent->m_x_Position.x > 1920 * 2.5f)
				{
					transformComponent->m_x_Position.x -= 1920 * 5;

					if (transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityStaticObject)
						ReviveBuildingOnShift(transformComponent);
				}

				if (transformComponent->m_x_Position.y < -1080 * 2.5f)
				{
					transformComponent->m_x_Position.y += 1080 * 5;

					if (transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityStaticObject)
						ReviveBuildingOnShift(transformComponent);
				}
				else if (transformComponent->m_x_Position.y > 1080 * 2.5f)
				{
					transformComponent->m_x_Position.y -= 1080 * 5;

					if (transformComponent->m_po_OwnerEntity->GetEntityID() == kEntityStaticObject)
						ReviveBuildingOnShift(transformComponent);
				}
			}

			Aabb otherAABB = { {transformComponent->m_x_Position.x - transformComponent->GetDrawScale().x / 2,
				transformComponent->m_x_Position.y - transformComponent->GetDrawScale().y / 2},
				{transformComponent->m_x_Position.x + transformComponent->GetDrawScale().x / 2,
				transformComponent->m_x_Position.y + transformComponent->GetDrawScale().y / 2} };
				
			auto collisionComponent = transformComponent->GetComponent<CollisionComponent>();

			auto drawComponent =transformComponent->GetComponent<DrawComponent>();

			if (AabbHelper::CheckAabbIntersect(cameraAABB, otherAABB))
			{
				if (collisionComponent)
				{
					if(collisionComponent->m_po_OwnerEntity->m_b_IsActive)
						collisionComponent->m_b_IsActive = true;
				}
				if (drawComponent)
				{
					if (drawComponent->m_po_OwnerEntity->m_b_IsActive)
						drawComponent->m_b_IsActive = true;
				}
			}
			else
			{
				if (collisionComponent)
				{
					if (collisionComponent->m_po_OwnerEntity->m_b_IsActive)
						collisionComponent->m_b_IsActive = false;
				}
				if (drawComponent)
				{
					if (drawComponent->m_po_OwnerEntity->m_b_IsActive)
						drawComponent->m_b_IsActive = false;
				}
			}
			transformComponent = static_cast<TransformComponent*>(transformComponent->m_po_NextComponent);
		}
	}
}

void CameraSystem::Update(float dt)
{
	UpdateCamera(dt);
}

float CameraSystem::AddShake(float magnitude)
{
	return m_po_CamShake->AddShake(magnitude);
}

void CameraSystem::SetShake(float magnitude)
{
	m_po_CamShake->SetShake(magnitude);
}

void CameraSystem::RemoveCameraTrackObjects()
{
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	if (cameraComponent)
	{
		cameraComponent->m_v_EntitiesToTrack.clear();
	}
}

void CameraSystem::ReviveBuildingOnShift(TransformComponent* transformComponent) const
{
	auto collisionComponent =
		m_po_ComponentManager->GetSpecificComponentInstance<CollisionComponent>(
			transformComponent, kComponentCollision
			);
	collisionComponent->enabled = true;
	collisionComponent->m_b_IsActive = true;

	const char * buildingName = "Building01";
	int check = (rand() % 100) / 5;

	if (check >= 0 && check <= 7)
	{
		buildingName = "Building01";
	}
	else if (check >= 8 && check <= 14)
	{
		buildingName = "Building02";
	}
	else if (check >= 15 && check <= 18)
	{
		buildingName = "Building03";
	}
	else
	{
		buildingName = "Building04";
	}

	auto objectDrawComp =
		m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
			collisionComponent, kComponentDraw
			);
	objectDrawComp->m_px_Texture = m_po_GraphicsSystem->FetchTexture(buildingName);
}




