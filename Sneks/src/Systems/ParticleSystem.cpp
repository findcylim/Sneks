
#include "ParticleSystem.h"
#include <random>
#include <iostream>

ParticleSystem::ParticleSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics)
	: BaseSystem(entityManagerPointer)
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

void ParticleSystem::Update(float dt)
{
	for (auto pec = m_po_ComponentManager->GetFirstComponentInstance
		<ParticleEffectComponent>(Component::kComponentParticleEffect);
		pec != nullptr;	pec = static_cast<ParticleEffectComponent*>(pec->m_po_NextComponent))
	{
		if (pec->IsParticleEffectAlive())
		{
			if (pec->GetIsParticleEffectOneShot())
			{
				int density = pec->GetParticleSpawnDensity();
				for (int iter = 0; iter < density; iter++)
					SpawnParticle(pec);
			}
			else
			{
				float spawnperframe = 
					pec->GetParticleSpawnFrequency() / pec->GetParticleSpawnDensity();
				float& iter = pec->GetParticleLeftoverTime();
				for (iter += dt; iter > spawnperframe; iter -= spawnperframe)
					SpawnParticle(pec);
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
	CollisionGroupName collGroupActual;
	ParticleType particleType;

	TransformComponent* tcp1 = eventData.object1->GetComponent<TransformComponent>();

	TransformComponent* tcp2 = eventData.object2->GetComponent<TransformComponent>();


	collGroupActual = CollisionGroupName::kCollGroupBuilding;
	particleType = ParticleType::kParticleBasicOneShot;

	if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek1Body;
		particleType = ParticleType::kParticleBasicOneShot;
	}
	else 
		return;
	if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek2Body;
		particleType = ParticleType::kParticleLargeOneShot;
	}
	else
		return;
	if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek1Head;
		particleType = ParticleType::kParticleLargeOneShot;
	}
	else 
		return;
	if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		//collGroupActual = CollisionGroupName::kCollGroupSnek2Head;
		//particleType = ParticleType::kParticleLargeOneShot;
	}
	else 
		return;

	/*if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek1Body;
		particleType = ParticleType::kParticleBasicOneShot;
	}
	else if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek2Body;
		particleType = ParticleType::kParticleBasicOneShot;
	}
	else if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek1Head;
		particleType = ParticleType::kParticleBasicOneShot;
	}
	else if (!CollisionCheckForParticleSystem(collGroup1, tcp1, collGroup2, tcp2, collGroupActual, particleType))
	{
		collGroupActual = CollisionGroupName::kCollGroupSnek2Head;
		particleType = ParticleType::kParticleBasicOneShot;
	}*/
}

bool ParticleSystem::CollisionCheckForParticleSystem(CollisionGroupName name1, TransformComponent* transComp_1,
	CollisionGroupName collisionGroupName1, TransformComponent* transComp_2, CollisionGroupName collisionGroupName2, ParticleType particleType)
{
	if (name1 == collisionGroupName2)
	{
		SpawnParticleEffect(transComp_1, particleType);
		return true;
	}
	else if (collisionGroupName1 == collisionGroupName2)
	{
		SpawnParticleEffect(transComp_2, particleType);
		return true;
	}
	else
		return false;
}

void ParticleSystem::SpawnParticleEffect(TransformComponent* spawnTransform, ParticleType particleType)
{
	if (spawnTransform)
	{
		auto particleEffectEntity = m_po_EntityManager->NewEntity
			<ParticleEffectEntity>(Entity::kEntityParticleEffect, "ParticleEffect");

		auto particleEffectComponent = particleEffectEntity->GetComponent<ParticleEffectComponent>();

		if (particleEffectComponent)
		{
			particleEffectComponent->SetParticleType(particleType, m_o_GraphicsSystem);
			particleEffectComponent->SetSpawnTransform(spawnTransform);
		}
	}
}

void ParticleSystem::SpawnParticle(ParticleEffectComponent* particleEffectComp)
{
	auto newParticleEntity = m_po_EntityManager->NewEntity<ParticleEntity>(Entity::kEntityParticle, "Particle");

	for (auto i_Component : newParticleEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentParticle)
		{
			static_cast<ParticleComponent*>(i_Component)->SetParticleMaxLifetime(
																		  particleEffectComp->GetParticleMaxLifetime());
		}
		else if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			auto transComp = static_cast<TransformComponent*>(i_Component);
			auto particleEffectTransform = particleEffectComp->GetSpawnTransform();

			transComp->SetPositionX(CalculatePositionX(particleEffectComp, particleEffectTransform));

			transComp->SetPositionY(CalculatePositionY(particleEffectComp, particleEffectTransform));

			transComp->SetRotation(CalculateRotation(particleEffectComp, particleEffectTransform));

			transComp->SetScale(particleEffectComp->GetParticleSize());
		}
		else if (i_Component->m_x_ComponentID == kComponentPhysics)
		{
			static_cast<PhysicsComponent*>(i_Component)->m_f_Speed = particleEffectComp->GetParticleSpeed();
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			auto drawComp = static_cast<DrawComponent*>(i_Component);
			m_o_GraphicsSystem->InitializeDrawComponent(drawComp, particleEffectComp->GetParticleTexture(),HTColor{1,1,1,1}
																		,4,7);

			drawComp->m_f_DrawPriority = particleEffectComp->GetParticleDrawOrder();

		}
		else if (i_Component->m_x_ComponentID == kComponentAnimation)
		{
			auto animComp = static_cast<AnimationComponent*>(i_Component);
			int randRock = static_cast<int>(AERandFloat() * 28) % 28;
			Animation anim(SpriteSheet{ particleEffectComp->GetParticleTexture()->mpName,4,7}, randRock, 4 * 7);

			anim.m_f_SecondsPerFrame = 10.0f;
			animComp->m_vx_AnimationsList.push_back(anim);
			animComp->m_b_IsAnimating = true;
			animComp->m_i_CurrentAnimationId = 0;
			
		}
	}

}

float ParticleSystem::CalculateRotation(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp)
{
	return ((AERandFloat() - 0.5f) * particleEffectComp->GetSpreadAngle() + particleEffectComp->GetOffsetAngle()) + transformComp->GetRotation();
}

float ParticleSystem::CalculatePositionX(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp)
{
	float randFloat = AERandFloat();
	return (transformComp->GetPosition().x + cos(transformComp->GetRotation() + particleEffectComp->GetAngleForOffsetDistance()) * 
		particleEffectComp->GetOffsetDistance() + (particleEffectComp->GetSplitBool() ? particleEffectComp->GetCurrentSplitFactor() : randFloat) *
		particleEffectComp->GetSpreadDistance() * cos(transformComp->GetRotation() + particleEffectComp->GetAngleForSpreadDistance()) *
		((randFloat > 0.5f) ? 1.0f : -1.0f));
}

float ParticleSystem::CalculatePositionY(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp)
{
	float randFloat = AERandFloat();
	return transformComp->GetPosition().y + (sin(transformComp->GetRotation() + particleEffectComp->GetAngleForOffsetDistance()) *
		particleEffectComp->GetOffsetDistance() + (particleEffectComp->GetSplitBool() ? particleEffectComp->GetCurrentSplitFactor() : randFloat)*
		particleEffectComp->GetSpreadDistance() * sin(transformComp->GetRotation() + particleEffectComp->GetAngleForSpreadDistance()) *
		((randFloat > 0.5f) ? 1.0f : -1.0f));
}
