/* Start Header****************************************************************/
/*!
\file SnekSystem.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 30

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "SnekSystem.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CollisionComponent.h"
#include "../Components/FollowComponent.h"
#include "../Components/PowerUpComponent.h"
#include "../Systems/Menus/WinScreenSystem.h"
#include "../ECS/SystemManager.h"
#include <iostream>
#include <algorithm>

//"Forward" declaration includes
#include "PhysicsSystem.h"
#include "../Components/BloomComponent.h"
#include "BuildingsSystem.h"
#include "CollisionSystem.h"
#include "../Components/PowerUpHolderComponent.h"
#include "FollowSystem.h"
#include "CameraSystem.h"
#include "../Components/ParticleSpawnerComponent.h"
#include "ParticleSystem.h"
#include "PowerUpSystem.h"

int SnekSystem::GetWinner()
{
	return winner;
}

void SnekSystem::ResetLivesAll()
{
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		snekHead->m_i_LivesLeft = 3;
	}, kComponentSnekHead);
}

void SnekSystem::ResetDamageAll()
{
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		snekHead->m_i_CurrentDamage = snekHead->m_i_BaseDamage;
	}, kComponentSnekHead);
}

int SnekSystem::GetLives(SnekHeadComponent* snekHead) const
{
	return snekHead->m_i_LivesLeft;
}

int SnekSystem::GetLives(int playerNum) const
{
	int toReturn = 0;
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		if (snekHead->m_i_PlayerNumber == playerNum)
		{
			toReturn = GetLives(snekHead);
		}
	}, kComponentSnekHead);
	return toReturn;
}

void SnekSystem::TweakPlayerDamageBySpeed(SnekHeadComponent* snekHead)
{
	auto physicsComp = snekHead->GetComponent<PhysicsComponent>();

	auto speedMulti = static_cast<float>(physicsComp->m_f_Speed / 400.0f);

	snekHead->m_i_CurrentDamage = static_cast<int>(snekHead->m_i_BaseDamage +
																  snekHead->m_i_BaseDamage * 2 * speedMulti);
	if (snekHead->m_i_CurrentDamage < 2)
		snekHead->m_i_CurrentDamage = 2;
	else if (snekHead->m_i_CurrentDamage >= 10)
		snekHead->m_i_CurrentDamage = 10;
}

void SnekSystem::TweakGrowthRate(SnekHeadComponent* snekHead, float change)
{
	snekHead->m_f_CurrentGrowth *= change;
	CheckGrowthMeters();
}

float SnekSystem::GetGrowthPercentage(SnekHeadComponent* snekHead) const
{
	return snekHead->m_f_CurrentGrowth / snekHead->m_f_GrowthMeter;
}

float SnekSystem::GetGrowthPercentage(int playerNum) const
{
	float toReturn = 0;
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		if (snekHead->m_i_PlayerNumber == playerNum)
		{
			toReturn = GetGrowthPercentage(snekHead);
		}
	}, kComponentSnekHead);
	return toReturn;
}

void SnekSystem::CheckGrowthMeters()
{
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		
		if (snekHead->m_f_CurrentGrowth >= snekHead->m_f_GrowthMeter)
		{
			const char* bodyTexture = nullptr;
			switch (snekHead->m_x_SnekType)
			{
			case kSnekTypeFlip:
				bodyTexture = "SnekBody03";
				break;
			case kSnekTypeShoot:
				bodyTexture = "SnekBody02";
				break;
			case kSnekTypeSpeed:
				bodyTexture = "SnekBody01";
				break;

			}
			snekHead->m_f_CurrentGrowth = 0;
			//Make it harder for the snake to grow
			snekHead->m_f_GrowthMeter *= 1.1f;
			//Spawn new body part
			CreateSnekBody(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity),
				bodyTexture, snekHead->m_i_PlayerNumber);
		}
	}, kComponentSnekHead);
}

float SnekSystem::GetSpecialAttackPercentage(SnekHeadComponent* snekHead) const
{
	return snekHead->m_f_BoostCooldown / snekHead->m_f_BoostSetCooldown;
}

float SnekSystem::GetSpecialAttackPercentage(int playerNum) const
{
	float toReturn = 0;
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		if (snekHead->m_i_PlayerNumber == playerNum)
		{
			toReturn = GetSpecialAttackPercentage(snekHead);
		}
	}, kComponentSnekHead);
	return toReturn;
}

SnekSystem::SnekSystem( GraphicsSystem* graphics)
{
	m_o_GraphicsSystem = graphics;
}

SnekSystem::~SnekSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
};


void SnekSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	CollisionGroupName collGroup1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName collGroup2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupPairing colPairing ={ collGroup1, collGroup2 };

	if (colPairing == CollisionGroupPairing{ kCollGroupMoon, kCollGroupSnek2Body } ||
		 colPairing == CollisionGroupPairing{ kCollGroupMoon, kCollGroupSnek1Body })
	{
		auto victimHeadComponent = eventData.object2->m_po_OwnerEntity->
			GetComponent<FollowComponent>()->m_po_ParentEntity->GetComponent<SnekHeadComponent>();
		/*auto attackerHeadComponent = eventData.object1->m_po_OwnerEntity->
			GetComponent<FollowComponent>()->m_po_ParentEntity->GetComponent<SnekHeadComponent>();
		*/
		if (victimHeadComponent->m_po_OwnerEntity->m_b_IsActive)
		{
			RemoveBodyParts(2, victimHeadComponent);
			//RemoveBodyParts(attackerHeadComponent->m_i_CurrentDamage / 2, victimHeadComponent);
		}
		return;
	}

	if (colPairing == CollisionGroupPairing{ kCollGroupSnek2Head, kCollGroupSnek1Body } ||
		 colPairing == CollisionGroupPairing{ kCollGroupSnek1Head, kCollGroupSnek2Body })
	{
		HeadCollideBody(eventData.object2, eventData.object1);
		return;
	}

	if (colPairing.groupA == kCollGroupBuilding ||
		 colPairing.groupB == kCollGroupBuilding)
	{
		
		auto objectColliding = eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupBuilding ?
			eventData.object1 : eventData.object2;
		auto otherObjectCollide = eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupBuilding ?
			eventData.object1 : eventData.object2;

		if (auto snekHeadComp = 
				otherObjectCollide->m_po_OwnerEntity->GetComponent<SnekHeadComponent>())
		{
			if (snekHeadComp->m_po_OwnerEntity->m_b_IsActive)
			{
				snekHeadComp->m_f_CurrentGrowth += 0.5f;
				CheckGrowthMeters();
			}
		}
		objectColliding->enabled = false;
		objectColliding->m_b_IsActive = false;
		auto objectDrawComp = 
			m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
				objectColliding, kComponentDraw
			);
		objectDrawComp->m_px_Texture = m_o_GraphicsSystem->FetchTexture("Destroyed01");
	}

	if (colPairing == CollisionGroupPairing{ kCollGroupSnek1Head, kCollGroupSnek2Head }) 
	{
		auto snekHed1 = eventData.object1->m_po_OwnerEntity->GetComponent<SnekHeadComponent>();
		auto snekHed2 = eventData.object2->m_po_OwnerEntity->GetComponent<SnekHeadComponent>();
	
		if (snekHed1->m_po_OwnerEntity->m_b_IsActive && snekHed2->m_po_OwnerEntity->m_b_IsActive)
		{
			bool snekLostLife = false;
			//Perform removal of body parts for body snek heds and check for win condition
			for (auto i_SnekHed = snekHed1;; i_SnekHed = snekHed2)
			{
				if (i_SnekHed->m_x_BodyParts.size() > 1)
				{
					RemoveBodyParts(i_SnekHed->m_i_CurrentDamage, i_SnekHed);
				}
				else
				{
					SnekLoseLife(i_SnekHed);
					snekLostLife = true;
				}

				if (i_SnekHed == snekHed2)
					break;
			}
			if (snekLostLife)
			{
				ResetStage();
				return;
			}

			//Send both heads flying in random directions, opposite of each other
			srand(clock());
			auto randDirection = AERandFloat() * 2 * PI;
			snekHed1->m_po_OwnerEntity->GetComponent<TransformComponent>()->SetRotation(randDirection);
			snekHed2->m_po_OwnerEntity->GetComponent<TransformComponent>()->SetRotation(randDirection + PI);

			//Set both speeds to the higher of the two
			float* snek1Speed = &snekHed1->m_po_OwnerEntity->GetComponent<PhysicsComponent>()->m_f_Speed;
			float* snek2Speed = &snekHed2->m_po_OwnerEntity->GetComponent<PhysicsComponent>()->m_f_Speed;
			float newSpeed = max(*snek1Speed, *snek2Speed);
			*snek1Speed = newSpeed;
			*snek2Speed = newSpeed;

			HeadInvulnerableSet(1.0f, snekHed1);
			HeadInvulnerableSet(1.0f, snekHed2);
		}
	}
}

