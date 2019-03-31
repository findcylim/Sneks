#pragma once
#include "../ECS/EntityManager.h"

enum PowerUpType : unsigned char
{
	kPowerUpSpeedIncrease = 0,
	kPowerUpGrowthIncrease,
	kPowerUpUnlimitedSpecial,
	kPowerUpStar,
	kPowerUpPlusBody,
	kPowerUpIncreaseDamage,
	kPowerUpEnd
};

struct PowerUpComponent : public BaseComponent
{
	PowerUpType m_x_PowerUpType = kPowerUpEnd;
	float m_f_PowerUpDurationLeft = 0;
	float m_f_PowerIncrease = 0;
	bool m_b_JustExpired = false;
	float m_f_SpecialTimer = 0;

	PowerUpComponent();
	void UpdateTime(float dt);
	void SetPowerUp(const PowerUpType type);
	void ResetPowerIncrease();
	PowerUpType GetPowerUp() const;
	float GetPowerIncrease() const;
	bool IsActive() const;
	bool Expired();
};
