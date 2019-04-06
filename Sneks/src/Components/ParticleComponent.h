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
#include <vector>
#include "DrawComponent.h"
#include "ParticleSpawnerComponent.h"

struct ParticleComponent : public BaseComponent
{
public:
	bool  m_b_AlphaScalingEnabled = true;
	float m_f_BaseAlpha = 1.0f;
	void SetParticleMaxLifetime(float maxLifetime);
	void UpdateTime(float dt);
	float GetAlphaValue();
	bool IsAlive();
	float m_f_ParticleStartingAlpha = 1.0f;
	float m_f_ParticleFadeAmount = 1.0f;

private:
	float m_f_ParticleRemainingLifetime = 0;
	float m_f_ParticleMaxLifetime = 0;
};