void SnekSystem::SnekLoseLife(SnekHeadComponent* snekHead)
{
	if (--snekHead->m_i_LivesLeft <= 0)
	{
		//Winning should emit event instead of changing a variable
		winner = snekHead->m_i_PlayerNumber;
		GameStateManager::SetState(kStateWinScreen);
	}
}

void SnekSystem::ResetStage()
{


	//m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	//{
	// ResetSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
	//}, kComponentSnekHead);
	
	//ResetDamageAll();
	
	int lives[2],Counter=0;
	SnekType snekType[2];

	auto camera = m_po_SystemManager->GetSystem<CameraSystem>("Camera");
	camera->RemoveCameraTrackObjects();

	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		lives[Counter] = snekHead->m_i_LivesLeft;
		snekType[Counter] = snekHead->m_x_SnekType;
		++Counter;
		DeleteSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
		//ResetSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
	}, kComponentSnekHead);

	//Remove all power up
	m_po_ComponentManager->Each<PowerUpHolderComponent>([&](PowerUpHolderComponent* comp)
	{
		m_po_EntityManager->AddToDeleteQueue(comp->m_po_OwnerEntity);
	}, kComponentPowerUpHolder);

	////Remove all particles
	//m_po_ComponentManager->Each<ParticleComponent>([&](ParticleComponent* comp)
	//{
	//	m_po_EntityManager->AddToDeleteQueue(comp->m_po_OwnerEntity);
	//}, kComponentPowerUpHolder);

	//Regenerate buildings
	m_po_SystemManager->GetSystem<BuildingsSystem>()->ResetLevel1();

	CreateSnek(-200, 0, PI * 3 / 4, 20, snekType[0], 0, lives[0]);
	CreateSnek(200, 0, PI * 7 / 4, 20, snekType[1], 1, lives[1]);

	//TODO MOVE TO EVENTS
	m_po_SystemManager->GetSystem<ParticleSystem>()->ResetTrails();

}


