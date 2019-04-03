
#include "ParticleSpawnerComponent.h"

void ParticleSpawnerComponent::SetParticleType(ParticleType type, GraphicsSystem* graphics)
{
	switch (type)
	{
		case ParticleType::kParticleBasicOneShot:
			m_x_ParticleType = type;
			
			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("Rocks");

			m_i_RandomStartFrame = 28;
			m_i_SpriteCountX = 4;
			m_i_SpriteCountY = 7;
			m_i_SecondsPerFrame = 10.0f;

			m_i_ParticleDrawOrder = 6;
		
			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;
		
			m_f_ParticleSize = 1.7f;
		
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

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			m_b_UseStaticLocation = true;


			break;

		case ParticleType::kParticleLargeOneShot:
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("Rock");
			m_i_SpriteCountX = 4;
			m_i_SpriteCountY = 7;
			m_i_SecondsPerFrame = 10.0f;


			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 1.0f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 0.0f;
			m_f_SpreadAngle = PI * 2.0f;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_f_ParticleSpeed = 100.0f;
			m_f_ParticleSpawnFrequency = 0.0f;
			m_i_ParticleSpawnDensity = 10;
			m_f_ParticleMaxLifetime = 2.0f;

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			break;

		case ParticleType::kParticleTrailEffect:
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("junction.png");
			m_i_SpriteCountX = 1;
			m_i_SpriteCountY = 1;
			m_i_SecondsPerFrame = 10.0f;

			m_i_ParticleDrawOrder = 8;
		
			m_i_SplitNumber = 2;
			m_i_CurrentSplit = -1;
		
			m_f_ParticleSize = 1.0f;
			m_f_ParticleSize.x *= 5.0f;

		
			m_f_SpreadDistance = 20.0f;
			m_f_AngleForSpreadDistance = 0;
			m_f_SpreadAngle = 0;
			m_f_OffsetDistance = 10.0f;
			m_f_AngleForOffsetDistance = 0;
			m_f_OffsetAngle = 0;
		
			m_f_ParticleSpeed = 0;
			m_f_ParticleSpawnFrequency = 0.03f;
			m_i_ParticleSpawnDensity = 2;
			m_f_ParticleMaxLifetime = 0.8f;

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 10000000.0f;
			m_b_IsParticleEffectEternal = true;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = true;

		break;
		case kParticleSnekImpactSparks: 
			
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("Rocks");
			m_i_SpriteCountX = 4;
			m_i_SpriteCountY = 7;
			m_i_SecondsPerFrame = 10.0f;


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

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
		break;

		case ParticleType::kParticleExplosion:
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("ExplosionParticle");
			m_i_SpriteCountX = 5;
			m_i_SpriteCountY = 1;
			m_i_SecondsPerFrame = 0.1f;

			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 1.7f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 0.0f;
			m_f_SpreadAngle = 0;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_f_ParticleSpeed = 0.1f;
			m_f_ParticleSpawnFrequency = 0.0f;
			m_i_ParticleSpawnDensity = 1;
			m_f_ParticleMaxLifetime = 0.5f;

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = false;
			m_b_UseStaticLocation = true;
			break;

		case ParticleType::kParticleHit:
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("HitParticle");
			m_i_SpriteCountX = 4;
			m_i_SpriteCountY = 1;
			m_i_SecondsPerFrame = 0.08f;

			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 1;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 1.7f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = 0;
			m_f_SpreadAngle = 0;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_f_ParticleSpeed = 0;
			m_f_ParticleSpawnFrequency = 0.05f;
			m_i_ParticleSpawnDensity = 3;
			m_f_ParticleMaxLifetime = 0.32f;

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.3f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = true;
			m_b_UseStaticLocation = true;
			break;
		case ParticleType::kParticleSpark:
			m_x_ParticleType = type;

			m_b_ParticleFixOffset = false;
			m_b_ParticleFixDirection = false;

			m_px_ParticleTexture = graphics->FetchTexture("SparkParticleWhite");
			m_i_SpriteCountX = 5;
			m_i_SpriteCountY = 1;
			m_i_SecondsPerFrame = 0.15f;

			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 12.0f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 2.0f;
			m_f_SpreadAngle = 0;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_x_ParticleEffectColor={ 0, 0,0.45f,0.4f};
			m_f_ParticleSpeed = 0;
			m_f_ParticleSpawnFrequency = 0.05f;
			m_i_ParticleSpawnDensity = 1;
			m_f_ParticleMaxLifetime = 0.75f;

			m_f_ParticleStartingAlpha = 1.0f;
			m_f_ParticleFadeAmount = 1.0f;

			//m_f_ParticleEffectMaxLifetime = 0.5f;
			m_f_ParticleEffectRemainingLifetime = 0.2f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = true;
			break;
	}
}

