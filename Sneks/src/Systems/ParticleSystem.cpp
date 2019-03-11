
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
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void ParticleSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this,this);
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
	CollisionGroupName cgn1 = eventData.object1->m_i_CollisionGroupVec[0];
	CollisionGroupName cgn2 = eventData.object2->m_i_CollisionGroupVec[0];
	CollisionGroupName cgnc;
	ParticleType type;

	TransformComponent* tcp1 = eventData.object1->GetComponent<TransformComponent>();
	//	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
	//		eventData.object1, Component::kComponentTransform);
	TransformComponent* tcp2 = eventData.object2->GetComponent<TransformComponent>();
	//	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
	//		eventData.object2, Component::kComponentTransform);

	cgnc = CollisionGroupName::kCollGroupBuilding;
	type = ParticleType::kParticleBasicOneShot;

	if (!CollisionCheckForParticleSystem(cgn1, tcp1, cgn2, tcp2, cgnc, type))
	{
		cgnc = CollisionGroupName::kCollGroupSnek1Body;
		type = ParticleType::kParticleBasicOneShot;
	}
	else if (!CollisionCheckForParticleSystem(cgn1, tcp1, cgn2, tcp2, cgnc, type))
	{
		cgnc = CollisionGroupName::kCollGroupSnek2Body;
		type = ParticleType::kParticleBasicOneShot;
	}
	else if (!CollisionCheckForParticleSystem(cgn1, tcp1, cgn2, tcp2, cgnc, type))
	{
	}
}

bool ParticleSystem::CollisionCheckForParticleSystem(CollisionGroupName name1, TransformComponent* spawn1,
	CollisionGroupName name2, TransformComponent* spawn2, CollisionGroupName namecheck, ParticleType type)
{
	if (name1 == namecheck)
	{
		SpawnParticleEffect(spawn1, type);
		return true;
	}
	else if (name2 == namecheck)
	{
		SpawnParticleEffect(spawn2, type);
		return true;
	}
	else
		return false;
}

void ParticleSystem::SpawnParticleEffect(TransformComponent* spawnTransform, ParticleType type)
{
	if (spawnTransform)
	{
		auto pee = m_po_EntityManager->NewEntity
			<ParticleEffectEntity>(Entity::kEntityParticleEffect, "ParticleEffect");

		auto pec = pee->GetComponent<ParticleEffectComponent>();
			//m_po_ComponentManager->GetSpecificComponentInstance<ParticleEffectComponent>(
			//	pee, Component::kComponentParticleEffect);

		if (pec)
		{
			pec->SetParticleType(type, m_o_GraphicsSystem);
			pec->SetSpawnTransform(spawnTransform);
		}
	}
}

void ParticleSystem::SpawnParticle(ParticleEffectComponent* pec)
{
	auto pep = m_po_EntityManager->NewEntity<ParticleEntity>(Entity::kEntityParticle, "Particle");

	m_po_ComponentManager->GetSpecificComponentInstance<ParticleComponent>(
		pep, Component::kComponentParticle)->SetParticleMaxLifetime(
			pec->GetParticleMaxLifetime());

	auto tcp = pec->GetSpawnTransform();

	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
		pep, Component::kComponentTransform)->SetPositionX(
			CalculatePositionX(pec, tcp));

	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
		pep, Component::kComponentTransform)->SetPositionY(
			CalculatePositionY(pec, tcp));

	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
		pep, Component::kComponentTransform)->SetRotation(CalculateRotation(pec, tcp));

	m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
		pep, Component::kComponentTransform)->SetScale(pec->GetParticleSize());

	m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(
		pep, Component::kComponentPhysics)->m_f_Speed = pec->GetParticleSpeed();

	m_o_GraphicsSystem->InitializeDrawComponent(m_po_ComponentManager->
		GetSpecificComponentInstance<DrawComponent>(pep, Component::kComponentDraw),
		pec->GetParticleTexture());

	m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
		pep, Component::kComponentDraw)->m_f_DrawPriority = pec->GetParticleDrawOrder();
}

float ParticleSystem::CalculateRotation(ParticleEffectComponent* pec, TransformComponent* tcp)
{
	return ((AERandFloat() - 0.5f) * pec->GetSpreadAngle() + pec->GetOffsetAngle()) + tcp->GetRotation();
}

float ParticleSystem::CalculatePositionX(ParticleEffectComponent* pec, TransformComponent* tcp)
{
	float randfloat = AERandFloat();
	return (tcp->GetPosition().x + cos(tcp->GetRotation() + pec->GetAngleForOffsetDistance()) * 
		pec->GetOffsetDistance() + (pec->GetSplitBool() ? pec->GetCurrentSplitFactor() : randfloat) *
		pec->GetSpreadDistance() * cos(tcp->GetRotation() + pec->GetAngleForSpreadDistance()) *
		((randfloat > 0.5f) ? 1.0f : -1.0f));
}

float ParticleSystem::CalculatePositionY(ParticleEffectComponent* pec, TransformComponent* tcp)
{
	float randfloat = AERandFloat();
	return tcp->GetPosition().y + (sin(tcp->GetRotation() + pec->GetAngleForOffsetDistance()) *
		pec->GetOffsetDistance() + (pec->GetSplitBool() ? pec->GetCurrentSplitFactor() : randfloat)*
		pec->GetSpreadDistance() * sin(tcp->GetRotation() + pec->GetAngleForSpreadDistance()) *
		((randfloat > 0.5f) ? 1.0f : -1.0f));
}
