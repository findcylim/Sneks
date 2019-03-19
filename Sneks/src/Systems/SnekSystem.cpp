
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

int SnekSystem::GetWinner()
{
	return winner;
}


float SnekSystem::GetP1GrowthPercentage()
{
	return p1Growth/p1GrowthMeter;
}

float SnekSystem::GetP2GrowthPercentage()
{
	return p2Growth/p2GrowthMeter;
}

void SnekSystem::ResetLives()
{
	p1Lives = 3;
	p2Lives = 3;
}

int SnekSystem::GetP1Lives()
{
	return p1Lives;
}

int SnekSystem::GetP2Lives()
{
	return p2Lives;
}

void SnekSystem::TweakP1Damage(int increase)
{
	i_P1Damage += increase;
}

void SnekSystem::TweakP2Damage(int increase)
{
	i_P2Damage += increase;
}

void SnekSystem::ResetDamage()
{
	i_P1Damage = i_DamageBase;
	i_P2Damage = i_DamageBase;
}

void SnekSystem::IncreaseGrowthRate(unsigned short player, float increase)
{
	if (player == 0)
		p1GrowthMeter /= increase;
	else
		p2GrowthMeter /= increase;
	CheckGrowthMeters();
}

void SnekSystem::DecreaseGrowthRate(unsigned short player, float decrease)
{
	if (player == 0)
		p1GrowthMeter *= decrease;
	else
		p2GrowthMeter *= decrease;
	CheckGrowthMeters();
}

void SnekSystem::CheckGrowthMeters()
{
	auto snekHeadComp = m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);

	while (snekHeadComp)
	{
		float* playerGrowth = &p1Growth;
		float* playerGrowthMeter = &p1GrowthMeter;
		auto texture = "SnekBody01";

		if (snekHeadComp->m_i_PlayerNumber != 0)
		{
			playerGrowth = &p2Growth;
			playerGrowthMeter = &p2GrowthMeter;
			texture = "SnekBody02";
		}

		if (*playerGrowth >= *playerGrowthMeter)
		{
			*playerGrowth = 0;
			*playerGrowthMeter *= 1.1f;
			CreateSnekBody(static_cast<SnekHeadEntity*>(snekHeadComp->m_po_OwnerEntity),
				texture, snekHeadComp->m_i_PlayerNumber);
		}
		snekHeadComp = static_cast<SnekHeadComponent*>(snekHeadComp->m_po_NextComponent);
	}

}

SnekSystem::SnekSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics, GameStateManager* gameStateManagerPtr)
: BaseSystem(entityManagerPtr)
{
	m_o_GameStateManager = gameStateManagerPtr;
	m_o_GraphicsSystem = graphics;
}

SnekSystem::~SnekSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_SNEK_INVULNERABLE>(this);
};


void SnekSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMoon)
	{
		if (eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupSnek2Body)
		{
			auto snekHeadFollow = eventData.object2->m_po_OwnerEntity->
				GetComponent<FollowComponent>()->m_po_ParentEntity->GetComponent<SnekHeadComponent>();
			if (snekHeadFollow->m_po_OwnerEntity->m_b_IsActive)
			{
				if (snekHeadFollow->m_i_PlayerNumber == 0)
				{
					CutSnekBody(snekHeadFollow->m_x_BodyParts.at(
						snekHeadFollow->m_x_BodyParts.size() > i_P2Damage ?
						snekHeadFollow->m_x_BodyParts.size() - i_P2Damage : 0), snekHeadFollow);
					if(i_P1Damage > i_DamageBase)
						i_P1Damage--;
				}
				else
				{
					CutSnekBody(snekHeadFollow->m_x_BodyParts.at(
						snekHeadFollow->m_x_BodyParts.size() > i_P1Damage ?
						snekHeadFollow->m_x_BodyParts.size() - i_P1Damage : 0), snekHeadFollow);
					if (i_P2Damage > i_DamageBase)
						i_P2Damage--;
				}
			}
		}
	}
	else
	{
		//body collision destroys the body
		//HeadCollideBodyCheck(eventData.object1, eventData.object2);
		HeadCollideBodyCheck(eventData.object2, eventData.object1);
	}

	if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupBuilding ||
		 eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupBuilding)
	{
		
		auto objectColliding = eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupBuilding ?
			eventData.object1 : eventData.object2;
		auto otherObjectCollide = eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupBuilding ?
			eventData.object1 : eventData.object2;

		//Create new snek Body parts
		if (auto snekHeadComp = 
				otherObjectCollide->m_po_OwnerEntity->GetComponent<SnekHeadComponent>())
		{
			if (snekHeadComp->m_po_OwnerEntity->m_b_IsActive)
			{
				if (snekHeadComp->m_i_PlayerNumber == 0)
				{
					p1Growth += 0.5f;
					CheckGrowthMeters();
				}
				else
				{
					p2Growth += 0.5f;
					CheckGrowthMeters();
				}
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

	//if both have snek heads
	if (auto snekHed1 = eventData.object1->m_po_OwnerEntity->GetComponent<SnekHeadComponent>())
	{
		if (auto snekHed2 = eventData.object2->m_po_OwnerEntity->GetComponent<SnekHeadComponent>())
		{
			if (snekHed1->m_po_OwnerEntity->m_b_IsActive && snekHed2->m_po_OwnerEntity->m_b_IsActive)
			{
			/*	auto snekTrans1 = m_po_ComponentManager->GetSpecificComponentInstance
					<TransformComponent>(snekHed1, kComponentTransform);
				auto snekTrans2 = m_po_ComponentManager->GetSpecificComponentInstance
					<TransformComponent>(snekHed2, kComponentTransform);*/
/*
				float xDistance = snekTrans2->GetPosition().x - snekTrans1->GetPosition().x;
				float yDistance = snekTrans2->GetPosition().y - snekTrans1->GetPosition().y;
				float angle, snekHedDir;
				bool collide;

				collide = false;
				snekHedDir = snekTrans1->GetRotation();

				if (xDistance != 0)
				{
					angle = AEATan(yDistance / xDistance);

					if (xDistance > 0 && angle > PI / 2 && angle < PI / 2 + PI)
					{
						if (angle < PI)
							angle += PI;
						else
							angle -= PI;
					}
				}
				else if (yDistance > 0)
					angle = PI / 2;
				else
					angle = PI + PI / 2;

				if (angle < 0)
					angle += PI * 2;
				else if (angle > PI * 2)
					angle -= PI * 2;

				if (angle < snekHedDir + f_AngleHeadHit && angle > snekHedDir - f_AngleHeadHit)
					collide = true;
				else if(snekHedDir + f_AngleHeadHit > PI * 2)
				{
					if (angle < snekHedDir + f_AngleHeadHit - PI * 2 ||
						angle > snekHedDir - f_AngleHeadHit)
						collide = true;
				}
				else if(snekHedDir - f_AngleHeadHit < 0)
				{
					if (angle < snekHedDir + f_AngleHeadHit ||
						angle > snekHedDir - f_AngleHeadHit + PI * 2)
						collide = true;
				}
*/
				//if (collide)
				//{
				if (snekHed2->m_x_BodyParts.size() > 1)
				{
					int i_Damage = i_P2Damage / 2;
					while (snekHed2->m_x_BodyParts.size() > 1) 
					{
						CutSnekBody(snekHed2->m_x_BodyParts[snekHed2->m_x_BodyParts.size() - 2], snekHed2);
						if (--i_Damage)
							break;
					}
					//if (i_P2Damage > i_DamageBase)
					//	i_P2Damage--;
				}
				else
				{
					p2Lives--;
					ResetSnek(static_cast<SnekHeadEntity*>(snekHed1->m_po_OwnerEntity));
					ResetSnek(static_cast<SnekHeadEntity*>(snekHed2->m_po_OwnerEntity));
					m_o_SystemManager->GetSystem<BuildingsSystem>("Buildings")->ResetLevel1();
				}
			//	}

				//collide = false;
				/*snekHedDir = snekTrans2->GetRotation();

				xDistance = -xDistance;
				yDistance = -yDistance;

				if (angle < PI)
					angle += PI;
				else
					angle -= PI;

				if (angle < snekHedDir + f_AngleHeadHit && angle > snekHedDir - f_AngleHeadHit)
					collide = true;
				else if (snekHedDir + f_AngleHeadHit > PI * 2)
				{
					if (angle < snekHedDir + f_AngleHeadHit - PI * 2 ||
						angle > snekHedDir - f_AngleHeadHit)
						collide = true;
				}
				else if (snekHedDir - f_AngleHeadHit < 0)
				{
					if (angle < snekHedDir + f_AngleHeadHit ||
						angle > snekHedDir - f_AngleHeadHit + PI * 2)
						collide = true;
				}

				if (collide)
				{*/
				if (snekHed1->m_x_BodyParts.size() > 1)
				{
					int i_Damage = i_P1Damage / 2;
					while (snekHed1->m_x_BodyParts.size() > 1)
					{
						CutSnekBody(snekHed1->m_x_BodyParts[snekHed1->m_x_BodyParts.size() - 2], snekHed1);
						if (--i_Damage)
							break;
					}
					//if (i_P1Damage > i_DamageBase)
					//	i_P1Damage--;
				}
				else
				{
					p1Lives--;
					ResetSnek(static_cast<SnekHeadEntity*>(snekHed1->m_po_OwnerEntity));
					ResetSnek(static_cast<SnekHeadEntity*>(snekHed2->m_po_OwnerEntity));
					//TODO REMOVE DEPENDENCY
					m_o_SystemManager->GetSystem<BuildingsSystem>("Buildings")->ResetLevel1();

				}
				//}

					/*
					m_po_EntityManager->AddToDeleteQueue(snekHed2->m_x_BodyParts[0]);
					m_po_EntityManager->AddToDeleteQueue(snekHed2->m_po_OwnerEntity);
					auto i_CameraComponent = static_cast<CameraComponent*>(
						m_po_ComponentManager->GetFirstComponentInstance(kComponentCamera));

				//	CreateSnek(0, 0, 180, 20, "SnekHead02", 1);
				//	m_po_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());
				//	*/
				//}

				//todo: optimize this portion
				if (p1Lives <= 0)
				{
					winner = 2;
					m_o_GameStateManager->SetState(kStateWinScreen);
					/* m_o_SystemManager->DisableSystem<PhysicsSystem, DrawComponent, kComponentDraw>();

					auto WinScreen = new WinScreenSystem(m_po_EntityManager, m_po_EventManagerPtr, static_cast<char>(2));
					WinScreen->SetName("WinScreen");
					m_o_SystemManager->AddSystem(WinScreen);

					m_po_EntityManager->DisableComponentsFromEntityType<SnekBodyEntity, kEntitySnekBody, CollisionComponent>();
					m_po_EntityManager->DisableComponentsFromEntityType<SnekHeadEntity, kEntitySnekHead, CollisionComponent>(); */

					/*auto snek = m_po_EntityManager->GetFirstEntityInstance<SnekHeadEntity>(kEntitySnekHead);
					while(snek)
					{
						snek->m_b_IsActive = false;
						snek = static_cast<SnekHeadEntity*>(snek->m_po_NextEntity);
					}*/
				}
				else if (p2Lives <= 0)
				{
					winner = 1;
					m_o_GameStateManager->SetState(kStateWinScreen);
					/*
					m_o_SystemManager->DisableSystem<PhysicsSystem, DrawComponent, kComponentDraw>();

					auto WinScreen = new WinScreenSystem(m_po_EntityManager, m_po_EventManagerPtr, static_cast<char>(2));
					WinScreen->SetName("WinScreen");
					m_o_SystemManager->AddSystem(WinScreen);

					m_po_EntityManager->DisableComponentsFromEntityType<SnekBodyEntity, kEntitySnekBody, CollisionComponent>();
					m_po_EntityManager->DisableComponentsFromEntityType<SnekHeadEntity, kEntitySnekHead, CollisionComponent>();
					/*auto snek = m_po_EntityManager->GetFirstEntityInstance<SnekHeadEntity>(kEntitySnekHead);
					while(snek)
					{
						snek->m_b_IsActive = false;
						snek = static_cast<SnekHeadEntity*>(snek->m_po_NextEntity);
					}*/
				}

				srand(clock());
				auto randDirection = rand() % 360;
				snekHed1->m_po_OwnerEntity->GetComponent<TransformComponent>()->SetRotation(AEDegToRad(static_cast<float>(randDirection)));
				snekHed2->m_po_OwnerEntity->GetComponent<TransformComponent>()->SetRotation(AEDegToRad(static_cast<float>(randDirection) + 180.0f));
				float* snek1Speed = &snekHed1->m_po_OwnerEntity->GetComponent<PhysicsComponent>()->m_f_Speed;
				float* snek2Speed = &snekHed2->m_po_OwnerEntity->GetComponent<PhysicsComponent>()->m_f_Speed;
				float newSpeed = (*snek1Speed + *snek2Speed) * 0.75f;
				*snek1Speed = newSpeed;
				*snek2Speed = newSpeed;

				//HeadApplyRecoil(snekHed1, snekHed2);
				//HeadApplyRecoil(snekHed2, snekHed1);
				HeadInvulnerableSet(1.0f, snekHed1);
				HeadInvulnerableSet(1.0f, snekHed2);
			}
		}
	}
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

void SnekSystem::HeadCollideBodyCheck(CollisionComponent* victimCollision, CollisionComponent* aggressorCollision)
{
	if (victimCollision->m_i_CollisionGroupVec[0] == kCollGroupSnek1Body ||
		(victimCollision->m_i_CollisionGroupVec[0] == kCollGroupSnek2Body))
	{
		auto snekHeadAggressor = aggressorCollision->GetComponent<SnekHeadComponent>();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(
			//	aggressorCollision, kComponentSnekHead
			//);

		//Get the parent
		auto objectFollowComp = victimCollision->GetComponent<FollowComponent>();
			//m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
			//	victimCollision, kComponentFollow
			//);

		auto snekHeadVictim = objectFollowComp->m_po_ParentEntity->GetComponent<SnekHeadComponent>();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(
			//	objectFollowComp->m_po_ParentEntity, kComponentSnekHead
			//);
		auto physicsAggressor = aggressorCollision->GetComponent<PhysicsComponent>();

		/*for (auto snekBodyPart : snekHeadVictim->m_x_BodyParts)
		{
			auto bodyPhysics = snekBodyPart->GetComponent<PhysicsComponent>();
			bodyPhysics->SetVelocity(bodyPhysics->m_x_Velocity + HTVector2{physicsAggressor->m_x_Velocity} * 0.1f);
		}*/
		auto victimPhysics = snekHeadVictim->GetComponent<PhysicsComponent>();
		victimPhysics->SetVelocity(victimPhysics->m_x_Velocity + HTVector2{ physicsAggressor->m_x_Velocity } * 0.1f);

		HeadApplyRecoil(snekHeadAggressor, snekHeadVictim);

		if (snekHeadVictim->m_i_PlayerNumber == 0)
		{
			CutSnekBody(snekHeadVictim->m_x_BodyParts.at(
				snekHeadVictim->m_x_BodyParts.size() > i_P2Damage ?
				snekHeadVictim->m_x_BodyParts.size() - i_P2Damage : 0), snekHeadVictim);
			if (i_P1Damage > i_DamageBase)
				i_P1Damage--;
		}
		else
		{
			CutSnekBody(snekHeadVictim->m_x_BodyParts.at(
				snekHeadVictim->m_x_BodyParts.size() > i_P1Damage ?
				snekHeadVictim->m_x_BodyParts.size() - i_P1Damage : 0), snekHeadVictim);
			if (i_P2Damage > i_DamageBase)
				i_P2Damage--;
		}

		HeadInvulnerableSet(3.0f, snekHeadVictim);

	}	
}

void SnekSystem::Receive(const Events::EV_SNEK_INVULNERABLE& eventData)
{
	BodyInvulnerableSet(eventData.snekHead);
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

		if (i_SnekHead->m_f_BoostCooldown > 0)
			i_SnekHead->m_f_BoostCooldown -= dt;
		else if (i_SnekHead->m_x_SnekType == kSnekTypeSpeed &&
			i_SnekHead->m_f_AccelerationForce > 200)
		{
			i_SnekHead->m_f_AccelerationForce = 200;
			i_SnekHead->GetComponent<PhysicsComponent>()->m_f_MaxSpeed /= 1.5f;
			i_SnekHead->GetComponent<BloomComponent>()->m_f_BloomStrength = 0.2f;
			for (auto bodyPart : i_SnekHead->m_x_BodyParts)
				bodyPart->GetComponent<BloomComponent>()->m_f_BloomStrength = 0.2f;
			i_SnekHead->m_f_BoostCooldown = i_SnekHead->m_f_BoostSetCooldown;
		}

		if (GetAsyncKeyState(i_SnekHead->m_i_AccelerationKey)) 
		{
			Events::EV_PLAYER_MOVEMENT_KEY moveKey{ headPhysicsComponent, Events::MOVE_KEY_UP};
			m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKey);
		}
		else
		{
			headPhysicsComponent->m_f_Acceleration = 0;
		}

		if (AEInputCheckTriggered(static_cast<u8>(i_SnekHead->m_i_BoostKey)))
		{
			if (i_SnekHead->m_x_SnekType == kSnekTypeFlip)
			{
				Flip(static_cast<SnekHeadEntity*>(headTransComponent->m_po_OwnerEntity));
			}
			else if (i_SnekHead->m_f_BoostCooldown <= 0 &&
				i_SnekHead->m_x_SnekType == kSnekTypeShoot)
			{
				Events::EV_CREATE_PROJECTILE projData;

				projData.pos = &headTransComponent->m_x_Position;

				projData.velocity = &headPhysicsComponent->m_x_Velocity;

				projData.rot = headTransComponent->GetRotation();
				projData.speed = 1400.0f;
				projData.scale = 0.1f;
				projData.isCollide = true;

				projData.texName = "Moon";

				i_SnekHead->m_f_BoostCooldown = i_SnekHead->m_f_BoostSetCooldown;

				m_po_EventManagerPtr->EmitEvent<Events::EV_CREATE_PROJECTILE>(projData);
			}
			else if (i_SnekHead->m_f_BoostCooldown <= 0 &&
				i_SnekHead->m_x_SnekType == kSnekTypeSpeed)
			{
				i_SnekHead->m_f_AccelerationForce = 2000;
				i_SnekHead->GetComponent<PhysicsComponent>()->m_f_MaxSpeed *= 1.5f;
				i_SnekHead->GetComponent<BloomComponent>()->m_f_BloomStrength = 0.45f;
				for (auto bodyPart : i_SnekHead->m_x_BodyParts)
					bodyPart->GetComponent<BloomComponent>()->m_f_BloomStrength = 0.45f;
				i_SnekHead->m_f_BoostCooldown = i_SnekHead->m_f_BoostSetCooldown;
			}
		}

		if (GetAsyncKeyState(i_SnekHead->m_i_LeftKey))
		{
			Events::EV_PLAYER_MOVEMENT_KEY moveKey{ headPhysicsComponent, Events::MOVE_KEY_LEFT };
			m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKey);
		}
		else if (GetAsyncKeyState(i_SnekHead->m_i_RightKey))
		{
			Events::EV_PLAYER_MOVEMENT_KEY moveKey{ headPhysicsComponent,Events::MOVE_KEY_RIGHT };
			m_po_EventManagerPtr->EmitEvent<Events::EV_PLAYER_MOVEMENT_KEY>(moveKey);
		}

		for (auto i_Body : i_SnekHead->m_x_BodyParts)
		{
			auto bodyDraw = 
				m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
					i_Body, kComponentDraw
				);

			auto followComponent = 
				m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
					i_Body, kComponentFollow
				);

			auto followDrawComponent = 
				m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
					followComponent->m_po_TransformComponent->m_po_OwnerEntity, kComponentDraw
				);
			
			FaceReference(followComponent->m_po_TransformComponent, bodyDraw->m_po_TransformComponent);
			if (i_SnekHead->m_i_PlayerNumber)
				MoveTowardsReference(followDrawComponent, bodyDraw, headPhysicsComponent);
			else
				MoveTowardsReference2(followDrawComponent, bodyDraw, headPhysicsComponent);
		}

		i_SnekHead = static_cast<SnekHeadComponent*>(i_SnekHead->m_po_NextComponent);
	}
}

