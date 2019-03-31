
#ifndef POWERUP_SYSTEM_H
#define POWERUP_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "SnekSystem.h"
#include "../Components/PowerUpComponent.h"
#include "../Components/PowerUpHolderComponent.h"

class PowerUpSystem final : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
	private:
		GraphicsSystem* m_po_GraphicsSystem;
		float m_f_SpawnChance = 0.5f;
		float m_f_ForwardAngleRange = PI * 0.4f;
		float m_f_HolderSpeedRatio = 0.3f;
		float m_f_HolderSizeRatio = 0.3f;
		float m_f_HolderInvulTime = 1.0f;

		void SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekTransform);
		void PowerUpPickup(PowerUpComponent* powerUp, PowerUpHolderComponent* powerUpHolder);
		void PowerUpExpire(PowerUpComponent* powerUp) const;

	public:
		PowerUpSystem(GraphicsSystem* graphics);
		~PowerUpSystem();

		void Initialize();
		HTColor StarColorChange(HTColor& color) const;
		void Update(float dt) override;
		void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
};

#endif // !POWERUP_SYSTEM_H

