#ifndef SNEK_SYSTEM_H
#define SNEK_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "GraphicsSystem.h"
#include <vector>
#include "../Components/PhysicsComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/SnekHeadComponent.h"
#include "ProjectileSystem.h"

class SnekSystem final : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>,
	public EventListener<Events::Ev_SNEK_INVULNERABLE>
{
private:
	GraphicsSystem* m_o_GraphicsSystem;
public:
	SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	~SnekSystem() ;
	void receive(const Events::EV_PLAYER_COLLISION& eventData);
	void receive(const Events::Ev_SNEK_INVULNERABLE& eventData);
	void Update(float dt) override;
	void CheckInvulnerability(BaseComponent* component, float dt) const;
	void BodyInvulnerableSet(SnekHeadComponent* snekHead) const;
	void Initialize();
	void CreateSnek(float posX, float posY, float rotation, const int numBodyParts, const char* textureName, int controlScheme) const;
	void CreateSnekBody(SnekHeadEntity* owner, const char* textureName, int playerNumber) const;
	void CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const;
	void RemoveSnekBody(SnekBodyEntity*, SnekHeadComponent* snekHead);
	void FaceReference(const TransformComponent* reference, TransformComponent* toChange) const;
	void MoveTowardsReference(::DrawComponent* reference, ::DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const;
	void CheckOutOfBounds(TransformComponent* transformComponent) const;
	void Flip(SnekHeadEntity* owner);
};
#endif