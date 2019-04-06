/* Start Header****************************************************************/
/*!
\file ParticleComponent.cpp
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


#include "ParticleComponent.h"

void ParticleComponent::SetParticleMaxLifetime(float maxLifetime)
{
	m_f_ParticleRemainingLifetime = m_f_ParticleMaxLifetime = maxLifetime;
}

void ParticleComponent::UpdateTime(float dt)
{
	m_f_ParticleRemainingLifetime -= dt;
}

float ParticleComponent::GetAlphaValue()
{
	if (m_b_AlphaScalingEnabled)
		return (m_f_ParticleStartingAlpha - m_f_ParticleFadeAmount *
		(m_f_ParticleMaxLifetime - m_f_ParticleRemainingLifetime) / m_f_ParticleMaxLifetime);
	return m_f_ParticleStartingAlpha;
}

bool ParticleComponent::IsAlive()
{
	return m_f_ParticleRemainingLifetime > 0;
}
