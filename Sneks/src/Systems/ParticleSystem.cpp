
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
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this);
}

void ParticleSystem::Update(float dt)
{
	for (auto pec = static_cast<ParticleEffectComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(Component::kComponentParticleEffect));
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
				for (float iter = dt; iter > spawnperframe; iter -= spawnperframe);
				SpawnParticle(pec);
			}

			pec->UpdateTime(dt);
		}
	}

	for (auto pc = static_cast<ParticleComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(Component::kComponentParticle));
		pc != nullptr;	pc = static_cast<ParticleComponent*>(pc->m_po_NextComponent))
	{
		if (pc->IsAlive())
		{
			static_cast<DrawComponent*>(m_po_ComponentManager->GetSpecificComponentInstance
			(pc, Component::kComponentDraw))->SetAlpha(pc->GetAlphaValue());
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

	TransformComponent* tcp1 = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			eventData.object1, Component::kComponentTransform));
	TransformComponent* tcp2 = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(
			eventData.object2, Component::kComponentTransform));

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
		auto pee = static_cast<ParticleEffectEntity*>
			(m_po_EntityManager->NewEntity(Entity::kEntityParticleEffect, "ParticleEffect"));

		auto pec = static_cast<ParticleEffectComponent*>(
			m_po_ComponentManager->GetSpecificComponentInstance(
				pee, Component::kComponentParticleEffect));

		if (pec)
		{
			pec->SetParticleType(type, m_o_GraphicsSystem);
			pec->SetSpawnTransform(spawnTransform);
		}
	}
}

void ParticleSystem::SpawnParticle(ParticleEffectComponent* pec)
{
	auto pep = static_cast<ParticleEntity*>
		(m_po_EntityManager->NewEntity(Entity::kEntityParticle, "Particle"));

	static_cast<ParticleComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentParticle))->SetParticleMaxLifetime(
			pec->GetParticleMaxLifetime());

	auto tcp = pec->GetSpawnTransform();

	static_cast<TransformComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentTransform))->SetPosition(
			tcp->GetPosition().x, tcp->GetPosition().y);

	static_cast<TransformComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentTransform))->SetRotation(static_cast<float>(rand() % 360));

	static_cast<PhysicsComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentPhysics))->m_f_Speed = pec->GetParticleSpeed();

	static_cast<DrawComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentDraw))->Initialize(
			pec->GetParticleTexture(), pec->GetParticleSizeX(), pec->GetParticleSizeY(), HTColor{ 1,1,1,1 });
}