void SnekSystem::HeadApplyRecoil(BaseComponent* aggressor, BaseComponent* victim)
{
	auto aggPhysics = aggressor->m_po_OwnerEntity->GetComponent<PhysicsComponent>();

	auto victimPhysics = victim->m_po_OwnerEntity->GetComponent<PhysicsComponent>();

	auto newVel = CalculateReflectVelocity(aggPhysics->m_x_Velocity, GetNormal(victimPhysics->m_x_Velocity)) ;
	aggPhysics->SetVelocity(newVel);

	if (aggPhysics->m_f_Speed > aggPhysics->m_f_MaxSpeed) {
		aggPhysics->m_f_Speed = aggPhysics->m_f_MaxSpeed;
	}
}

void SnekSystem::HeadInvulnerableSet(float duration, BaseComponent* anyComponent)
{
	auto snakeHeadInvulComponent = 
		m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
			anyComponent, KComponentInvulnerable
		);

	snakeHeadInvulComponent->m_f_InvulnerableTime = duration;

	BodyInvulnerableSet(snakeHeadInvulComponent->m_po_OwnerEntity->GetComponent<SnekHeadComponent>());
}

void SnekSystem::HeadCollideBody(CollisionComponent* victimCollision, CollisionComponent* aggressorCollision)
{
	auto snekHeadAggressor = aggressorCollision->GetComponent<SnekHeadComponent>();

	//Get the parent
	auto objectFollowComp = victimCollision->GetComponent<FollowComponent>();

	auto snekHeadVictim = objectFollowComp->m_po_ParentEntity->GetComponent<SnekHeadComponent>();

	auto physicsAggressor = aggressorCollision->GetComponent<PhysicsComponent>();

	//Head recoils away
	HeadApplyRecoil(snekHeadAggressor, snekHeadVictim);

	auto powerUpComp = snekHeadVictim->GetComponent<PowerUpComponent>();
	//if victim under star power
	if (powerUpComp->m_x_PowerUpType == kPowerUpStar &&
		 powerUpComp->m_f_PowerUpDurationLeft >= 0) 
	{
		RemoveBodyParts(snekHeadAggressor->m_i_CurrentDamage, snekHeadAggressor);
		HeadInvulnerableSet(3.0f, snekHeadAggressor);
	}
	else
	{
		auto victimPhysics = snekHeadVictim->GetComponent<PhysicsComponent>();
		RemoveBodyParts(snekHeadAggressor->m_i_CurrentDamage, snekHeadVictim);
		HeadInvulnerableSet(3.0f, snekHeadVictim);
		//Victim head receives some force
		victimPhysics->SetVelocity(victimPhysics->m_x_Velocity + HTVector2{ physicsAggressor->m_x_Velocity } *0.5f);
	}
}	