constexpr float MAX_ALPHA_BLINKING = 0.5f;
constexpr float MIN_ALPHA_BLINKING = 0.1f;
constexpr float BLINK_SPEED = 2.0f;

void SnekSystem::CheckInvulnerability(BaseComponent* component, float dt) const
{
	auto invulComponent = component->GetComponent<InvulnerableComponent>();

	auto drawComponent = component->GetComponent<DrawComponent>();

	auto collisionComponent = component->GetComponent<CollisionComponent>();

	if (invulComponent->m_f_InvulnerableTime > 0)
	{
		float blinkSpeedModifier = 1.0f / invulComponent->m_f_InvulnerableTime;
		float blinkSpeedModifierClamped = AEClamp(blinkSpeedModifier, 0.2f, 1.0f);

		drawComponent->m_f_RgbaColor.alpha -= blinkSpeedModifierClamped * BLINK_SPEED * dt;

		if (drawComponent->GetAlpha() <= MIN_ALPHA_BLINKING)
			drawComponent->SetAlpha(MAX_ALPHA_BLINKING);

		collisionComponent->enabled = false;
		invulComponent->m_f_InvulnerableTime -= dt;
	}
	else 
	{
		collisionComponent->enabled = true;
		drawComponent->SetAlpha(1.0f);
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
	m_po_EventManagerPtr->AddListener<Events::EV_SNEK_INVULNERABLE>(this, this);
}

//HEAD SIZE : 105, 77
//BODY SIZE:  61,  80
//SCALE : 0.635f
void SnekSystem::CreateSnek(float posX, float posY, float rotation,
	const int numBodyParts, const char* textureName, int controlScheme) const
{
	//Count the number of previous snek heads
	int snekHeadCount = 0;
	auto i_SnekHeadComponents = 
		m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(
			kComponentSnekHead
		);

	auto tailTexture = "SnekTail01";
	auto bodyTexture = "SnekBody01";
	auto spriteCountX = 2;
	auto spriteCountY = 1;
	if (!strcmp(textureName, "SnekHead02"))
	{
		bodyTexture = "SnekBody02";
		tailTexture = "SnekTail02";
		spriteCountX = 1;
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
			static_cast<TransformComponent*>(i_Component)->SetPositionX(posX);
			static_cast<TransformComponent*>(i_Component)->SetPositionY(posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(rotation);

			auto cameraComponent = 
				m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(
					kComponentCamera
				);
			cameraComponent->AddToTrack(static_cast<TransformComponent*>(i_Component));
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if(i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), textureName, 
																		HTColor{ 1,1,1,1 }, spriteCountX, spriteCountY);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 4;
		}
		else if (i_Component->m_x_ComponentID == kComponentAnimation)
		{
			auto animComp = static_cast<AnimationComponent*>(i_Component);
			Animation anim(SpriteSheet{textureName, spriteCountX,spriteCountY}, 0, spriteCountX * spriteCountY);
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
			if (controlScheme)
			{
				
			}else
			{//TODO:: REMOVE HARDCODE
				static_cast<SnekHeadComponent*>(i_Component)->m_i_AccelerationKey = AEVK_W;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_BrakeKey = AEVK_S;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_LeftKey = AEVK_A;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_RightKey = AEVK_D;
				static_cast<SnekHeadComponent*>(i_Component)->m_i_BoostKey = AEVK_LCTRL;
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
	}



	for (int i_BodyParts = 0; i_BodyParts < numBodyParts; i_BodyParts++){
		CreateSnekBody(newSnekHeadEntity, bodyTexture, controlScheme);
	}
	
	CreateSnekTail(newSnekHeadEntity, tailTexture);
}

void SnekSystem::ResetSnek(SnekHeadEntity* owner)
{
	auto snekHeadComp = owner->GetComponent<SnekHeadComponent>();
	auto playerNumber = snekHeadComp->m_i_PlayerNumber;
	auto transformComp = owner->GetComponent<TransformComponent>();
	HTVector2 velocity;
	velocity.x = 0;
	velocity.y = 0;

	owner->GetComponent<PowerUpComponent>()->ResetPowerIncrease();

	transformComp->SetPositionY(0);

	if (playerNumber == 0)
	{
		transformComp->SetRotation(PI);
		transformComp->SetPositionX(-200);
		owner->GetComponent<PhysicsComponent>()->SetVelocity(velocity);
	}
	else
	{
		transformComp->SetRotation(0);
		transformComp->SetPositionX(200);
		owner->GetComponent<PhysicsComponent>()->SetVelocity(velocity);
	}

	for (int i_BodyParts = 0; i_BodyParts < static_cast<int>(20 - snekHeadComp->m_x_BodyParts.size()); i_BodyParts++) 
	{
		if (playerNumber == 0)
			CreateSnekBody(static_cast<SnekHeadEntity*>(owner), "SnekBody01", playerNumber);
		else
			CreateSnekBody(static_cast<SnekHeadEntity*>(owner), "SnekBody02", playerNumber);
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
	SnekBodyEntity* toCut = snekHead->m_x_BodyParts[partsToRemove - 1];
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

		tailFollowComponent->m_po_TransformComponent = snekHeadTransform;
	}
	else {
		auto lastBodyTransformComponent = m_po_ComponentManager->GetSpecificComponentInstance
		<TransformComponent>(snekHead->m_x_BodyParts.at(snekHead->m_x_BodyParts.size() - 2), kComponentTransform);

		tailFollowComponent->m_po_TransformComponent = lastBodyTransformComponent;
	}
}

void SnekSystem::CreateSnekBody(SnekHeadEntity* owner, const char* textureName, int playerNumber) const 
{
	//TODO:: MESH INSTANCING
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = 
		m_po_EntityManager->NewEntity<SnekBodyEntity>(kEntitySnekBody, "Body");

	auto ownerTransform = owner->GetComponent<TransformComponent>();

	auto ownerHeadComponent = owner->GetComponent<SnekHeadComponent>();

	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{
		auto referenceTransform = ownerHeadComponent->m_x_BodyParts.size() <= 1 ?
			ownerTransform :
			(*(ownerHeadComponent->m_x_BodyParts.end()-2))->GetComponent<TransformComponent>();

		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			AEVec2 angle;
			AEVec2FromAngle(&angle, referenceTransform->GetRotation() );

			if (referenceTransform->GetComponent<SnekHeadComponent>())
			{
				AEVec2FromAngle(&angle, referenceTransform->GetRotation() + PI);
			}
			auto referenceDraw = referenceTransform->GetComponent<DrawComponent>();

			angle.x *= referenceDraw->GetSizeInPixels().x * 0.85f;
			angle.y *= referenceDraw->GetSizeInPixels().y * 0.85f;

			static_cast<TransformComponent*>(i_Component)->SetPositionX(
				referenceTransform->m_x_Position.x + angle.x);
			static_cast<TransformComponent*>(i_Component)->SetPositionY(
				referenceTransform->m_x_Position.y + angle.y);

			FaceReference(referenceTransform, static_cast<TransformComponent*>(i_Component) );
			//static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			//TODO: REMOVE HARCCODE
			static_cast<TransformComponent*>(i_Component)->m_f_Scale = 0.635f;

		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component), textureName);
			MoveTowardsReference(referenceTransform->GetComponent<DrawComponent>(),
										static_cast<DrawComponent*>(i_Component), ownerHeadComponent->GetComponent<PhysicsComponent>());
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
		followComponent->m_po_TransformComponent = ownerTransform;
		ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

	}
	// if the only body part is the tail
	else if (!strcmp(ownerHeadComponent->m_x_BodyParts.front()->m_pc_EntityName, "Tail"))
	{
		followComponent->m_po_TransformComponent = ownerTransform;
		ownerHeadComponent->m_x_BodyParts.back()->GetComponent<FollowComponent>()->
			m_po_TransformComponent = newSnekBodyEntity->GetComponent<TransformComponent>();
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
				m_po_TransformComponent = newSnekBodyEntity->GetComponent<TransformComponent>();
		}

		//Update the follow component
		followComponent->m_po_TransformComponent = (*(lastBodyPartVecIter-1))->GetComponent<TransformComponent>();
		//ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

		ownerHeadComponent->m_x_BodyParts.insert(lastBodyPartVecIter, newSnekBodyEntity);
	}
	m_po_EventManagerPtr->EmitEvent<Events::EV_ENTITY_POOL_CHANGED>(Events::EV_ENTITY_POOL_CHANGED());
}

