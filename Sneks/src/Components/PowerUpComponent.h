/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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
	kPowerUpConsume,
	kPowerUpTailSwipe,
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