void SnekSystem::Update(float dt)
{
	auto i_InvulnerableComponent = 
		m_po_ComponentManager->GetFirstComponentInstance<InvulnerableComponent>(KComponentInvulnerable);

	while (i_InvulnerableComponent)
	{
		CheckInvulnerability(i_InvulnerableComponent, dt);
		i_InvulnerableComponent = static_cast<InvulnerableComponent*>(
			i_InvulnerableComponent->m_po_NextComponent
			);
	}

	auto i_SnekHead = 
		m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);

	while (i_SnekHead) {
		auto headTransComponent = i_SnekHead->m_po_OwnerEntity->
			GetComponent<TransformComponent>();

		auto headPhysicsComponent = i_SnekHead->m_po_OwnerEntity->
								GetComponent<PhysicsComponent>();

		if (i_SnekHead->m_f_BoostCooldown < i_SnekHead->m_f_BoostSetCooldown)
		{
			i_SnekHead->m_f_BoostCooldown += dt;
		}
		else if (i_SnekHead->m_x_SnekType == kSnekTypeSpeed &&
				   i_SnekHead->m_f_AccelerationForce > 300
			)
		{
			i_SnekHead->m_f_AccelerationForce /= 5.0f;
			i_SnekHead->GetComponent<PhysicsComponent>()->m_f_MaxSpeed /= 1.5f;
			//i_SnekHead->GetComponent<BloomComponent>()->m_f_BloomStrength /= 1.4f;
			//for (auto bodyPart : i_SnekHead->m_x_BodyParts)
			//	bodyPart->GetComponent<BloomComponent>()->m_f_BloomStrength /= 1.4f;
			//i_SnekHead->m_f_BoostCooldown = 0;
		}

		//if (GetAsyncKeyState(i_SnekHead->m_i_AccelerationKey)) 
		{
		//Always on acceleration
		Events::EV_PLAYER_MOVEMENT_KEY moveKey{ headPhysicsComponent, Events::MOVE_KEY_UP};
		m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKey);
		}
		//else
		//{
		//	headPhysicsComponent->m_f_Acceleration = 0;
		//}

		if (AEInputCheckTriggered(static_cast<u8>(i_SnekHead->m_i_BoostKey)) &&
			 i_SnekHead->m_f_BoostCooldown >= i_SnekHead->m_f_BoostSetCooldown)
		{
			i_SnekHead->specialTutorial = false;
			if (i_SnekHead->m_x_SnekType == kSnekTypeFlip)
			{
				m_po_EventManagerPtr->EmitEvent<Events::EV_SPECIAL_SKILL_FLIP>(Events::EV_SPECIAL_SKILL_FLIP{});
				Flip(static_cast<SnekHeadEntity*>(headTransComponent->m_po_OwnerEntity));
				i_SnekHead->m_f_BoostSetCooldown = 2;
				i_SnekHead->m_f_BoostCooldown = 0;
			}
			else if (i_SnekHead->m_x_SnekType == kSnekTypeShoot)
			{
				Events::EV_CREATE_PROJECTILE projData;

				projData.pos = &headTransComponent->m_x_Position;

				projData.velocity = &headPhysicsComponent->m_x_Velocity;

				projData.rot = headTransComponent->GetRotation();
				projData.speed = 1400.0f;
				projData.scale = 0.1f;
				projData.isCollide = true;

				projData.texName = "Moon";

				i_SnekHead->m_f_BoostCooldown = 0;

				m_po_EventManagerPtr->EmitEvent<Events::EV_CREATE_PROJECTILE>(projData);
			}
			else if (i_SnekHead->m_x_SnekType == kSnekTypeSpeed)
			{
				m_po_EventManagerPtr->EmitEvent<Events::EV_SPECIAL_SKILL_BOOST>(Events::EV_SPECIAL_SKILL_BOOST{});
				i_SnekHead->m_f_AccelerationForce *= 5.0f;
				i_SnekHead->GetComponent<PhysicsComponent>()->m_f_MaxSpeed *= 1.5f;
				//i_SnekHead->GetComponent<BloomComponent>()->m_f_BloomStrength *= 1.4f;
				//for (auto bodyPart : i_SnekHead->m_x_BodyParts)
				//	bodyPart->GetComponent<BloomComponent>()->m_f_BloomStrength *= 1.4f;
				i_SnekHead->m_f_BoostCooldown = 0;
			}
		}
		if (GetAsyncKeyState(i_SnekHead->m_i_LeftKey))
		{
			Events::EV_PLAYER_MOVEMENT_KEY moveKeyL{ headPhysicsComponent, Events::MOVE_KEY_LEFT };
			m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKeyL);
		}
		else if (GetAsyncKeyState(i_SnekHead->m_i_RightKey))
		{
			Events::EV_PLAYER_MOVEMENT_KEY moveKeyR{ headPhysicsComponent,Events::MOVE_KEY_RIGHT };
			m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKeyR);
		}


		for (auto& i_Body : i_SnekHead->m_x_BodyParts)
		{
			auto bodyDraw = 
				m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
					i_Body, kComponentDraw
				);

			auto followComponent = 
				m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
					i_Body, kComponentFollow
				);

			//auto followDrawComponent = 
			//	m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
			//		followComponent->m_po_FolloweeTransform->m_po_OwnerEntity, kComponentDraw
			//	);
			
			FollowSystem::FaceReference(followComponent->m_po_FolloweeTransform, bodyDraw->m_po_TransformComponent);
			//if (i_SnekHead->m_i_PlayerNumber)
			//else
			//	MoveTowardsReference2(followDrawComponent, bodyDraw, headPhysicsComponent);
		}

		TweakPlayerDamageBySpeed(i_SnekHead);
		i_SnekHead = static_cast<SnekHeadComponent*>(i_SnekHead->m_po_NextComponent);
	}
}

