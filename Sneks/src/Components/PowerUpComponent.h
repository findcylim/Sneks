#pragma once
#include "../ECS/EntityManager.h"

enum PowerUpType : unsigned char
{
	kPowerUpSpeedIncrease = 0,
	kPowerUpGrowthIncrease,
	kPowerUpUnlimitedSpecial,
	kPowerUpInvul,
	kPowerUpPlusBody,
	kPowerUpEnd
};

class PowerUpComponent : public BaseComponent
{
	private:
		PowerUpType m_x_PowerUpType = kPowerUpEnd;
		float m_f_PowerUpRemainingLifetime = 0;
		float m_f_PowerIncrease = 0;
		bool m_b_JustDied = false;

	public:
		PowerUpComponent();
		void UpdateTime(float dt);
		void SetPowerUp(PowerUpType type);
		PowerUpType GetPowerUp();
		float GetPowerIncrease();
		bool IsAlive();
		bool GetJustDied();
};