void ParticleSpawnerComponent::SetPickUpText(AEGfxTexture* texture)
{
	m_x_ParticleType = kParticleText;

	m_b_ParticleFixOffset = true;
	m_b_ParticleFixDirection = true;

	m_px_ParticleTexture = texture;
	m_i_SpriteCountX = 1;
	m_i_SpriteCountY = 1;
	m_i_SecondsPerFrame = 10.0f;

	m_i_ParticleDrawOrder = 2;

	m_i_SplitNumber = 0;
	m_i_CurrentSplit = -1;

	m_f_ParticleSize = 1.0f;

	m_f_SpreadDistance = 0.0f;
	m_f_AngleForSpreadDistance = PI * 0.0f;
	m_f_SpreadAngle = PI * 0.0f;
	m_f_OffsetDistance = 50.0f;
	m_f_AngleForOffsetDistance = PI * 0.5f;
	m_f_OffsetAngle = PI * 0.5f;

	m_x_ParticleEffectColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_f_ParticleSpeed = 200.0f;
	m_f_ParticleSpawnFrequency = 0.00f;
	m_i_ParticleSpawnDensity = 1;
	m_f_ParticleMaxLifetime = 0.75f;

	m_f_ParticleStartingAlpha = 1.0f;
	m_f_ParticleFadeAmount = 0.5f;

	//m_f_ParticleEffectMaxLifetime = 0.5f;
	m_f_ParticleEffectRemainingLifetime = 0.0f;
	m_b_IsParticleEffectEternal = false;
	m_b_IsParticleEffectOneShot = true;
	m_b_HasParticleEffectFired = false;
	m_b_AlphaScalingEnabled = true;
}

void ParticleSpawnerComponent::UpdateTime(float dt)
{
	m_f_ParticleEffectRemainingLifetime -= dt;
	m_b_HasParticleEffectFired = true;
}

bool ParticleSpawnerComponent::IsParticleEffectAlive()
{
	/*return !HasParticleEffectOneShot();*/
	if (m_b_IsParticleEffectOneShot)
	{
		return !m_b_HasParticleEffectFired;
	}
	return m_f_ParticleEffectRemainingLifetime > 0;
}

ParticleType ParticleSpawnerComponent::GetParticleType()
{
	return m_x_ParticleType;
}

AEGfxTexture* ParticleSpawnerComponent::GetParticleTexture()
{
	return m_px_ParticleTexture;
}

int ParticleSpawnerComponent::GetParticleDrawOrder()
{
	return m_i_ParticleDrawOrder;
}

int ParticleSpawnerComponent::GetSplitNumber()
{
	return m_i_SplitNumber;
}

int ParticleSpawnerComponent::GetCurrentSplit()
{
	if ((++m_i_CurrentSplit) >= m_i_SplitNumber)
		m_i_CurrentSplit -= m_i_SplitNumber;
	return m_i_CurrentSplit;
}

float ParticleSpawnerComponent::GetCurrentSplitFactor()
{
	// ( 2 / ( max - 1) ) * curr - 1
	return ( GetSplitBool() ? ((2.0f / (m_i_SplitNumber - 1)) * GetCurrentSplit() - 1.0f): 0.0f );
}

bool ParticleSpawnerComponent::GetSplitBool()
{
	return (m_i_SplitNumber > 1);
}

float ParticleSpawnerComponent::GetSpreadDistance()
{
	return m_f_SpreadDistance;
}

float ParticleSpawnerComponent::GetAngleForSpreadDistance()
{
	return m_f_AngleForSpreadDistance;
}

float ParticleSpawnerComponent::GetSpreadAngle()
{
	return m_f_SpreadAngle;
}

float ParticleSpawnerComponent::GetOffsetDistance()
{
	return m_f_OffsetDistance;
}

float ParticleSpawnerComponent::GetAngleForOffsetDistance()
{
	return m_f_AngleForOffsetDistance;
}

float ParticleSpawnerComponent::GetOffsetAngle()
{
	return m_f_OffsetAngle;
}

float ParticleSpawnerComponent::GetParticleSpeed()
{
	return m_f_ParticleSpeed;
}

float ParticleSpawnerComponent::GetParticleMaxLifetime()
{
	return m_f_ParticleMaxLifetime;
}

int ParticleSpawnerComponent::GetParticleSpawnDensity()
{
	return m_i_ParticleSpawnDensity;
}

float ParticleSpawnerComponent::GetParticleSpawnFrequency()
{
	return m_f_ParticleSpawnFrequency;
}

float& ParticleSpawnerComponent::GetParticleSpawnTimer()
{
	return m_f_ParticleEffectSpawnTimer;
}

float ParticleSpawnerComponent::GetParticleStartingAlpha()
{
	return m_f_ParticleStartingAlpha;
}

float ParticleSpawnerComponent::GetParticleFadeAmount()
{
	return m_f_ParticleFadeAmount;
}

bool ParticleSpawnerComponent::GetIsParticleEffectOneShot()
{
	return m_b_IsParticleEffectOneShot;
}

bool ParticleSpawnerComponent::GetIsParticleFixOffset()
{
	return m_b_ParticleFixOffset;
}

bool ParticleSpawnerComponent::GetIsParticleFixDirection()
{
	return m_b_ParticleFixDirection;
}

void ParticleSpawnerComponent::SetSpawnTransform(TransformComponent* spawnTransform)
{
	if (spawnTransform)
		m_x_LastKnown = *spawnTransform;
	m_po_SpawnTransform = spawnTransform;
}

TransformComponent* ParticleSpawnerComponent::GetSpawnTransform()
{
	if (m_b_UseStaticLocation)
	{
		return &m_x_LastKnown;
	}
	//TODO FIX do not store TransformComponent*
	if (m_po_SpawnTransform->m_x_ComponentID == kComponentTransform) 
	{
		m_x_LastKnown = *m_po_SpawnTransform;
		return m_po_SpawnTransform;
	}
	else 
		return &m_x_LastKnown;
}

