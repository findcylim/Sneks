
#include "ParticleEffectComponent.h"

void ParticleEffectComponent::SetParticleType(ParticleType type, GraphicsSystem* graphics)
{
	switch (type)
	{
	case ParticleType::kParticleBasicOneShot:
		m_x_ParticleType = type;
		AEGfxTexture* m_px_ParticleTexture =
			graphics->FetchTexture("Ball");
		m_f_ParticleSizeMultiplier = 5.05f;
		m_f_ParticleSizeX = m_f_ParticleSizeMultiplier * 199.0f;
		m_f_ParticleSizeY = m_f_ParticleSizeMultiplier * 117.0f;
		m_f_ParticleSpeed = 15.0f;
		m_i_ParticleSpawnDensity = 10;
		m_f_ParticleSpawnFrequency = 0.0f;
		m_f_ParticleEffectRemainingLifetime = 0.0f;
		m_f_ParticleEffectMaxLifetime = 0.0f;
		m_b_IsParticleEffectEternal = false;
		m_b_IsParticleEffectOneShot = true;
		m_b_HasParticleEffectFired = false;
		break;
	}
}

void ParticleEffectComponent::UpdateTime(float dt)
{
	m_f_ParticleEffectRemainingLifetime -= dt;
	m_b_HasParticleEffectFired = true;
}

void ParticleEffectComponent::Revive()
{
	m_f_ParticleEffectRemainingLifetime = m_f_ParticleEffectMaxLifetime;
	m_b_HasParticleEffectFired = false;
}

bool ParticleEffectComponent::IsParticleEffectAlive()
{
	if (m_f_ParticleEffectRemainingLifetime > 0 || !HasParticleEffectOneShot())
		return true;
	else
		return false;
}

bool ParticleEffectComponent::HasParticleEffectOneShot()
{
	return (m_b_HasParticleEffectFired && m_b_IsParticleEffectOneShot);
}

AEGfxTexture* ParticleEffectComponent::GetParticleTexture()
{
	return m_px_ParticleTexture;
}

float ParticleEffectComponent::GetParticleSizeX()
{
	return m_f_ParticleSizeX;
}

float ParticleEffectComponent::GetParticleSizeY()
{
	return m_f_ParticleSizeY;
}

float ParticleEffectComponent::GetParticleSpeed()
{
	return m_f_ParticleSpeed;
}

float ParticleEffectComponent::GetParticleMaxLifetime()
{
	return m_f_ParticleMaxLifetime;
}

int ParticleEffectComponent::GetParticleSpawnDensity()
{
	return m_i_ParticleSpawnDensity;
}

float ParticleEffectComponent::GetParticleSpawnFrequency()
{
	return m_f_ParticleSpawnFrequency;
}

bool ParticleEffectComponent::GetIsParticleEffectOneShot()
{
	return m_b_IsParticleEffectOneShot;
}
