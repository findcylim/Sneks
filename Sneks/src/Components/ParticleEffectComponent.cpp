
#include "ParticleEffectComponent.h"

void ParticleEffectComponent::SetParticleType(ParticleType type, GraphicsSystem* graphics)
{
	switch (type)
	{
		case ParticleType::kParticleBasicOneShot:
			m_x_ParticleType = type;
		
			m_px_ParticleTexture = graphics->FetchTexture("Rock");
			m_i_ParticleDrawOrder = 6;
		
			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;
		
			m_f_ParticleSize = 0.7f;
		
			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 0.0f;
			m_f_SpreadAngle = PI * 2.0f;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;
		
			m_f_ParticleSpeed = 50.0f;
			m_f_ParticleSpawnFrequency = 0.0f;
			m_i_ParticleSpawnDensity = 10;
			m_f_ParticleMaxLifetime = 2.0f;

			m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			break;

		case ParticleType::kParticleTrailEffect:
			m_x_ParticleType = type;
		
			m_px_ParticleTexture = graphics->FetchTexture("Rock");
			m_i_ParticleDrawOrder = 6;
		
			m_i_SplitNumber = 2;
			m_i_CurrentSplit = -1;
		
			m_f_ParticleSize = 0.6f;
		
			m_f_SpreadDistance = 25.0f;
			m_f_AngleForSpreadDistance = PI * 0.5f;
			m_f_SpreadAngle = PI * 0.15f;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;
		
			m_f_ParticleSpeed = 100.0f;
			m_f_ParticleSpawnFrequency = 1.0f;
			m_i_ParticleSpawnDensity = 3;
			m_f_ParticleMaxLifetime = 2.0f;

			m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = true;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
		break;
		case kParticleSnekImpactSparks: 
			
			m_x_ParticleType = type;

			m_px_ParticleTexture = graphics->FetchTexture("Rock");
			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 0.7f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 0.0f;
			m_f_SpreadAngle = PI * 2.0f;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_f_ParticleSpeed = 50.0f;
			m_f_ParticleSpawnFrequency = 0.0f;
			m_i_ParticleSpawnDensity = 10;
			m_f_ParticleMaxLifetime = 2.0f;

			m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
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
	return m_f_ParticleEffectRemainingLifetime > 0 || !HasParticleEffectOneShot();
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

int ParticleEffectComponent::GetParticleDrawOrder()
{
	return m_i_ParticleDrawOrder;
}

int ParticleEffectComponent::GetSplitNumber()
{
	return m_i_SplitNumber;
}

int ParticleEffectComponent::GetCurrentSplit()
{
	if ((++m_i_CurrentSplit) >= m_i_SplitNumber)
		m_i_CurrentSplit -= m_i_SplitNumber;
	return m_i_CurrentSplit;
}

float ParticleEffectComponent::GetCurrentSplitFactor()
{
	// ( 2 / ( max - 1) ) * curr - 1
	return ( GetSplitBool() ? ((2.0f / (m_i_SplitNumber - 1)) * GetCurrentSplit() - 1.0f): 0.0f );
}

bool ParticleEffectComponent::GetSplitBool()
{
	return (m_i_SplitNumber > 1);
}

float ParticleEffectComponent::GetSpreadDistance()
{
	return m_f_SpreadDistance;
}

float ParticleEffectComponent::GetAngleForSpreadDistance()
{
	return m_f_AngleForSpreadDistance;
}

float ParticleEffectComponent::GetSpreadAngle()
{
	return m_f_SpreadAngle;
}

float ParticleEffectComponent::GetOffsetDistance()
{
	return m_f_OffsetDistance;
}

float ParticleEffectComponent::GetAngleForOffsetDistance()
{
	return m_f_AngleForOffsetDistance;
}

float ParticleEffectComponent::GetOffsetAngle()
{
	return m_f_OffsetAngle;
}

float ParticleEffectComponent::GetParticleSize()
{
	return m_f_ParticleSize;
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

float& ParticleEffectComponent::GetParticleLeftoverTime()
{
	return m_f_ParticleEffectLeftoverTime;
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


