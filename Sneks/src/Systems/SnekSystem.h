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

struct SnekPreset
{
	float startPosX                        = 0;
	float startPosY                        = 0;
	float startRot                         = 0; //Radians
	const int startNumBodyParts            = 20;
	const char* textureName                = "SnekHead01";
	const char* bodyTextureName            = "SnekBody01";
	unsigned m_i_AccelerationKey           = AEVK_W;
	unsigned m_i_BrakeKey                  = AEVK_S;
	unsigned m_i_LeftKey                   = AEVK_A;
	unsigned m_i_RightKey                  = AEVK_D;

	float m_f_MaxVelocity                  = 900;
	float m_f_AccelerationForce            = 200;
	float m_f_BrakeForce                   = 6;
	float m_f_TurnSpeed                    = 6;
	float m_f_Friction                     = 1.5f;	   //natural slowdown
	float m_f_TurnMinSpeed                 = 60;       //need to be moving at this speed to turn
	float m_f_MinSpeed                     = 300;	   //if speed lower than this then clamp to 0
	float m_f_IdleSpeed                    = 100;		//default move speed
};


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