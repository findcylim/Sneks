#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H
#pragma once

#include "../ECS/System.h"
#include "../ECS/EventListener.h"
#include "../ECS/EventManager.h"
#include "../Utility/GameStateManager.h"
#include "../Math/HTVector2.h"
#include "../Components/DrawComponent.h"
#include "../Components/PhysicsComponent.h"


class PhysicsSystem : public BaseSystem,
	public EventListener<Events::Ev_PLAYER1GAME_LEFTKEY>,
	public EventListener<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>,
	public EventListener<Events::Ev_PLAYER1GAME_RIGHTKEY>,
	public EventListener<Events::Ev_PLAYER2GAME_LEFTKEY>,
	public EventListener<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>,
	public EventListener<Events::Ev_PLAYER2GAME_RIGHTKEY>
{
private:
	GameStateManager* m_o_GameStateManager;
public:
	~PhysicsSystem();
	PhysicsSystem(EventManager* eventManager, GameStateManager* gameStateManager);

	void Initialize(EventManager* eventManager,GameStateManager* gameStateManager);

	virtual void receive(const Events::Ev_PLAYER1GAME_LEFTKEY& eventData) override;
	virtual void receive(const Events::Ev_PLAYER1GAME_LEFTSHIFTKEY& eventData) override;
	virtual void receive(const Events::Ev_PLAYER1GAME_RIGHTKEY& eventData) override;
	virtual void receive(const Events::Ev_PLAYER2GAME_LEFTKEY& eventData) override;
	virtual void receive(const Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY& eventData) override;
	virtual void receive(const Events::Ev_PLAYER2GAME_RIGHTKEY& eventData) override;
	void Update(float dt) override;
	HTVector2 ApplyVelocity(PhysicsComponent* physicsComponent, float dt);
	HTVector2 GetForwardVelocity(PhysicsComponent* physicsComponent) const;
};
#endif