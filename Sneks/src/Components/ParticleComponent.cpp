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
