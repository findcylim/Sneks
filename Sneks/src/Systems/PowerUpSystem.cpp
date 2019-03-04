
#include "PowerUpSystem.h"
#include "../Components/InvulnerableComponent.h"




PowerUpSystem::PowerUpSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics)
	: BaseSystem(entityManagerPointer)
{
	m_o_GraphicsSystem = graphics;
}

PowerUpSystem::~PowerUpSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}


void PowerUpSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this);
}

void PowerUpSystem::Update(float dt)
{
	for (auto powerupComponent = m_po_ComponentManager->GetFirstComponentInstance
		<PowerUpComponent>(Component::kComponentPowerUp);
		powerupComponent != nullptr;
		powerupComponent = static_cast<PowerUpComponent*>(powerupComponent->m_po_NextComponent))
	{
		if (powerupComponent->IsAlive())
		{
			powerupComponent->UpdateTime(dt);
		}
		else if (powerupComponent->GetJustDied())
		{
			RemovePowerUp(powerupComponent);
		}
	}
}

void PowerUpSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->m_i_CollisionGroupVec[0] == CollisionGroupName::kCollGroupBuilding)
		SpawnPowerUp(
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>
			(eventData.object1, Component::kComponentTransform),
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>
			(eventData.object2, Component::kComponentTransform));
	else if (eventData.object2->m_i_CollisionGroupVec[0] == CollisionGroupName::kCollGroupBuilding)
		SpawnPowerUp(
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>
			(eventData.object2, Component::kComponentTransform),
			m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>
			(eventData.object1, Component::kComponentTransform));

	else if (eventData.object1->m_i_CollisionGroupVec[0] == CollisionGroupName::kCollGroupPowerUp)
	{
		UpdatePowerUp(m_po_ComponentManager->GetSpecificComponentInstance<PowerUpComponent>
			(eventData.object2, Component::kComponentPowerUp));

		m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(
			eventData.object1->m_po_OwnerEntity));
	}
	else if (eventData.object2->m_i_CollisionGroupVec[0] == CollisionGroupName::kCollGroupPowerUp)
	{
		UpdatePowerUp(m_po_ComponentManager->GetSpecificComponentInstance<PowerUpComponent>
			(eventData.object1, Component::kComponentPowerUp));

		m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(
			eventData.object2->m_po_OwnerEntity));
	}
}

void PowerUpSystem::SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekVelocity)
{
	if (spawnPoint && snekVelocity && (AERandFloat() < m_f_SpawnChance))
	{
		auto powerupHolder = m_po_EntityManager->NewEntity
			<PowerUpHolderEntity>(Entity::kEntityPowerUpHolder, "PowerUpHolder");

		m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
			powerupHolder, Component::kComponentTransform)->SetPosition(
				spawnPoint->GetPosition().x, spawnPoint->GetPosition().y);

		m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
			powerupHolder, Component::kComponentTransform)->SetRotation(
				snekVelocity->GetRotation() + (AERandFloat() - 0.5f) * m_f_ForwardAngleRange);

		m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(
			powerupHolder, Component::kComponentTransform)->m_f_Scale = m_f_HolderSizeRatio;

		m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(
			powerupHolder, Component::kComponentPhysics)->m_f_Speed = 
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(
				snekVelocity, Component::kComponentPhysics)->m_f_Speed * m_f_HolderSpeedRatio;

		m_po_ComponentManager->GetSpecificComponentInstance<DrawComponent>(
			powerupHolder, Component::kComponentDraw)->Initialize(
				m_o_GraphicsSystem->FetchTexture("Moon"));

		m_po_ComponentManager->GetSpecificComponentInstance<CollisionComponent>(
			powerupHolder, Component::kComponentCollision)->m_i_CollisionGroupVec.push_back(
				CollisionGroupName::kCollGroupPowerUp);
	}
}

void PowerUpSystem::UpdatePowerUp(PowerUpComponent* powerup)
{
	PowerUpType type = kPowerUpInvul;//static_cast<PowerUpType>(rand() % kPowerUpEnd);

	if (powerup->IsAlive() || powerup->GetJustDied())
		RemovePowerUp(powerup);

	powerup->SetPowerUp(type);

	switch (type)
	{
		case kPowerUpSpeedIncrease:
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_Speed *= powerup->GetPowerIncrease();
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_MaxSpeed *= powerup->GetPowerIncrease();
			break;

		case kPowerUpGrowthIncrease:
			break;

		case kPowerUpUnlimitedSpecial:
			break;

		case kPowerUpInvul:
			m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>
				(powerup, KComponentInvulnerable)->m_f_InvulnerableTime = powerup->GetPowerIncrease();

			for (auto i_BodyParts : m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
				(powerup, kComponentSnekHead)->m_x_BodyParts)
				m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
					i_BodyParts, KComponentInvulnerable)->m_f_InvulnerableTime =
					powerup->GetPowerIncrease();
			break;

		case kPowerUpPlusBody:
			break;

		case kPowerUpEnd:
			break;
	}
}

void PowerUpSystem::RemovePowerUp(PowerUpComponent* powerup)
{
	switch (powerup->GetPowerUp())
	{
	case kPowerUpSpeedIncrease:
		m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
			Component::kComponentPhysics)->m_f_Speed /= powerup->GetPowerIncrease();
		m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
			Component::kComponentPhysics)->m_f_MaxSpeed /= powerup->GetPowerIncrease();
		break;

	case kPowerUpGrowthIncrease:
		break;

	case kPowerUpUnlimitedSpecial:
		break;

	case kPowerUpInvul:
		m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>
			(powerup, KComponentInvulnerable)->m_f_InvulnerableTime = 0;

		for (auto i_BodyParts : m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
			(powerup, kComponentSnekHead)->m_x_BodyParts)
			m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
				i_BodyParts, KComponentInvulnerable)->m_f_InvulnerableTime = 0;
		break;
	}

	powerup->SetPowerUp(kPowerUpEnd);
}







