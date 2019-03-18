#pragma once
#include "PowerUpComponent.h"


PowerUpComponent::PowerUpComponent()
{
	SetPowerUp(kPowerUpEnd);
}

void PowerUpComponent::UpdateTime(float dt)
{
	if (m_f_PowerUpRemainingLifetime)
	{
		m_f_PowerUpRemainingLifetime -= dt;
	}
	else
		m_b_JustDied = true;
}

void PowerUpComponent::SetPowerUp(PowerUpType type)
{
	m_x_PowerUpType = type;

	switch (type)
	{
		case kPowerUpSpeedIncrease:
			m_f_PowerUpRemainingLifetime = 5.0f;
			m_f_PowerIncrease = 1.5f;
			break;

		case kPowerUpGrowthIncrease:
			m_f_PowerUpRemainingLifetime = 5.0f;
			m_f_PowerIncrease = 2.0f;
			break;

		case kPowerUpUnlimitedSpecial:
			m_f_PowerUpRemainingLifetime = 0;
			m_f_PowerIncrease = 0;
			break;

		case kPowerUpInvul:
			m_f_PowerUpRemainingLifetime = 5.0f;
			m_f_PowerIncrease = 1.5f;
			break;

		case kPowerUpPlusBody:
			m_f_PowerUpRemainingLifetime = 0;
			m_f_PowerIncrease = 0;
			break;

		case kPowerUpEnd:
			m_f_PowerUpRemainingLifetime = 0;
			m_f_PowerIncrease = 0;
			break;
	}
}

PowerUpType PowerUpComponent::GetPowerUp()
{
	return m_x_PowerUpType;
}

float PowerUpComponent::GetPowerIncrease()
{
	return m_f_PowerIncrease;
}

bool PowerUpComponent::IsAlive()
{
	return (m_f_PowerUpRemainingLifetime > 0);
}

bool PowerUpComponent::GetJustDied()
{
	if (m_b_JustDied)
	{
		m_b_JustDied = false;
		return true;
	}
	else
		return false;
}

void  PowerUpComponent::ResetPowerIncrease()
{
	m_f_PowerIncrease = 0;
}






