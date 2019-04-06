/* Start Header****************************************************************/
/*!
\file ParticleSystem.cpp
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 28.74%  (Rework Speed Powerup)
\par Contribution : Edwin  - 71.26%  (Main code)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "ParticleSystem.h"
#include <random>
#include <iostream>
#include "CollisionSystem.h"
#include "../Components/PowerUpHolderComponent.h"

ParticleSystem::ParticleSystem(GraphicsSystem* graphics)
{
	m_o_GraphicsSystem = graphics;
}

ParticleSystem::~ParticleSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void ParticleSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);
}

void ParticleSystem::ResetTrails()
{
	static ParticleSpawnerComponent* trails[2];

	while (m_i_CreatedTrails)
	{
		m_po_EntityManager->AddToDeleteQueue(trails[m_i_CreatedTrails - 1]->m_po_OwnerEntity);
		--m_i_CreatedTrails;
	}

	m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)
	{
		trails[m_i_CreatedTrails] = CreateParticleSpawner(snekHead->m_x_BodyParts.back()->
			GetComponent<TransformComponent>(), kParticleTrailEffect);
		if (!snekHead->m_i_PlayerNumber)
		{
			trails[m_i_CreatedTrails]->m_x_ParticleEffectColor ={ 0, 0,0.45f,1.0f };
		}
		else
		{
			trails[m_i_CreatedTrails]->m_x_ParticleEffectColor ={ 0.5f,0,0.1f,1.0f };
		}
		++m_i_CreatedTrails;
	}, kComponentSnekHead, true);
}

void ParticleSystem::Update(float dt)
{
	if (!m_i_CreatedTrails)
	{
		ResetTrails();
	}
	//int snekCount = 0;
	//m_po_ComponentManager->Each<SnekHeadComponent>([&](SnekHeadComponent* snekHead)
	//{
	//	trails[snekCount]->SetSpawnTransform(snekHead->m_x_BodyParts.back()->
	//													 GetComponent<TransformComponent>());
	//	++snekCount;
	//}, kComponentSnekHead);

	for (auto pec = m_po_ComponentManager->GetFirstComponentInstance
		<ParticleSpawnerComponent>(Component::kComponentParticleEffect);
		pec != nullptr;	pec = static_cast<ParticleSpawnerComponent*>(pec->m_po_NextComponent))
	{

		if (pec->IsParticleEffectAlive())
		{
			if (pec->m_b_IsParticleEffectOneShot)
			{
				int density = pec->m_i_ParticleSpawnDensity;
				for (int iter = 0; iter < density; iter++) 
					SpawnParticle(pec);
				
			}
			else
			{
				float& spawnTimer = pec->m_f_ParticleEffectSpawnTimer;
				spawnTimer += dt;

				if (spawnTimer >= pec->m_f_ParticleSpawnFrequency)
				{
					for (int i = 0; i < pec->m_i_ParticleSpawnDensity; ++i)
					{
						SpawnParticle(pec);

					}
					spawnTimer -= pec->m_f_ParticleSpawnFrequency;
				}
			}

			pec->UpdateTime(dt);
		}
		else
			m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(pec->m_po_OwnerEntity));
	}
	for (auto pc = m_po_ComponentManager->GetFirstComponentInstance
		<ParticleComponent>(Component::kComponentParticle);
		pc != nullptr;	pc = static_cast<ParticleComponent*>(pc->m_po_NextComponent))
	{
		if (pc->IsAlive())
		{
			pc->GetComponent<DrawComponent>()->
				SetAlpha(pc->GetAlphaValue());
			
			pc->UpdateTime(dt);
		}
		else
			m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(pc->m_po_OwnerEntity));
	}

}
void ParticleSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	CollisionGroupName collGroup1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName collGroup2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupPairing colPairing ={ collGroup1, collGroup2 };

	auto* tcp1 = eventData.object1->GetComponent<TransformComponent>();
	auto* tcp2 = eventData.object2->GetComponent<TransformComponent>();

	//Building collisions
	if (colPairing == CollisionSystem::m_vx_CollisionsPairings[2] ||
		 colPairing == CollisionSystem::m_vx_CollisionsPairings[5])
	{
		CreateParticleSpawner(tcp2, kParticleExplosion);
		CreateParticleSpawner(tcp2, kParticleBasicOneShot);
		return;
	}

	//Power up pickups
	if (colPairing == CollisionSystem::m_vx_CollisionsPairings[3] ||
		 colPairing == CollisionSystem::m_vx_CollisionsPairings[6])
	{
		auto particleSpawnerComp = CreateParticleSpawner(tcp1, kParticleSpark);

		if (tcp1->GetComponent<SnekHeadComponent>()->m_i_PlayerNumber == 1)
			particleSpawnerComp->m_x_ParticleEffectColor = {0.5f,0,0.1f,0.4f};

		auto text = CreateParticleSpawner(tcp1, kParticleText);

		AEGfxTexture* texture = nullptr;

		switch (tcp2->GetComponent<PowerUpHolderComponent>()->m_x_Type)
		{
			case kPowerUpSpring:
				texture = m_o_GraphicsSystem->FetchTexture("PowerUpTextBoost");
				break;
			case kPowerUpPlusBody:
				texture = m_o_GraphicsSystem->FetchTexture("PowerUpTextHealth");
				break;
			case kPowerUpStar:
				texture = m_o_GraphicsSystem->FetchTexture("PowerUpTextStar");
				break;
		}

		if(texture)
			text->SetPickUpText(texture);

		return;
	}

	//Snek battle collisions
	if (colPairing == CollisionSystem::m_vx_CollisionsPairings[0] ||
		colPairing == CollisionSystem::m_vx_CollisionsPairings[1] ||
		colPairing == CollisionSystem::m_vx_CollisionsPairings[4])
	{
		auto particleSpawnerComp = CreateParticleSpawner(tcp2, kParticleHit);
		//auto particleSpawnerComp = CreateParticleSpawner(tcp2, kParticleSparks);
		//particleSpawnerComp->GetSpawnTransform()->SetRotation(particleSpawnerComp->GetSpawnTransform()->GetRotation() + PI);
		//head x head
		if (colPairing == CollisionSystem::m_vx_CollisionsPairings[0])
		{
			particleSpawnerComp->m_f_ParticleSize *= 0.5f + ((tcp1->GetComponent<SnekHeadComponent>()->m_i_CurrentDamage +
																  tcp2->GetComponent<SnekHeadComponent>()->m_i_CurrentDamage) * 0.16f);
		}
		else //if head x body, take heads' damage
		{
			particleSpawnerComp->m_f_ParticleSize *= 0.5f + (tcp1->GetComponent<SnekHeadComponent>()->m_i_CurrentDamage * 0.33f);
		}
		return;
	}
}

bool ParticleSystem::CollisionCheckForParticleSystem(CollisionGroupName collisionGroupName1, TransformComponent* transComp_1,
	CollisionGroupName collisionGroupName2, TransformComponent* transComp_2, CollisionGroupName collGroupToCheck,
	TransformComponent** outTransformComponent)
{
	if (collisionGroupName1 == collGroupToCheck)
	{
		*outTransformComponent = transComp_1;
		//SpawnParticleEffect(transComp_1, particleType);
		return true;
	}
	else if (collisionGroupName2 == collGroupToCheck)
	{
		*outTransformComponent = transComp_2;
		//SpawnParticleEffect(transComp_2, particleType);
		return true;
	}
	else
		return false;
}

ParticleSpawnerComponent* ParticleSystem::CreateParticleSpawner(TransformComponent* spawnTransform, ParticleType particleType) const
{
	auto particleEffectEntity = m_po_EntityManager->NewEntity
		<ParticleSpawnerEntity>(Entity::kEntityParticleEffect, "ParticleEffect");

	auto particleEffectComponent = particleEffectEntity->GetComponent<ParticleSpawnerComponent>();

	if (particleEffectComponent)
	{
		particleEffectComponent->SetParticleType(particleType, m_o_GraphicsSystem);
		particleEffectComponent->SetSpawnTransform(spawnTransform);
	}
	return particleEffectComponent;
}

void ParticleSystem::SpawnParticle(ParticleSpawnerComponent* particleEffectComp)
{
	auto newParticleEntity = m_po_EntityManager->NewEntity<ParticleEntity>(Entity::kEntityParticle, "Particle");

	for (auto i_Component : newParticleEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentParticle)
		{
			auto particleComp = static_cast<ParticleComponent*>(i_Component);
			particleComp->SetParticleMaxLifetime(particleEffectComp->m_f_ParticleMaxLifetime);
			particleComp->m_b_AlphaScalingEnabled = particleEffectComp->m_b_AlphaScalingEnabled;
			particleComp->m_f_BaseAlpha = particleEffectComp->m_x_ParticleEffectColor.alpha;

		}
		else if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			auto transComp = static_cast<TransformComponent*>(i_Component);
			auto particleEffectTransform = particleEffectComp->GetSpawnTransform();

			transComp->SetRotation(CalculateRotation(particleEffectComp, particleEffectTransform));

			transComp->m_x_Position.x = (CalculatePositionX(particleEffectComp, particleEffectTransform));

			transComp->m_x_Position.y = (CalculatePositionY(particleEffectComp, particleEffectTransform));


			transComp->m_f_Scale = (particleEffectComp->m_f_ParticleSize);
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_Speed = particleEffectComp->m_f_ParticleSpeed;
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			auto drawComp = static_cast<DrawComponent*>(i_Component);
			m_o_GraphicsSystem->InitializeDrawComponent(drawComp, particleEffectComp->m_px_ParticleTexture, particleEffectComp->m_x_ParticleEffectColor
																		, particleEffectComp->m_i_SpriteCountX, particleEffectComp->m_i_SpriteCountY);

			drawComp->m_f_DrawPriority = particleEffectComp->m_i_ParticleDrawOrder;
		}
		else if (i_Component->m_x_ComponentID == kComponentAnimation)
		{
			auto animComp = static_cast<AnimationComponent*>(i_Component);

			auto maxStartFrame = particleEffectComp->m_i_RandomStartFrame;
			//if max start frame is >0, randomise it 
			if (maxStartFrame)
			{
				maxStartFrame = static_cast<int>(AERandFloat() * maxStartFrame) % maxStartFrame;
			}
			Animation anim(SpriteSheet{ particleEffectComp->m_px_ParticleTexture->mpName,particleEffectComp->m_i_SpriteCountX,
												 particleEffectComp->m_i_SpriteCountY}, maxStartFrame,
												 particleEffectComp->m_i_SpriteCountX * particleEffectComp->m_i_SpriteCountY);

			anim.m_f_SecondsPerFrame = particleEffectComp->m_i_SecondsPerFrame;
			animComp->m_vx_AnimationsList.push_back(anim);
			animComp->m_b_IsAnimating = true;
			animComp->m_i_CurrentAnimationId = 0;
			
		}
	}

}


float ParticleSystem::CalculateRotation(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp)
{
	if (particleEffectComp->m_b_ParticleFixDirection)
		return particleEffectComp->m_f_OffsetAngle;
	else
		return ((AERandFloat() - 0.5f) * particleEffectComp->m_f_SpreadAngle + particleEffectComp->m_f_OffsetAngle) + transformComp->GetRotation();
}

float ParticleSystem::CalculatePositionX(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp)
{
	if (particleEffectComp->m_b_ParticleFixDirection)
	{
		return (transformComp->m_x_Position.x + cos(particleEffectComp->m_f_AngleForOffsetDistance) * particleEffectComp->m_f_OffsetDistance);
	}
	else
	{
		float randFloat = AERandFloat();
		return (transformComp->m_x_Position.x + cos(transformComp->GetRotation() + particleEffectComp->m_f_AngleForOffsetDistance) *
			particleEffectComp->m_f_OffsetDistance + (particleEffectComp->GetSplitBool() ? particleEffectComp->GetCurrentSplitFactor() : randFloat) *
			particleEffectComp->m_f_SpreadDistance * cos(transformComp->GetRotation() + particleEffectComp->m_f_AngleForSpreadDistance) *
			((randFloat > 0.5f) ? 1.0f : -1.0f));
	}
}

float ParticleSystem::CalculatePositionY(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp)
{
	if (particleEffectComp->m_b_ParticleFixDirection)
	{
		return (transformComp->m_x_Position.y + sin(particleEffectComp->m_f_AngleForOffsetDistance) * particleEffectComp->m_f_OffsetDistance);
	}
	else
	{
		float randFloat = AERandFloat();
		return transformComp->m_x_Position.y + (sin(transformComp->GetRotation() + particleEffectComp->m_f_AngleForOffsetDistance) *
			particleEffectComp->m_f_OffsetDistance + (particleEffectComp->GetSplitBool() ? particleEffectComp->GetCurrentSplitFactor() : randFloat)*
			particleEffectComp->m_f_SpreadDistance * sin(transformComp->GetRotation() + particleEffectComp->m_f_AngleForSpreadDistance) *
			((randFloat > 0.5f) ? 1.0f : -1.0f));
	}
}
