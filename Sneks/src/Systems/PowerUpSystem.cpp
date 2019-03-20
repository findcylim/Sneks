
#include "PowerUpSystem.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/BloomComponent.h"


PowerUpSystem::PowerUpSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics, SnekSystem* snek)
	: BaseSystem(entityManagerPointer)
{
	m_po_GraphicsSystem = graphics;
	m_po_SnekSystem = snek;
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
			(eventData.object2, Component::kComponentPowerUp), eventData.object1->GetComponent<DrawComponent>());

		m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(
			eventData.object1->m_po_OwnerEntity));
	}
	else if (eventData.object2->m_i_CollisionGroupVec[0] == CollisionGroupName::kCollGroupPowerUp)
	{
		UpdatePowerUp(m_po_ComponentManager->GetSpecificComponentInstance<PowerUpComponent>
			(eventData.object1, Component::kComponentPowerUp), eventData.object2->GetComponent<DrawComponent>());

		m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(
			eventData.object2->m_po_OwnerEntity));
	}
}

void PowerUpSystem::SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekTransform)
{
	if (spawnPoint && snekTransform && (AERandFloat() < m_f_SpawnChance))
	{
		auto powerupHolder = m_po_EntityManager->NewEntity
			<PowerUpHolderEntity>(Entity::kEntityPowerUpHolder, "PowerUpHolder");

		auto transformComponent = m_po_ComponentManager->GetSpecificComponentInstance
			<TransformComponent>(powerupHolder, Component::kComponentTransform);

		transformComponent->SetPositionX(spawnPoint->GetPosition().x);

		transformComponent->SetPositionY(spawnPoint->GetPosition().y);

		transformComponent->SetRotation(snekTransform->GetRotation() +
			(AERandFloat() - 0.5f) * m_f_ForwardAngleRange);

		transformComponent->m_f_Scale = m_f_HolderSizeRatio;

		powerupHolder->GetComponent<PhysicsComponent>()->m_f_Speed = 
			snekTransform->GetComponent<PhysicsComponent>()->m_f_Speed * m_f_HolderSpeedRatio;

		powerupHolder->GetComponent<PowerUpComponent>();
		auto type = static_cast<PowerUpType>(rand() % kPowerUpEnd);

		const char * PowerUpIcon = "PowerUpIcon";

		switch (type)
		{
		case kPowerUpSpeedIncrease:
			PowerUpIcon = "PowerUpIconSpeed";
			break;
		case kPowerUpInvul:
			PowerUpIcon = "PowerUpIconInvul";
			break;
		case kPowerUpPlusBody:
			PowerUpIcon = "PowerUpIconHealth";
			break;
		case kPowerUpIncreaseDamage:
			PowerUpIcon = "PowerUpIconDamage";
			break;
			//TODO Add icon for growth. 
		}

		m_po_GraphicsSystem->InitializeDrawComponent(m_po_ComponentManager->
			GetSpecificComponentInstance<DrawComponent>(powerupHolder, Component::kComponentDraw),
			PowerUpIcon);

		m_po_ComponentManager->GetSpecificComponentInstance<CollisionComponent>(
			powerupHolder, Component::kComponentCollision)->m_i_CollisionGroupVec.push_back(
				CollisionGroupName::kCollGroupPowerUp);

		m_po_ComponentManager->GetSpecificComponentInstance<InvulnerableComponent>(
			powerupHolder, Component::KComponentInvulnerable)->m_f_InvulnerableTime =
			m_f_HolderInvulTime;


		m_po_ComponentManager->GetSpecificComponentInstance<BloomComponent>(
			powerupHolder, Component::kComponentBloom)->m_b_FlashingBloom = true;

	}
}

void PowerUpSystem::UpdatePowerUp(PowerUpComponent* powerup,DrawComponent* powerUpDrawComponent)
{
	PowerUpType type;
	
	//TODO
	// GET A WAY TO GET THE NAME OF THE TEXTURE SIMPLER
	if (strcmp(powerUpDrawComponent->m_px_Texture->mpName, "../Resources/PowerUpIconDamage.png")==0)
	{
		type = kPowerUpIncreaseDamage;
	}
	else if (strcmp(powerUpDrawComponent->m_px_Texture->mpName, "../Resources/PowerUpIconSpeed.png") == 0)
	{
		type = kPowerUpSpeedIncrease;
	}
	else if (strcmp(powerUpDrawComponent->m_px_Texture->mpName, "../Resources/PowerUpIconInvul.png") == 0)
	{
		type = kPowerUpInvul;
	}
	else if(strcmp(powerUpDrawComponent->m_px_Texture->mpName, "../Resources/PowerUpIconHealth.png") == 0)
	{
		type = kPowerUpPlusBody;
	}
	else
	{
		type = kPowerUpGrowthIncrease;
	}

	if (powerup->IsAlive() || powerup->GetJustDied())
		RemovePowerUp(powerup);

	powerup->SetPowerUp(type);

	auto snekHeadComponent = powerup->GetComponent<SnekHeadComponent>();
	switch (type)
	{
		case kPowerUpSpeedIncrease:
		{
			snekHeadComponent->m_f_AccelerationForce *= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MinSpeed *= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MaxVelocity *= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
			m_po_SnekSystem->TweakGrowthRate(snekHeadComponent, powerup->GetPowerIncrease());
		}
			break;

		case kPowerUpUnlimitedSpecial:
		{
		}
			break;

		case kPowerUpInvul:
		{
			//auto invulComponent = powerup->GetComponent<InvulnerableComponent>();
			//invulComponent->m_f_InvulnerableTime = powerup->GetPowerIncrease();

			//for (auto& i_BodyParts : snekHeadComponent->m_x_BodyParts) 
			//{
			//	invulComponent->m_f_InvulnerableTime =	powerup->GetPowerIncrease();
			//}
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

			for (int i = 0; i < powerup->GetPowerIncrease(); i++)
				m_po_SnekSystem->CreateSnekBody(static_cast<SnekHeadEntity*>(powerup->m_po_OwnerEntity),
					bodyTexture, snekHeadComponent->m_i_PlayerNumber);
		}
			break;

		case kPowerUpIncreaseDamage:
			m_po_SnekSystem->TweakPlayerDamage(snekHeadComponent, static_cast<int>(powerup->GetPowerIncrease()));

			break;
		case kPowerUpEnd:
			break;
	}
}

void PowerUpSystem::RemovePowerUp(PowerUpComponent* powerup)
{
	auto snekHeadComponent = powerup->GetComponent<SnekHeadComponent>();
	switch (powerup->GetPowerUp())
	{
		case kPowerUpSpeedIncrease:
		{
			snekHeadComponent->m_f_AccelerationForce /= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MinSpeed /= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MaxVelocity /= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
			m_po_SnekSystem->TweakGrowthRate(snekHeadComponent, powerup->GetPowerIncrease());
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







