
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
		GraphicsSystem* m_po_GraphicsSystem;
		SnekSystem* m_po_SnekSystem;
		float m_f_SpawnChance = 0.2f;
		float m_f_ForwardAngleRange = PI * 0.4f;
		float m_f_HolderSpeedRatio = 0.3f;
		float m_f_HolderSizeRatio = 0.3f;
		float m_f_HolderInvulTime = 1.0f;

		void SpawnPowerUp(TransformComponent* spawnPoint, TransformComponent* snekTransform);
		void UpdatePowerUp(PowerUpComponent* powerup, DrawComponent* powerUpDrawComponent);
		void RemovePowerUp(PowerUpComponent* powerup);

	public:
		PowerUpSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics, SnekSystem* snek);
		~PowerUpSystem();

		void Initialize();
		void Update(float dt);
		void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};








#endif // !POWERUP_SYSTEM_H

