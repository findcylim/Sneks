
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
	return (m_f_ParticleRemainingLifetime / m_f_ParticleMaxLifetime);
}

bool ParticleComponent::IsAlive()
{
	return m_f_ParticleRemainingLifetime > 0;
}
