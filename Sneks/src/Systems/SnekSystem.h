#ifndef SNEK_SYSTEM_H
#define SNEK_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
//#include "../ECS/ECSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "GraphicsSystem.h"
#include <vector>
#include "../Components/PhysicsComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/SnekHeadComponent.h"
//#include "ProjectileSystem.h"
#include "../Utility/GameStateManager.h"

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
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	GraphicsSystem* m_o_GraphicsSystem;
	GameStateManager* m_o_GameStateManager;

	float f_AngleHeadHit = PI / 4;

	int winner;
public:
	SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics, GameStateManager* gameStateManagerPtr);
	~SnekSystem() ;
	void CheckGrowthMeters();
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
	void SnekLoseLife(SnekHeadComponent* snekHead);
	void ResetStage();
	void HeadApplyRecoil(BaseComponent* aggressor, BaseComponent* victim);
	void HeadInvulnerableSet(float duration, BaseComponent* anyComponent);
	void HeadCollideBodyCheck(CollisionComponent* victimCollision, CollisionComponent* aggressorCollision);
	void Update(float dt) override;
	void BodyPartsFollowHead();
	void CheckInvulnerability(BaseComponent* component, float dt) const;
	void BodyInvulnerableSet(SnekHeadComponent* snekHead) const;
	void Initialize();
	void CreateSnek(float posX, float posY, float rotation, const int numBodyParts, const char* textureName, int controlScheme) const;
	void ResetSnek(SnekHeadEntity* owner);
	void CreateSnekBody(SnekHeadEntity* owner, const char* textureName, int playerNumber) const;
	void CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const;
	void DeleteSnek(SnekHeadEntity* snekHead);
	void RemoveBodyParts(int partsToRemove, SnekHeadComponent* snekHead);
	void CutSnekBody(SnekBodyEntity*, SnekHeadComponent* snekHead);
	void FaceReference(const TransformComponent* reference, TransformComponent* toChange) const;
	void MoveTowardsReference(::DrawComponent* reference, ::DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const;
	void MoveTowardsReference2(DrawComponent* reference, DrawComponent* toChange,
	                           PhysicsComponent* headPhysicsComponent) const;

	void CheckOutOfBounds(TransformComponent* transformComponent) const;
	void Flip(SnekHeadEntity* owner);
	void UpdateFollowComponents(SnekHeadComponent* snekHeadComponent);

	void TweakGrowthRate(SnekHeadComponent* snekHead, float change);
	void SetSnekType(int playerNumber, SnekType snekType);
	void ResetLivesAll();
	int GetLives(SnekHeadComponent* snekHead) const;
	int GetLives(int playerNum) const;
	void TweakPlayerDamage(SnekHeadComponent* snekHead, int change);
	void ResetDamageAll();
	int GetWinner();
	float GetGrowthPercentage(SnekHeadComponent* snekHead) const;
	float GetGrowthPercentage(int playerNum) const;
	float GetSpecialAttackPercentage(SnekHeadComponent* snekHead) const;
	float GetSpecialAttackPercentage(int playerNum) const;
};

//float GetFlipChargeRate();
#endif