void SnekSystem::CheckInvulnerability(BaseComponent* component, float dt) const
{
	auto invulComponent = component->GetComponent<InvulnerableComponent>();

	auto drawComponent = component->GetComponent<DrawComponent>();

	auto collisionComponent = component->GetComponent<CollisionComponent>();

	if (invulComponent->m_f_InvulnerableTime > 0)
	{
		float blinkSpeedModifier = 1.0f / invulComponent->m_f_InvulnerableTime;
		float blinkSpeedModifierClamped = AEClamp(blinkSpeedModifier, 0.2f, 1.0f);

		//if (!invulComponent->m_b_IsAlive)
		//{
		//	drawComponent->m_f_RgbaColor.red += 1.0f;
		//	drawComponent->m_f_RgbaColor.blue -= 0.3f;
		//	drawComponent->m_f_RgbaColor.green -= 0.3f;
		//	invulComponent->m_b_IsAlive = true;
		//}

		drawComponent->m_f_RgbaColor.alpha -= blinkSpeedModifierClamped * invulComponent->m_f_BlinkSpeed * dt;

		if (drawComponent->GetAlpha() <= invulComponent->m_f_MinAlphaBlinking)
			drawComponent->SetAlpha(invulComponent->m_f_MaxAlphaBlinking);

		collisionComponent->enabled = false;
		invulComponent->m_f_InvulnerableTime -= dt;
	}
	else 
	{
		collisionComponent->enabled = true;
		drawComponent->SetAlpha(1.0f);

		//if (->m_b_IsAlive)
		//{
		//	//drawComponent->m_f_RgbaColor.red -= 1.0f;
		//	//drawComponent->m_f_RgbaColor.blue += 0.3f;
		//	//drawComponent->m_f_RgbaColor.green += 0.3f;
		//	//invulComponent->m_b_IsAlive = false;
		//}
	}
}

void SnekSystem::BodyInvulnerableSet(SnekHeadComponent* snekHead) const
{
	auto headInvulComponent = 
		m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
		snekHead, KComponentInvulnerable
		);

	for (auto i_BodyParts : snekHead->m_x_BodyParts)
	{
		auto invulComponent = 
			m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
				i_BodyParts, KComponentInvulnerable
			);
		invulComponent->m_f_InvulnerableTime = headInvulComponent->m_f_InvulnerableTime;
	}
}

void SnekSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}

//HEAD SIZE : 105, 77
//BODY SIZE:  61,  80
//SCALE : 0.635f
void SnekSystem::CreateSnek(float posX, float posY, float rotation,
	const int numBodyParts, SnekType snekType, int controlScheme,int lives) const
{
	//Count the number of previous snek heads
	int snekHeadCount = 0;
	auto i_SnekHeadComponents = 
		m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(
			kComponentSnekHead
		);

	auto tailTexture = "SnekTail01";
	auto bodyTexture = "SnekBody01";
	auto headTexture = "SnekHead01";
	auto spriteCountX = 2;
	auto spriteCountY = 1;

	switch (snekType)
	{
	case kSnekTypeFlip:
		spriteCountX = 1;
		spriteCountY = 1;
		headTexture = "SnekHead03";
		bodyTexture = "SnekBody03";
		tailTexture = "SnekTail03";
		break;
	case kSnekTypeSpeed:
		spriteCountX = 2;
		spriteCountY = 1;
		headTexture = "HeadAnim";
		bodyTexture = "SnekBody01";
		tailTexture = "SnekTail01";
		break;
	case kSnekTypeShoot:
		spriteCountX = 1;
		spriteCountY = 1;
		headTexture = "SnekHead02";
		bodyTexture = "SnekBody02";
		tailTexture = "SnekTail02";
		break;
	}

	while (i_SnekHeadComponents)
	{
		snekHeadCount++;
		i_SnekHeadComponents = static_cast<SnekHeadComponent*>(i_SnekHeadComponents->m_po_NextComponent);
	}

	auto newSnekHeadEntity = 
		m_po_EntityManager->NewEntity<SnekHeadEntity>(kEntitySnekHead, "Head");


	for (auto i_Component : newSnekHeadEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->m_x_Position.x=(posX);
			static_cast<TransformComponent*>(i_Component)->m_x_Position.y = (posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(rotation);

			auto cameraComponent =
				m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(
					kComponentCamera
					);
			cameraComponent->AddToTrack(static_cast<TransformComponent*>(i_Component));
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), headTexture,
				HTColor{ 1,1,1,1 }, spriteCountX, spriteCountY);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 4;
		}
		else if (i_Component->m_x_ComponentID == kComponentAnimation)
		{
			auto animComp = static_cast<AnimationComponent*>(i_Component);
			Animation anim(SpriteSheet{ headTexture, spriteCountX,spriteCountY }, 0, spriteCountX * spriteCountY);
			anim.m_f_SecondsPerFrame = 1.0f;
			animComp->m_vx_AnimationsList.push_back(anim);

			animComp->m_b_IsAnimating = true;
			animComp->m_i_CurrentAnimationId = 0;
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 400;
		}
		else if (i_Component->m_x_ComponentID == kComponentSnekHead)
		{
			static_cast<SnekHeadComponent*>(i_Component)->m_i_PlayerNumber = static_cast<unsigned short>(controlScheme);
			static_cast<SnekHeadComponent*>(i_Component)->m_i_LivesLeft = lives;
			static_cast<SnekHeadComponent*>(i_Component)->m_x_SnekType = snekType;
			if (controlScheme)
			{

			}
			else
			{//TODO:: REMOVE HARDCODE
				static_cast<SnekHeadComponent*>(i_Component)->m_i_AccelerationKey = AEVK_W;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_BrakeKey = AEVK_S;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_LeftKey = AEVK_A;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_RightKey = AEVK_D;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_BoostKey = AEVK_LSHIFT;
				//static_cast<SnekHeadComponent*>(i_Component)->m_i_SpecialKey = AEVK_Q;
			}
			//TODO :: LOTS OF SHIT
			//((SnekHeadComponent*)i_Component)->
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(static_cast<CollisionGroupName>(controlScheme * 2));
		}
		else if (i_Component->m_x_ComponentID == kComponentBloom)
		{
			
		}
	}

	CreateSnekTail(newSnekHeadEntity, tailTexture);

	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++)
	{
		CreateSnekBody(newSnekHeadEntity, bodyTexture, controlScheme);
	}
}

