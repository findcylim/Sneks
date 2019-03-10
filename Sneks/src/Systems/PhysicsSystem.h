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
private:
	GameStateManager* m_o_GameStateManager;
public:
	~PhysicsSystem();
	PhysicsSystem(EntityManager* entityManagerPtr);

	void Initialize(GameStateManager* gameStateManager);
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