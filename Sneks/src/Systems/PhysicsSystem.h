/* Start Header****************************************************************/
/*!
\file PhysicsSystem.h
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

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
#pragma once

#include "../ECS/System.h"
#include "../ECS/EventListener.h"
#include "../ECS/EventManager.h"
#include "../ECS/EntityManager.h"
#include "../Utility/GameStateManager.h"
#include "../Math/HTVector2.h"
#include "../Components/DrawComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/SnekHeadComponent.h"


class PhysicsSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>,
	public EventListener<Events::EV_PLAYER_MOVEMENT_KEY>
{
public:
	~PhysicsSystem();
	PhysicsSystem();

	void Initialize();
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
	void Receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData) override;
	void Update(float dt) override;
	HTVector2 ApplyVelocity(PhysicsComponent* physicsComponent, float dt) const;
	HTVector2 CalculateVelocity(PhysicsComponent* physicsComponent) const;
	void ClampVelocity(PhysicsComponent* physicsComponent, SnekHeadComponent* snekHeadComponent) const;
	void ApplyAcceleration(PhysicsComponent* physicsComponent, float dt) const;
	void CheckOutOfBounds(TransformComponent* transformComponent, PhysicsComponent* physicsComponent) const;
	void DeleteOutOfBound(TransformComponent* transformComponent) const;

};
#endif