void SnekSystem::ResetSnek(SnekHeadEntity* owner)
{
	auto snekHeadComp = owner->GetComponent<SnekHeadComponent>();
	auto playerNumber = snekHeadComp->m_i_PlayerNumber;
	auto transformComp = owner->GetComponent<TransformComponent>();
	HTVector2 velocity;
	velocity.x = 0;
	velocity.y = 0;

	auto powerUpComp = owner->GetComponent<PowerUpComponent>();
	powerUpComp->ResetPowerIncrease();
	powerUpComp->m_f_PowerUpDurationLeft = 0;

	transformComp->m_x_Position.y = (0);

	if (playerNumber == 0)
	{
		transformComp->SetRotation(PI * 3 / 4);
		transformComp->m_x_Position.x = (-200);
		owner->GetComponent<PhysicsComponent>()->SetVelocity(velocity);
	}
	else
	{
		transformComp->SetRotation(PI * 7 / 4);
		transformComp->m_x_Position.x = (200);
		owner->GetComponent<PhysicsComponent>()->SetVelocity(velocity);
	}
	int partsToSpawn = 20 - static_cast<int>(snekHeadComp->m_x_BodyParts.size()) + 1;
	for (int i_BodyParts = 0; i_BodyParts < partsToSpawn; i_BodyParts++)
	{
		if (playerNumber == 0)
			CreateSnekBody(static_cast<SnekHeadEntity*>(owner), "SnekBody01", playerNumber);
		else
			CreateSnekBody(static_cast<SnekHeadEntity*>(owner), "SnekBody02", playerNumber);
	}

	for (int i = 0 ; i < 120; ++i)
	{
		for (auto& i_Body : snekHeadComp->m_x_BodyParts)
		{
			auto bodyDraw = i_Body->GetComponent<DrawComponent>();
			auto followComponent = i_Body->GetComponent<FollowComponent>();

			FollowSystem::FaceReference(followComponent->m_po_FolloweeTransform, bodyDraw->m_po_TransformComponent);
		}
	}
	snekHeadComp->GetComponent<DrawComponent>()->m_f_RgbaColor = { 1.0f,1.0f,1.0f,1.0f };
	for (auto& i_BodyParts : snekHeadComp->m_x_BodyParts)
	{
		auto bodyDraw = i_BodyParts->GetComponent<DrawComponent>();
		bodyDraw->m_f_RgbaColor = { 1.0f,1.0f,1.0f,1.0f };
	}
}

void SnekSystem::DeleteSnek(SnekHeadEntity* snekHead)
{
	for (auto snekBody : snekHead->GetComponent<SnekHeadComponent>()->m_x_BodyParts) 
	{
		m_po_EntityManager->AddToDeleteQueue(snekBody);
		//RemoveSnekBody(snekBody, snekHead->GetComponent<SnekHeadComponent>());
	}
	m_po_EntityManager->AddToDeleteQueue(snekHead);
}

void SnekSystem::RemoveBodyParts(int partsToRemove, SnekHeadComponent* snekHead)
{
	int numBodyParts = static_cast<int>(snekHead->m_x_BodyParts.size()) - 1;

	if (numBodyParts <= 0)
		return;
	//Cant remove what you dont have
	partsToRemove = min(partsToRemove, numBodyParts);
	SnekBodyEntity* toCut = snekHead->m_x_BodyParts[numBodyParts - partsToRemove];
	CutSnekBody(toCut, snekHead);
}

void SnekSystem::CutSnekBody(SnekBodyEntity* snekBody, SnekHeadComponent* snekHead)
{
	if (snekHead->m_x_BodyParts.size() <= 1)
		return;
	std::vector<SnekBodyEntity*>::iterator toDelete;
	bool found = false;
	for (auto i_BodyParts = snekHead->m_x_BodyParts.begin();
		i_BodyParts != snekHead->m_x_BodyParts.end() - 1; ++i_BodyParts)
	{
		if (snekBody == *i_BodyParts)
		{
			found = true;
			toDelete = i_BodyParts;
		}
		if (found)
		{
			m_po_EntityManager->AddToDeleteQueue(*i_BodyParts);
		}
	}
	if (found)
		snekHead->m_x_BodyParts.erase(toDelete, snekHead->m_x_BodyParts.end() - 1);

	m_po_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());

		
	auto tailFollowComponent = m_po_ComponentManager->GetSpecificComponentInstance
		<FollowComponent>(snekHead->m_x_BodyParts.back(), kComponentFollow);

	if (snekHead->m_x_BodyParts.size() <= 1)
	{
		auto snekHeadTransform = m_po_ComponentManager->GetSpecificComponentInstance
		<TransformComponent>(snekHead, kComponentTransform);

		tailFollowComponent->m_po_FolloweeTransform = snekHeadTransform;
	}
	else {
		auto lastBodyTransformComponent = m_po_ComponentManager->GetSpecificComponentInstance
		<TransformComponent>(snekHead->m_x_BodyParts.at(snekHead->m_x_BodyParts.size() - 2), kComponentTransform);

		tailFollowComponent->m_po_FolloweeTransform = lastBodyTransformComponent;
	}
}