//TODO
void SnekSystem::CreateSnekTail(SnekHeadEntity* owner, const char* textureName) const
{
	//TODO:: MESH INSTANCING
	//Create a new body part to add to the vector
	auto newSnekBodyEntity = 
		m_po_EntityManager->NewEntity<SnekBodyEntity>(kEntitySnekBody, "Tail");

	auto ownerTransform = 
		m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
			owner, kComponentTransform);

	auto ownerHeadComponent = owner->GetComponent<SnekHeadComponent>();

	auto referenceTransform = ownerHeadComponent->m_x_BodyParts.size() <= 1 ?
		ownerTransform :
		(*(ownerHeadComponent->m_x_BodyParts.end() - 2))->GetComponent<TransformComponent>();
	
	for (auto i_Component : newSnekBodyEntity->m_v_AttachedComponentsList)
	{

		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			AEVec2 angle;
			AEVec2FromAngle(&angle, referenceTransform->GetRotation());

			if (referenceTransform->GetComponent<SnekHeadComponent>())
			{
				AEVec2FromAngle(&angle, referenceTransform->GetRotation() + PI);
			}

			auto referenceDraw = referenceTransform->GetComponent<DrawComponent>();

			angle.x *= referenceDraw->GetSizeInPixels().x * 0.55f;
			angle.y *= referenceDraw->GetSizeInPixels().y * 0.55f;


			static_cast<TransformComponent*>(i_Component)->SetPositionX(
				referenceTransform->m_x_Position.x + angle.x);
			static_cast<TransformComponent*>(i_Component)->SetPositionY(
				referenceTransform->m_x_Position.y + angle.y);

			FaceReference(referenceTransform, static_cast<TransformComponent*>(i_Component));
			//static_cast<TransformComponent*>(i_Component)->SetRotation(0);
			//TODO: REMOVE HARCCODE
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
		else if (i_Component->m_x_ComponentID == KComponentInvulnerable)
		{
			static_cast<InvulnerableComponent*>(i_Component)->m_f_InvulnerableTime = 0;

		}
		else if (i_Component->m_x_ComponentID == kComponentCollision)
		{

		}
	}

	auto followComponent = 
		m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
			newSnekBodyEntity, kComponentFollow);

	if (ownerHeadComponent->m_x_BodyParts.empty())
	{
		followComponent->m_po_TransformComponent = ownerTransform;
	}
	else
	{
		auto toFollowTransform =
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
				ownerHeadComponent->m_x_BodyParts.back(), kComponentTransform);

		followComponent->m_po_TransformComponent = toFollowTransform;
	}
	ownerHeadComponent->m_x_BodyParts.push_back(newSnekBodyEntity);

}

