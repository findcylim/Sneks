#pragma once
#include "PowerUpComponent.h"


PowerUpComponent::PowerUpComponent()
{
	SetPowerUp(kPowerUpEnd);
}

void PowerUpComponent::UpdateTime(float dt)
{
	if (m_f_PowerUpDurationLeft)
	{
		m_f_PowerUpDurationLeft -= dt;
	}
	else
		m_b_JustExpired = true;
}

void PowerUpComponent::SetPowerUp(const PowerUpType type)
{
	m_x_PowerUpType = (PowerUpType)type;

	switch (type)
	{
		case kPowerUpSpeedIncrease:
			m_f_PowerUpDurationLeft = 5.0f;
			m_f_PowerIncrease = 1.2f;
			break;

		case kPowerUpGrowthIncrease:
			m_f_PowerUpDurationLeft = 5.0f;
			m_f_PowerIncrease = 2.0f;
			break;

		case kPowerUpUnlimitedSpecial:
			m_f_PowerUpDurationLeft = 0;
			m_f_PowerIncrease = 0;
			break;

		case kPowerUpStar:
			m_f_PowerUpDurationLeft = 5.0f;
			m_f_PowerIncrease = 5.0f;
			break;

		case kPowerUpPlusBody:
			m_f_PowerUpDurationLeft = 0;
			m_f_PowerIncrease = 5;
			break;

		case kPowerUpIncreaseDamage:
			m_f_PowerUpDurationLeft = 0;
			m_f_PowerIncrease = 5;
			break;


		case kPowerUpEnd:
			m_f_PowerUpDurationLeft = 0;
			m_f_PowerIncrease = 0;
			break;

	}
}

PowerUpType PowerUpComponent::GetPowerUp() const
{
	return m_x_PowerUpType;
}

float PowerUpComponent::GetPowerIncrease() const
{
	return m_f_PowerIncrease;
}

bool PowerUpComponent::IsActive() const
{
	return (m_f_PowerUpDurationLeft > 0);
}

bool PowerUpComponent::Expired()
{
	if (m_b_JustExpired)
	{
		m_b_JustExpired = false;
		return true;
	}
	else
		return false;
}

void  PowerUpComponent::ResetPowerIncrease()
{
	m_f_PowerIncrease = 0;
}