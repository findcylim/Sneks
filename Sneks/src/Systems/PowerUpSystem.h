
#ifndef POWERUP_SYSTEM_H
#define POWERUP_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "SnekSystem.h"
#include "../Components/PowerUpComponent.h"

class PowerUpSystem final : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
	private:
		GraphicsSystem* m_o_GraphicsSystem;
		SnekSystem* m_o_SnekSystem;
		float m_f_SpawnChance = 0.2f;
		float m_f_ForwardAngleRange = PI * 0.4f;
		float m_f_HolderSpeedRatio = 0.3f;
		float m_f_HolderSizeRatio = 0.3f;
		float m_f_HolderInvulTime = 1.0f;

		void SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekVelocity);
		void UpdatePowerUp(PowerUpComponent* powerup);
		void RemovePowerUp(PowerUpComponent* powerup);

	public:
		PowerUpSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics, SnekSystem* snek);
		~PowerUpSystem();

		void Initialize();
		void Update(float dt);
		void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};








#endif // !POWERUP_SYSTEM_H