void SnekSystem::FaceReference(const TransformComponent* reference, TransformComponent* toChange) const
{
	float referenceEdgeX = (reference->m_x_Position.x);// +refHead->GetRotatedOffsetXx());
	float referenceEdgeY = (reference->m_x_Position.y);// +refHead->GetRotatedOffsetXy());

	float distanceX = toChange->m_x_Position.x - referenceEdgeX;
	float distanceY = toChange->m_x_Position.y - referenceEdgeY;

	auto newRot = static_cast<float>(atan2(distanceY, distanceX));
	// make sure rotation is between 0 and 2Pi
	if (newRot < 0)
		newRot += 2 * PI;
	else if (newRot >= 2 * PI)
		newRot -= 2 * PI;

	toChange->SetRotation(newRot);
	//toChange->m_f_Scale = reference->m_f_Scale;

}

void SnekSystem::MoveTowardsReference(DrawComponent* reference, DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const
{

	AEVec2 rotation;
	AEVec2FromAngle(&rotation, toChange->m_po_TransformComponent->GetRotation() );

	auto stretchThreshold = 600.0f; //Any faster than this speed the snek will start stretching
	auto stretchFactorMultiplier = 0.3f;
	auto stretchFactor  = 100.0f / stretchThreshold;

	if (stretchFactor < 1.0f)
		stretchFactor = 1.0f;

	auto scaleFactor = (stretchFactor - 1.0f) * stretchFactorMultiplier + 1.0f;

	if (headPhysicsComponent->m_po_OwnerEntity == reference->m_po_OwnerEntity)
	{
		stretchFactor = 0.3f;
	}

	toChange->m_po_TransformComponent->m_f_ScaleMultiplier.x = scaleFactor;

	toChange->m_po_TransformComponent->m_x_Position.x = reference->m_po_TransformComponent->m_x_Position.x + 
		rotation.x * reference->m_po_TransformComponent->m_f_Scale.x / 3 * stretchFactor;
	toChange->m_po_TransformComponent->m_x_Position.y = reference->m_po_TransformComponent->m_x_Position.y + 
		rotation.y * reference->m_po_TransformComponent->m_f_Scale.y / 3 * stretchFactor;

}

void SnekSystem::MoveTowardsReference2(DrawComponent* reference, DrawComponent* toChange, PhysicsComponent* headPhysicsComponent) const
{

	float distanceX = toChange->m_po_TransformComponent->m_x_Position.x -
		 reference->m_po_TransformComponent->m_x_Position.x;
	float distanceY = toChange->m_po_TransformComponent->m_x_Position.y -
		 reference->m_po_TransformComponent->m_x_Position.y;

	auto headBodyAllowance = 0.83f;
	auto headBodyClosenessMultiplier = 0.4f;
	auto stretchFactor = headPhysicsComponent->m_f_Speed / 300.0f;
	if (stretchFactor > 1.0f)
		stretchFactor = 1.0f;

	if (headPhysicsComponent->m_po_OwnerEntity == reference->m_po_OwnerEntity) {
		toChange->m_po_TransformComponent->m_x_Position.x =
			reference->m_po_TransformComponent->m_x_Position.x + distanceX
			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));

		toChange->m_po_TransformComponent->m_x_Position.y =
			reference->m_po_TransformComponent->m_x_Position.y + distanceY
			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
	}
	else
	{
		headBodyAllowance = 0.95f;
		headBodyClosenessMultiplier = 0.4f;// *reference->m_po_TransformComponent->m_f_Scale;
		stretchFactor = headPhysicsComponent->m_f_Speed / 900.0f;

		toChange->m_po_TransformComponent->m_x_Position.x =
			reference->m_po_TransformComponent->m_x_Position.x + distanceX // hypo * 15
			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));

		toChange->m_po_TransformComponent->m_x_Position.y =
			reference->m_po_TransformComponent->m_x_Position.y + distanceY // hypo * 15
			* (headBodyAllowance - headBodyClosenessMultiplier * (stretchFactor));
	}
}

