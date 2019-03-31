
#include "PowerUpSystem.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/BloomComponent.h"
#include "../Components/PowerUpHolderComponent.h"


PowerUpSystem::PowerUpSystem(GraphicsSystem* graphics)
{
	m_po_GraphicsSystem = graphics;
}

PowerUpSystem::~PowerUpSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}


void PowerUpSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);
}

HTColor PowerUpSystem::StarColorChange(HTColor& color) const
{
//	float* colorToChange = nullptr;
	HTColor newColor = {1.5f, 1.5f, 1.5f, 1.0f};
	newColor.alpha = color.alpha;
	do {
		int randInt = static_cast<int>(AERandFloat() * 3);
		if (randInt == 0)
		{
			newColor.red = 0;
			/*colorToChange = &color.red;*/
		}
		else if (randInt == 1)
		{
			newColor.green = 0;
		}
		else if (randInt == 2)
		{
			newColor.blue = 0;
		}
	} while (newColor.red == color.red &&
				newColor.green == color.green &&
				newColor.blue == color.blue);

	color = newColor;
	return newColor;
}

void PowerUpSystem::Update(float dt)
{
	m_po_ComponentManager->Each<PowerUpComponent>([&](PowerUpComponent* powerUpComponent)
	{
		if (powerUpComponent->IsActive())
		{
			switch (powerUpComponent->m_x_PowerUpType)
			{
			case kPowerUpSpeedIncrease: break;
			case kPowerUpGrowthIncrease: break;
			case kPowerUpUnlimitedSpecial: break;
			case kPowerUpStar:
			{
				auto snekDraw = powerUpComponent->GetComponent<DrawComponent>();
				auto snekHeadComponent = powerUpComponent->GetComponent<SnekHeadComponent>();
				//float timer = 0;
				powerUpComponent->m_f_SpecialTimer += dt * (0.5f + (powerUpComponent->m_f_PowerUpDurationLeft / 10));
				if (powerUpComponent->m_f_SpecialTimer >= 0.2f)
				{
					auto newColor = StarColorChange(snekDraw->m_f_RgbaColor);
					powerUpComponent->m_f_SpecialTimer = 0;
					for (auto& i_BodyParts : snekHeadComponent->m_x_BodyParts)
					{
						auto bodyDraw = i_BodyParts->GetComponent<DrawComponent>();
						bodyDraw->m_f_RgbaColor = newColor;
					}
				}
				break;
			}
			case kPowerUpPlusBody: break;
			case kPowerUpIncreaseDamage: break;
			default:
				break;
			}

			powerUpComponent->m_f_PowerUpDurationLeft -= dt;
			if (powerUpComponent->m_f_PowerUpDurationLeft <= 0)
			{
				PowerUpExpire(powerUpComponent);
			}
		}
		else if (powerUpComponent->Expired())
		{
			PowerUpExpire(powerUpComponent);
		}

	}, kComponentPowerUp, true);
}

void PowerUpSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	auto obj2 = eventData.object2;
	auto obj1 = eventData.object1;

	if (obj2->m_i_CollisionGroupVec[0] == kCollGroupBuilding)
		SpawnPowerUp(obj2->GetComponent<TransformComponent>(),
						 obj1->GetComponent<TransformComponent>());
	else if (obj2->m_i_CollisionGroupVec[0] == kCollGroupPowerUp)
	{
		PowerUpPickup(obj1->GetComponent<PowerUpComponent>(), obj2->GetComponent<PowerUpHolderComponent>());

		m_po_EntityManager->AddToDeleteQueue(obj2->m_po_OwnerEntity);
	}
}

