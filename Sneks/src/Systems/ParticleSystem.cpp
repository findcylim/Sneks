
#include "ParticleSystem.h"
#include <random>

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

void ParticleSystem::receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->m_i_CollisionGroupVec[0] == 0 ||
		(eventData.object1->m_i_CollisionGroupVec[0] == 2))
	{
		PlayParticleEffect(static_cast<SnekHeadEntity*>(eventData.object1->m_po_OwnerEntity),
			ParticleType::kParticleBasicOneShot);
	}

	if (eventData.object2->m_i_CollisionGroupVec[0] == 0 ||
		(eventData.object2->m_i_CollisionGroupVec[0] == 2))
	{
		PlayParticleEffect(static_cast<SnekHeadEntity*>(eventData.object2->m_po_OwnerEntity),
			ParticleType::kParticleBasicOneShot);
	}
}

void ParticleSystem::PlayParticleEffect(SnekHeadEntity* owner, ParticleType type)
{
	auto pec = static_cast<ParticleEffectComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(Component::kComponentParticleEffect));

	pec->Revive();

	if (!pec->IsParticleEffectAlive())
		ChangeParticleEffect(owner, type);
}

void ParticleSystem::ChangeParticleEffect(SnekHeadEntity* owner, ParticleType type)
{
	auto pec = static_cast<ParticleEffectComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(Component::kComponentParticleEffect));

	pec->SetParticleType(type, m_o_GraphicsSystem);
}

void ParticleSystem::SpawnParticle(ParticleEffectComponent* pec)
{
	auto* pep = static_cast<ParticleEntity*>
		(m_po_EntityManager->NewEntity(Entity::kEntityParticle, "Particle"));

	static_cast<ParticleComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentParticle))->SetParticleMaxLifetime(
			pec->GetParticleMaxLifetime());

	auto* tcp = static_cast<TransformComponent*>(
		m_po_ComponentManager->GetSpecificComponentInstance(pec, Component::kComponentTransform));

	static_cast<TransformComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentTransform))->SetPosition(
			tcp->GetPosition().x, tcp->GetPosition().y);

	static_cast<TransformComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentTransform))->SetRotation(tcp->GetRotation());

	static_cast<DrawComponent*>(m_po_ComponentManager->GetSpecificComponentInstance(
		pep, Component::kComponentDraw))->Initialize(
			pec->GetParticleTexture(), pec->GetParticleSizeX(),
			pec->GetParticleSizeY(), HTColor{ 1,1,1,1 });
}