//float timeStamp1 = 0;

void SnekSystem::Flip(SnekHeadEntity* owner)
{
	//float currTime = getDt() - timeStamp1;
	//if (currTime > 3)
	//{
		/*Swap head and tail positions*/
		auto snekHeadComponent = owner->GetComponent<SnekHeadComponent>();
		AE_ASSERT_MESG(snekHeadComponent,"SnekHead in Flip not found");

		auto headTransformComponent = owner->GetComponent<TransformComponent>();
		AE_ASSERT_MESG(headTransformComponent, "SnekHead Trans in Flip not found");

		auto tailTransformComponent = snekHeadComponent->m_x_BodyParts.back()->GetComponent<TransformComponent>();
		AE_ASSERT_MESG(tailTransformComponent, "SnekHead Tail trans in Flip not found");

		auto headPhysicsComponent = owner->GetComponent<PhysicsComponent>();
		AE_ASSERT_MESG(headPhysicsComponent, "SnekHead physics in Flip not found");

		headPhysicsComponent->m_f_Acceleration = 0;
		headPhysicsComponent->m_f_Speed = 0;

		auto tempX = headTransformComponent->GetPosition().x;
		auto tempY = headTransformComponent->GetPosition().y;
		std::cout << "new Head PositionXY: " << tailTransformComponent->GetPosition().x << ", " << tailTransformComponent->GetPosition().y << std::endl;
		headTransformComponent->SetPositionX(tailTransformComponent->GetPosition().x);
		headTransformComponent->SetPositionY(tailTransformComponent->GetPosition().y);
		tailTransformComponent->SetPositionX(tempX);
		tailTransformComponent->SetPositionY(tempY);

		tempX = headTransformComponent->GetRotation();
		std::cout << "new Head Rotation: " << tailTransformComponent->GetRotation() << std::endl;
		headTransformComponent->SetRotation(tailTransformComponent->GetRotation());
		tailTransformComponent->SetRotation(tempX);

		//reverse the body parts vector
		std::reverse(snekHeadComponent->m_x_BodyParts.begin(), snekHeadComponent->m_x_BodyParts.end() - 1);

		UpdateFollowComponents(snekHeadComponent);

		//timeStamp1 = getDt();
	//}
}

