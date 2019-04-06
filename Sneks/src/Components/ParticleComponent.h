/* Start Header****************************************************************/
/*!
\file ParticleComponent.h
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