void SnekSystem::CreateSnekBody(SnekHeadEntity* owner, const char* textureName, int playerNumber) const 
{
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = 
		m_po_EntityManager->NewEntity<SnekBodyEntity>(kEntitySnekBody, "Body");

	auto ownerTransform = owner->GetComponent<TransformComponent>();

	auto ownerHeadComponent = owner->GetComponent<SnekHeadComponent>();

	//Set the follow to head if there are no body parts, otherwise follow the last body part.
	auto referenceTransform = ownerHeadComponent->m_x_BodyParts.size() <= 1 ?
		ownerTransform : (*(ownerHeadComponent->m_x_BodyParts.end() - 2))->GetComponent<TransformComponent>();

	auto tailTransform = ownerHeadComponent->m_x_BodyParts.back()->GetComponent<TransformComponent>();

	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{

		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->m_x_Position = tailTransform->m_x_Position;

			//Spawn a new area behind the tail
			AEVec2 angle;
			AEVec2FromAngle(&angle, tailTransform->GetRotation() );

			if (tailTransform->GetComponent<SnekHeadComponent>())
			{
				AEVec2FromAngle(&angle, tailTransform->GetRotation() + PI);
			}
			auto referenceDraw = tailTransform->GetComponent<DrawComponent>();

			angle.x *= referenceDraw->GetSizeInPixels().x * 0.85f;
			angle.y *= referenceDraw->GetSizeInPixels().y * 0.85f;

			tailTransform->m_x_Position.x = (
				referenceTransform->m_x_Position.x + angle.x);
			tailTransform->m_x_Position.y = (
				referenceTransform->m_x_Position.y + angle.y);

			FollowSystem::FaceReference(referenceTransform, static_cast<TransformComponent*>(i_Component) );
			//FaceReference(tailTransform, static_cast<TransformComponent*>(i_Component));


			//static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			//TODO: REMOVE HARCCODE
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), textureName);
			//MoveTowardsReference(referenceTransform->GetComponent<DrawComponent>(),
			//							static_cast<DrawComponent*>(i_Component), ownerHeadComponent->GetComponent<PhysicsComponent>());
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 400;
		}
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{
			static_cast<CollisionComponent*>(i_Component)->m_i_CollisionGroupVec.push_back
			(static_cast<CollisionGroupName>(playerNumber * 2 + 1));
		}
	}



	auto followComponent = 
		m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
			newSnekBodyEntity, kComponentFollow);

	followComponent->m_po_ParentEntity = owner;

	//if there are no parts
	if (ownerHeadComponent->m_x_BodyParts.empty())
	{
		followComponent->m_po_FolloweeTransform = ownerTransform;
		ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

	}
	// if the only body part is the tail
	else if (!strcmp(ownerHeadComponent->m_x_BodyParts.front()->m_pc_EntityName, "Tail"))
	{
		followComponent->m_po_FolloweeTransform = ownerTransform;
		ownerHeadComponent->m_x_BodyParts.back()->GetComponent<FollowComponent>()->
			m_po_FolloweeTransform = newSnekBodyEntity->GetComponent<TransformComponent>();
		ownerHeadComponent->m_x_BodyParts.insert(ownerHeadComponent->m_x_BodyParts.end()-1, newSnekBodyEntity);

	}
	else
	{
		//Vec iter that points to the last body part
		auto lastBodyPartVecIter   = ownerHeadComponent->m_x_BodyParts.end();

		//if the last body part is the tail, then we insert the new body part in front of the tail
		// and then make the tail follow it.
		if (!strcmp(ownerHeadComponent->m_x_BodyParts.back()->m_pc_EntityName,
			"Tail"))
		{
			--lastBodyPartVecIter;
			ownerHeadComponent->m_x_BodyParts.back()->GetComponent<FollowComponent>()->
				m_po_FolloweeTransform = newSnekBodyEntity->GetComponent<TransformComponent>();
		}

		//Update the follow component
		followComponent->m_po_FolloweeTransform = (*(lastBodyPartVecIter-1))->GetComponent<TransformComponent>();
		//ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

		ownerHeadComponent->m_x_BodyParts.insert(lastBodyPartVecIter, newSnekBodyEntity);
	}
	m_po_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());
}

