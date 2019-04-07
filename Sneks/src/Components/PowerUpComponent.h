/* Start Header****************************************************************/
/*!
\file PowerUpComponent.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/EntityManager.h"

enum PowerUpType : unsigned char
{
	kPowerUpSpring = 0,
	kPowerUpStar,
	kPowerUpPlusBody,
	kPowerUpConsume,
	kPowerUpTailSwipe,
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
	int m_i_Stage = 0;

	PowerUpComponent();
	void UpdateTime(float dt);
	void SetPowerUp(const PowerUpType type);
	void ResetPowerIncrease();
	PowerUpType GetPowerUp() const;
	float GetPowerIncrease() const;
	bool IsActive() const;
	bool Expired();
};
