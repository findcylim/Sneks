
#include "PowerUpSystem.h"
#include "../Components/InvulnerableComponent.h"




PowerUpSystem::PowerUpSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics, SnekSystem* snek)
	: BaseSystem(entityManagerPointer)
{
	m_o_GraphicsSystem = graphics;
	m_o_SnekSystem = snek;
}

PowerUpSystem::~PowerUpSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}


void PowerUpSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
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

		auto transformComponent = m_po_ComponentManager->GetSpecificComponentInstance
			<TransformComponent>(powerupHolder, Component::kComponentTransform);

		transformComponent->SetPositionX(spawnPoint->GetPosition().x);

		transformComponent->SetPositionY(spawnPoint->GetPosition().y);

		transformComponent->SetRotation(snekVelocity->GetRotation() +
			(AERandFloat() - 0.5f) * m_f_ForwardAngleRange);

		transformComponent->m_f_Scale = m_f_HolderSizeRatio;

		m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(
			powerupHolder, Component::kComponentPhysics)->m_f_Speed = 
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(
				snekVelocity, Component::kComponentPhysics)->m_f_Speed * m_f_HolderSpeedRatio;

		m_o_GraphicsSystem->InitializeDrawComponent(m_po_ComponentManager->
			GetSpecificComponentInstance<DrawComponent>(powerupHolder, Component::kComponentDraw),
			"PowerUpIcon");

		m_po_ComponentManager->GetSpecificComponentInstance<CollisionComponent>(
			powerupHolder, Component::kComponentCollision)->m_i_CollisionGroupVec.push_back(
				CollisionGroupName::kCollGroupPowerUp);

		m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
			powerupHolder, Component::KComponentInvulnerable)->m_f_InvulnerableTime =
			m_f_HolderInvulTime;
	}
}

void PowerUpSystem::UpdatePowerUp(PowerUpComponent* powerup)
{
	PowerUpType type = static_cast<PowerUpType>(rand() % kPowerUpEnd);

	if (powerup->IsAlive() || powerup->GetJustDied())
		RemovePowerUp(powerup);

	powerup->SetPowerUp(type);

	switch (type)
	{
		case kPowerUpSpeedIncrease:
		{
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_Speed *= powerup->GetPowerIncrease();
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_MaxSpeed *= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
			m_o_SnekSystem->IncreaseGrowthRate(m_po_ComponentManager->GetSpecificComponentInstance
				<SnekHeadComponent>(powerup, kComponentSnekHead)->m_i_PlayerNumber,
				powerup->GetPowerIncrease());
		}
			break;

		case kPowerUpUnlimitedSpecial:
		{
		}
			break;

		case kPowerUpInvul:
		{
			m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>
				(powerup, KComponentInvulnerable)->m_f_InvulnerableTime = powerup->GetPowerIncrease();

			for (auto i_BodyParts : m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
				(powerup, kComponentSnekHead)->m_x_BodyParts)
				m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
					i_BodyParts, KComponentInvulnerable)->m_f_InvulnerableTime =
					powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpPlusBody:
		{
			//TODO emit event to snek system to increase body part
			//TODO snek system to recieve event to grow body part
			const char* bodyTexture = nullptr;
			if (m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
				(powerup, kComponentSnekHead)->m_i_PlayerNumber == 0)
				bodyTexture = "SnekBody01";
			else
				bodyTexture = "SnekBody02";

			for (int i = 0; i < 5; i++)
				m_o_SnekSystem->CreateSnekBody(static_cast<SnekHeadEntity*>(powerup->m_po_OwnerEntity),
					bodyTexture, m_po_ComponentManager->GetSpecificComponentInstance
					<SnekHeadComponent>(powerup, kComponentSnekHead)->m_i_PlayerNumber);
		}
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
		{
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_Speed /= powerup->GetPowerIncrease();
			m_po_ComponentManager->GetSpecificComponentInstance<PhysicsComponent>(powerup,
				Component::kComponentPhysics)->m_f_MaxSpeed /= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
			m_o_SnekSystem->DecreaseGrowthRate(m_po_ComponentManager->GetSpecificComponentInstance
				<SnekHeadComponent>(powerup, kComponentSnekHead)->m_i_PlayerNumber,
				powerup->GetPowerIncrease());
		}
			break;

		case kPowerUpUnlimitedSpecial:
		{
		}
			break;

		case kPowerUpInvul:
		{
			m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>
				(powerup, KComponentInvulnerable)->m_f_InvulnerableTime = 0;

			for (auto i_BodyParts : m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
				(powerup, kComponentSnekHead)->m_x_BodyParts)
				m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
					i_BodyParts, KComponentInvulnerable)->m_f_InvulnerableTime = 0;
		}
			break;
	}

	powerup->SetPowerUp(kPowerUpEnd);
}