//TODO
void SnekSystem::CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const
{
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = 
		m_po_EntityManager->NewEntity<SnekBodyEntity>(kEntitySnekBody, "Tail");

	auto ownerTransform = owner->GetComponent<TransformComponent>();

	auto ownerHeadComponent = owner->GetComponent<SnekHeadComponent>();

	//Set the tail follow to head if there are no body parts, otherwise follow the last body part.
	auto referenceTransform = ownerHeadComponent->m_x_BodyParts.empty() ?
		ownerTransform : ownerHeadComponent->m_x_BodyParts.back()->GetComponent<TransformComponent>();
	
	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			//Set the position to behind its reference
			AEVec2 angle;
			AEVec2FromAngle(&angle, referenceTransform->GetRotation());

			if (referenceTransform->GetComponent<SnekHeadComponent>())
			{
				AEVec2FromAngle(&angle, referenceTransform->GetRotation() + PI);
			}

			auto referenceDraw = referenceTransform->GetComponent<DrawComponent>();

			angle.x *= referenceDraw->GetSizeInPixels().x * 0.55f;
			angle.y *= referenceDraw->GetSizeInPixels().y * 0.55f;

			static_cast<TransformComponent*>(i_Component)->m_x_Position.x = (
				referenceTransform->m_x_Position.x + angle.x);
			static_cast<TransformComponent*>(i_Component)->m_x_Position.y = (
				referenceTransform->m_x_Position.y + angle.y);
			//Change the rotation to face towards reference
			FollowSystem::FaceReference(referenceTransform, static_cast<TransformComponent*>(i_Component));

			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), textureName);
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_MaxSpeed = 400;
		}
		else if (i_Component->m_x_ComponentID == kComponentFollow)
		{
			static_cast<FollowComponent*>(i_Component)->m_po_FolloweeTransform = referenceTransform;
		}
	}

	ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);
}

void SnekSystem::Flip(SnekHeadEntity* owner)
{
	auto snekHeadComponent = owner->GetComponent<SnekHeadComponent>();

	auto headTransformComponent = owner->GetComponent<TransformComponent>();

	auto tailTransformComponent = snekHeadComponent->m_x_BodyParts.back()->GetComponent<TransformComponent>();

	auto headPhysicsComponent = owner->GetComponent<PhysicsComponent>();

	headPhysicsComponent->m_f_Acceleration = 0;
	headPhysicsComponent->m_f_Speed *= 0.6f;

	//Swap the head and tail position
	auto tempX = headTransformComponent->m_x_Position.x;
	auto tempY = headTransformComponent->m_x_Position.y;
	headTransformComponent->m_x_Position.x = (tailTransformComponent->m_x_Position.x);
	headTransformComponent->m_x_Position.y = (tailTransformComponent->m_x_Position.y);
	tailTransformComponent->m_x_Position.x = (tempX);
	tailTransformComponent->m_x_Position.y = (tempY);

	//Swap the head and tail rotation
	tempX = headTransformComponent->GetRotation();
	headTransformComponent->SetRotation(tailTransformComponent->GetRotation());
	tailTransformComponent->SetRotation(tempX);

	//reverse the body parts vector
	std::reverse(snekHeadComponent->m_x_BodyParts.begin(), snekHeadComponent->m_x_BodyParts.end() - 1);

	UpdateFollowComponents(snekHeadComponent);
}

void SnekSystem::UpdateFollowComponents(SnekHeadComponent* snekHeadComponent)
{
	auto toFollowTransformComponent = snekHeadComponent->GetComponent<TransformComponent>();

	//Update follow components to follow the previous one.
	// i.e. part[0] follows head, [1] follows [0], so on
	for (auto& m_x_BodyPart : snekHeadComponent->m_x_BodyParts)
	{
		//Assign the current body part
		m_x_BodyPart->GetComponent<FollowComponent>()->m_po_FolloweeTransform 
			= toFollowTransformComponent;

		//Keep a value to assign to the next body part
		toFollowTransformComponent = m_x_BodyPart->GetComponent<TransformComponent>();
	}
}

void SnekSystem::SetSnekType(int playerNumber, SnekType snekType)
{
	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		if (snekHead->m_i_PlayerNumber == playerNumber) 
		{
			snekHead->m_x_SnekType = snekType;
		}
	}, kComponentSnekHead);
	UpdateSneks();
}

void SnekSystem::UpdateSneks()
{
	int lives[2], Counter = 0;
	SnekType snekType[2];

	auto camera = m_po_SystemManager->GetSystem<CameraSystem>("Camera");
	camera->RemoveCameraTrackObjects();

	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)->void
	{
		lives[Counter] = snekHead->m_i_LivesLeft;
		snekType[Counter] = snekHead->m_x_SnekType;
		++Counter;
		DeleteSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
		//ResetSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
	}, kComponentSnekHead);

	CreateSnek(-200, 0, PI * 3 / 4, 20, snekType[0], 0, lives[0]);
	CreateSnek(200, 0, PI * 7 / 4, 20, snekType[1], 1, lives[1]);

}
