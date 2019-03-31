/* Start Header ***************************************************************/
/*!
\file PhysicsSystem.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "PhysicsSystem.h"
#include <iostream>
#include "SnekSystem.h"
#include "../Components/PowerUpComponent.h"

//TODO FIX THIS HARD CODE
static float currDt;

PhysicsSystem::PhysicsSystem()
{
}

PhysicsSystem::~PhysicsSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_MOVEMENT_KEY>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void PhysicsSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_MOVEMENT_KEY>(this,this);
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}
void PhysicsSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
}

void PhysicsSystem::Receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData)
{
	
	auto phyComp = eventData.caller;
	auto snekHeadComponent = m_po_ComponentManager->
		GetSpecificComponentInstance<SnekHeadComponent>(phyComp, kComponentSnekHead);

	if (eventData.key == Events::MOVE_KEY_UP) {
		phyComp->m_f_Acceleration = snekHeadComponent->m_f_AccelerationForce;
	}
	else if (eventData.key == Events::MOVE_KEY_DOWN)
	{
		phyComp->m_f_Acceleration = -snekHeadComponent->m_f_AccelerationForce / 3;
	}
	else if (eventData.key == Events::MOVE_KEY_LEFT) {

		auto turnSpeedMultiplier = phyComp->m_f_Speed / phyComp->m_f_MaxSpeed;

		if (turnSpeedMultiplier > 1.0f)
			turnSpeedMultiplier = 1.0f;
		else if (turnSpeedMultiplier < 0.5f)
			turnSpeedMultiplier = 0.5f;

		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() +
			snekHeadComponent->m_f_TurnSpeed * currDt *
			turnSpeedMultiplier
			);
	}
	else if (eventData.key == Events::MOVE_KEY_RIGHT) {
		auto turnSpeedMultiplier = phyComp->m_f_Speed / phyComp->m_f_MaxSpeed;

		if (turnSpeedMultiplier > 1.0f)
			turnSpeedMultiplier = 1.0f;
		else if (turnSpeedMultiplier < 0.5f)
			turnSpeedMultiplier = 0.5f;

		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() -
			snekHeadComponent->m_f_TurnSpeed * currDt *
			turnSpeedMultiplier
			);
	}

}


void PhysicsSystem::Update(float dt)
{
	currDt = dt;
	State currentState = GameStateManager::ReturnCurrentState();

	UNREFERENCED_PARAMETER(currentState);

	auto i_PhysicsComponent = m_po_ComponentManager
		->GetFirstComponentInstance<PhysicsComponent>(kComponentPhysics);

	while (i_PhysicsComponent)
	{
		//Calculate the stuff
		ApplyAcceleration(i_PhysicsComponent, dt);
		CalculateVelocity(i_PhysicsComponent);

		// Snek checks
		//for (auto component : i_PhysicsComponent->m_po_OwnerEntity->m_v_AttachedComponentsList) {
		//	//only if its a head
		if (auto component = i_PhysicsComponent->GetComponent<SnekHeadComponent>()){
				CheckOutOfBounds(i_PhysicsComponent->m_po_TransformComponent,i_PhysicsComponent);
				ClampVelocity(i_PhysicsComponent, static_cast<SnekHeadComponent*>(component));
		}
		//Move the object
		ApplyVelocity(i_PhysicsComponent, dt);
		
		if (i_PhysicsComponent->m_po_OwnerEntity->GetEntityID() == kEntityPowerUpHolder)
		//if (i_PhysicsComponent->GetComponent<PowerUpComponent>())
		{
			DeleteOutOfBound(i_PhysicsComponent->GetComponent<TransformComponent>());
		}

		i_PhysicsComponent = static_cast<PhysicsComponent*>(i_PhysicsComponent->m_po_NextComponent);
	}
}

HTVector2 PhysicsSystem::ApplyVelocity(PhysicsComponent* physicsComponent, float dt) const
{
	physicsComponent->m_po_TransformComponent->m_x_Position.x += physicsComponent->m_x_Velocity.x * dt;
	physicsComponent->m_po_TransformComponent->m_x_Position.y += physicsComponent->m_x_Velocity.y * dt;
	return physicsComponent->m_x_Velocity;
}

HTVector2 PhysicsSystem::CalculateVelocity(PhysicsComponent* physicsComponent) const
{
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, physicsComponent->m_po_TransformComponent->GetRotation());
	HTVector2 forwardVelocity ={ forwardVector.x * physicsComponent->m_f_Speed, forwardVector.y * physicsComponent->m_f_Speed };
	physicsComponent->m_x_Velocity = forwardVelocity;
	return forwardVelocity;
}

void PhysicsSystem::ClampVelocity(PhysicsComponent* physicsComponent, SnekHeadComponent* snekHeadComponent) const
{
	if (physicsComponent->m_f_Speed > physicsComponent->m_f_MaxSpeed)
	{
		physicsComponent->m_f_Speed = physicsComponent->m_f_Speed * 0.95f;
	}
	else
	{
		if (physicsComponent->m_f_Acceleration == 0)
			physicsComponent->m_f_Speed = physicsComponent->m_f_Speed * 0.99f;
	}

	//std::cout << "Accel: " << physicsComponent->m_f_Acceleration << ", " << physicsComponent->m_f_Speed << std::endl;
	if (physicsComponent->m_f_Acceleration == 0) 
	{
		if (physicsComponent->m_f_Speed < snekHeadComponent->m_f_IdleSpeed)
			physicsComponent->m_f_Speed = snekHeadComponent->m_f_IdleSpeed;		
		else if (physicsComponent->m_f_Speed < 0)
			physicsComponent->m_f_Speed += snekHeadComponent->m_f_Friction;
		else if (physicsComponent->m_f_Speed > 0)
			physicsComponent->m_f_Speed -= snekHeadComponent->m_f_Friction;
	}
	
}

void PhysicsSystem::ApplyAcceleration(PhysicsComponent* physicsComponent, float dt) const
{
	//Clamp percentage higher when speed is higher, so less acceleration when speed high
	float accelClamp = 1.0f - fabsf(physicsComponent->m_f_Speed / physicsComponent->m_f_MaxSpeed);
	
	if (accelClamp < 0)
		accelClamp = 0;
	else if (accelClamp > 1.0f)
		accelClamp = 1.0f;

	auto clampedAccel = physicsComponent->m_f_Acceleration * accelClamp;
	physicsComponent->m_f_Speed += clampedAccel * dt;
}

void PhysicsSystem::CheckOutOfBounds(TransformComponent* transformComponent, PhysicsComponent* physicsComponent) const
{
	
	UNREFERENCED_PARAMETER(physicsComponent);
	auto camera = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	auto viewDistHalf = 	camera->m_x_CurrentViewDistance / 2 * 0.95f;
	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.x > viewDistHalf.x - camera->m_f_VirtualOffset.x)
	{
		//physicsComponent->m_x_Velocity.x = 0;
		transformComponent->m_x_Position.x = viewDistHalf.x - camera->m_f_VirtualOffset.x;
	}
	else if (transformComponent->m_x_Position.x < -viewDistHalf.x - camera->m_f_VirtualOffset.x) {
		//physicsComponent->m_x_Velocity.x = 0;
		transformComponent->m_x_Position.x = -viewDistHalf.x - camera->m_f_VirtualOffset.x;
	}
	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.y > viewDistHalf.y - camera->m_f_VirtualOffset.y) {
		//physicsComponent->m_x_Velocity.y = 0;
		transformComponent->m_x_Position.y = viewDistHalf.y - camera->m_f_VirtualOffset.y;
	}
	else if (transformComponent->m_x_Position.y < -viewDistHalf.y - camera->m_f_VirtualOffset.y) {
		//physicsComponent->m_x_Velocity.y = 0;
		transformComponent->m_x_Position.y = -viewDistHalf.y - camera->m_f_VirtualOffset.y;
	}
}

void PhysicsSystem::DeleteOutOfBound(TransformComponent* transformComponent) const
{
	//if out of screen, delete object
	if ((transformComponent->m_x_Position.x > AEGfxGetWinMaxX() + 2 * 1920) ||
	(transformComponent->m_x_Position.x < AEGfxGetWinMinX() - 2 * 1920) ||
	(transformComponent->m_x_Position.y > AEGfxGetWinMaxY() + 2 * 1080) ||
	(transformComponent->m_x_Position.y < AEGfxGetWinMinY() - 2 * 1080))
	{
		m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(
			transformComponent->m_po_OwnerEntity));
	}
}