void SnekSystem::UpdateFollowComponents(SnekHeadComponent* snekHeadComponent)
{

	auto toFollowTransformComponent =
		m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
			snekHeadComponent, kComponentTransform);

	//Update follow components
	for (unsigned i_BodyPartsFront = 0; i_BodyPartsFront < snekHeadComponent->m_x_BodyParts.size(); i_BodyPartsFront++)
	{
		auto bodyFollowComponent =
			m_po_ComponentManager->GetSpecificComponentInstance<FollowComponent>(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentFollow
				);

		bodyFollowComponent->m_po_TransformComponent = toFollowTransformComponent;

		toFollowTransformComponent =
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
				snekHeadComponent->m_x_BodyParts[i_BodyPartsFront], kComponentTransform
				);
	}
}


void SnekSystem::SetSnek(int input)
{
	switch (input)
	{
		case 1:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
					(kComponentSnekHead);

			if (snekHeadComponent->m_i_PlayerNumber == 0)
				snekHeadComponent =
					static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeSpeed;
		}
		break;

		case 2:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
				(kComponentSnekHead);

			if (snekHeadComponent->m_i_PlayerNumber == 0)
				snekHeadComponent =
				static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeShoot;
		}
		break;

		case 3:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
				(kComponentSnekHead);

			if (snekHeadComponent->m_i_PlayerNumber == 0)
				snekHeadComponent =
				static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeFlip;
		}
		break;

		case 4:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
				(kComponentSnekHead);

			if (snekHeadComponent->m_i_PlayerNumber != 0)
				snekHeadComponent =
				static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeSpeed;
		}
		break;

		case 5:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
				(kComponentSnekHead);

			if (snekHeadComponent->m_i_PlayerNumber != 0)
				snekHeadComponent =
				static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeShoot;
		}
		break;

		case 6:
		{
			auto snekHeadComponent =
				m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>
				(kComponentSnekHead);

			while (snekHeadComponent->m_i_PlayerNumber != 1)
				snekHeadComponent =
				static_cast<SnekHeadComponent*>(snekHeadComponent->m_po_NextComponent);

			snekHeadComponent->m_x_SnekType = kSnekTypeFlip;
		}
		break;
	}
}
