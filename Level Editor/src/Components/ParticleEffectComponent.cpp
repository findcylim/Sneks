
#include "ParticleEffectComponent.h"

void ParticleEffectComponent::SetParticleType(ParticleType type, GraphicsSystem* graphics)
{
	switch (type)
	{
	case ParticleType::kParticleBasicOneShot:
		m_x_ParticleType = type;
		m_px_ParticleTexture = graphics->FetchTexture("SnekHead01");
		m_f_ParticleSizeMultiplier = 0.1f;
		m_f_ParticleSizeX = m_f_ParticleSizeMultiplier * 186.0f;
		m_f_ParticleSizeY = m_f_ParticleSizeMultiplier * 186.0f;
		m_f_ParticleSpeed = 15.0f;
		m_i_ParticleSpawnDensity = 10;
		m_f_ParticleMaxLifetime = 10.0f;
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

ParticleType ParticleEffectComponent::GetParticleType()
{
	return m_x_ParticleType;
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

void ParticleEffectComponent::SetSpawnTransform(TransformComponent* spawnTransform)
{
	m_po_SpawnTransform = spawnTransform;
}

TransformComponent* ParticleEffectComponent::GetSpawnTransform()
{
	return m_po_SpawnTransform;
}