void PowerUpSystem::SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekTransform)
{
	if (spawnPoint && snekTransform && (AERandFloat() < m_f_SpawnChance))
	{
		auto powerupHolder = m_po_EntityManager->NewEntity
			<PowerUpHolderEntity>(Entity::kEntityPowerUpHolder, "PowerUpHolder");

		auto transformComponent = powerupHolder->GetComponent<TransformComponent>();

		transformComponent->m_x_Position.x = (spawnPoint->m_x_Position.x);

		transformComponent->m_x_Position.y = (spawnPoint->m_x_Position.y);

		transformComponent->SetRotation(snekTransform->GetRotation() +
													 (AERandFloat() - 0.5f) * m_f_ForwardAngleRange);

		transformComponent->m_f_Scale = m_f_HolderSizeRatio;

		powerupHolder->GetComponent<PhysicsComponent>()->m_f_Speed = 
			snekTransform->GetComponent<PhysicsComponent>()->m_f_Speed * m_f_HolderSpeedRatio;

		auto powerUpComp = powerupHolder->GetComponent<PowerUpHolderComponent>();

		powerUpComp->m_x_Type = static_cast<PowerUpType>(rand() % kPowerUpEnd);
		//MARK
		powerUpComp->m_x_Type = kPowerUpStar;
		const char * texture = "PowerUpIcon";

		switch (powerUpComp->m_x_Type)
		{
		case kPowerUpSpeedIncrease:
			texture = "PowerUpIconSpeed";
			break;
		case kPowerUpStar:
			texture = "PowerUpIconInvul";
			break;
		case kPowerUpPlusBody:
			texture = "PowerUpIconHealth";
			break;
		case kPowerUpIncreaseDamage:
			texture = "PowerUpIconDamage";
			break;
		}

		m_po_GraphicsSystem->InitializeDrawComponent(powerupHolder->GetComponent<DrawComponent>(),
																	texture);

		powerupHolder->GetComponent<CollisionComponent>()->m_i_CollisionGroupVec.push_back(
				CollisionGroupName::kCollGroupPowerUp);

		powerupHolder->GetComponent<InvulnerableComponent>()->m_f_InvulnerableTime = m_f_HolderInvulTime;

		powerupHolder->GetComponent<BloomComponent>()->m_b_FlashingBloom = true;

	}
}

void PowerUpSystem::PowerUpPickup(PowerUpComponent* powerUp, PowerUpHolderComponent* powerUpHolder)
{
	//Expire the last powerUp
	if (powerUp->IsActive() || powerUp->Expired())
		PowerUpExpire(powerUp);
	powerUp->m_b_JustExpired = false;
	powerUp->SetPowerUp(powerUpHolder->m_x_Type);
	auto snekHeadComponent = powerUp->GetComponent<SnekHeadComponent>();
	switch (powerUpHolder->m_x_Type)
	{
		case kPowerUpSpeedIncrease:
		{
			snekHeadComponent->m_f_AccelerationForce *= powerUp->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MinSpeed *= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MaxVelocity *= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
		}
			break;

		case kPowerUpUnlimitedSpecial:
		{
		}
			break;

		case kPowerUpStar:
		{
		}
			break;

		//case kPowerUpPlusBody:
		//{
		//	//TODO emit event to snek system to increase body part
		//	//TODO snek system to recieve event to grow body part
		//	const char* bodyTexture = nullptr;
		//	if (m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>
		//		(powerUp, kComponentSnekHead)->m_i_PlayerNumber == 0)
		//		bodyTexture = "SnekBody01";
		//	else
		//		bodyTexture = "SnekBody02";

		//	for (int i = 0; i < powerUp->GetPowerIncrease(); i++)
		//		m_po_SnekSystem->CreateSnekBody(static_cast<SnekHeadEntity*>(powerUp->m_po_OwnerEntity),
		//			bodyTexture, snekHeadComponent->m_i_PlayerNumber);
		//}
		//	break;

		case kPowerUpIncreaseDamage:
			//m_po_SnekSystem->TweakPlayerDamage(snekHeadComponent, static_cast<int>(powerup->GetPowerIncrease()));

			break;
		case kPowerUpEnd:
			break;
	}
}

void PowerUpSystem::PowerUpExpire(PowerUpComponent* powerUp) const
{
	powerUp->m_f_PowerUpDurationLeft = 0.0f;
	auto snekHeadComponent = powerUp->GetComponent<SnekHeadComponent>();
	switch (powerUp->GetPowerUp())
	{
		case kPowerUpSpeedIncrease:
		{
			snekHeadComponent->m_f_AccelerationForce /= powerUp->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MinSpeed /= powerup->GetPowerIncrease();
			//m_po_ComponentManager->GetSpecificComponentInstance<SnekHeadComponent>(powerup,
			//	Component::kComponentSnekHead)->m_f_MaxVelocity /= powerup->GetPowerIncrease();
		}
			break;

		case kPowerUpGrowthIncrease:
		{
		}
			break;

		case kPowerUpUnlimitedSpecial:
		{
		}
			break;

		case kPowerUpStar:
		{
			snekHeadComponent->GetComponent<DrawComponent>()->m_f_RgbaColor ={ 1.0f,1.0f,1.0f,1.0f };
			for (auto& i_BodyParts : snekHeadComponent->m_x_BodyParts)
			{
				auto bodyDraw = i_BodyParts->GetComponent<DrawComponent>();
				bodyDraw->m_f_RgbaColor ={ 1.0f,1.0f,1.0f,1.0f };
			}
		}
			break;
	}

	powerUp->SetPowerUp(kPowerUpEnd);
}







