
#include "ParticleSpawnerComponent.h"

void ParticleSpawnerComponent::SetParticleType(ParticleType type, GraphicsSystem* graphics)
{
	switch (type)
	{
		case ParticleType::kParticleBasicOneShot:
			m_x_ParticleType = type;
		
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

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;

			break;

		case ParticleType::kParticleLargeOneShot:
			m_x_ParticleType = type;

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

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			break;

		case ParticleType::kParticleTrailEffect:
			m_x_ParticleType = type;
		
			m_px_ParticleTexture = graphics->FetchTexture("Rocks");
			m_i_SpriteCountX = 4;
			m_i_SpriteCountY = 7;
			m_i_SecondsPerFrame = 10.0f;


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

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = true;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
		break;
		case kParticleSnekImpactSparks: 
			
			m_x_ParticleType = type;

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

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
		break;

		case ParticleType::kParticleExplosion:
			m_x_ParticleType = type;

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

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = false;
			break;

		case ParticleType::kParticleHit:
			m_x_ParticleType = type;

			m_px_ParticleTexture = graphics->FetchTexture("HitParticle");
			m_i_SpriteCountX = 4;
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

			m_f_ParticleSpeed = 0;
			m_f_ParticleSpawnFrequency = 0.0f;
			m_i_ParticleSpawnDensity = 10;
			m_f_ParticleMaxLifetime = 0.4f;

			//m_f_ParticleEffectMaxLifetime = 0.0f;
			m_f_ParticleEffectRemainingLifetime = 0.0f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = true;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = false;
			break;
		case ParticleType::kParticleSpark:
			m_x_ParticleType = type;

			m_px_ParticleTexture = graphics->FetchTexture("SparkParticleWhite");
			m_i_SpriteCountX = 5;
			m_i_SpriteCountY = 1;
			m_i_SecondsPerFrame = 0.1f;

			m_i_ParticleDrawOrder = 6;

			m_i_SplitNumber = 0;
			m_i_CurrentSplit = -1;

			m_f_ParticleSize = 17.0f;

			m_f_SpreadDistance = 0.0f;
			m_f_AngleForSpreadDistance = PI * 2.0f;
			m_f_SpreadAngle = 0;
			m_f_OffsetDistance = 0.0f;
			m_f_AngleForOffsetDistance = PI * 0.0f;
			m_f_OffsetAngle = PI * 0.0f;

			m_x_ParticleEffectColor={ 0.56f, 0.65f,1.0f,0.5f };
			m_f_ParticleSpeed = 0;
			m_f_ParticleSpawnFrequency = 0.1f;
			m_i_ParticleSpawnDensity = 1;
			m_f_ParticleMaxLifetime = 0.5f;

			//m_f_ParticleEffectMaxLifetime = 0.5f;
			m_f_ParticleEffectRemainingLifetime = 0.2f;
			m_b_IsParticleEffectEternal = false;
			m_b_IsParticleEffectOneShot = false;
			m_b_HasParticleEffectFired = false;
			m_b_AlphaScalingEnabled = true;
			break;
	}
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

float ParticleSpawnerComponent::GetParticleSize()
{
	return m_f_ParticleSize;
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

bool ParticleSpawnerComponent::GetIsParticleEffectOneShot()
{
	return m_b_IsParticleEffectOneShot;
}

void ParticleSpawnerComponent::SetSpawnTransform(TransformComponent* spawnTransform)
{
	m_po_SpawnTransform = spawnTransform;
}

TransformComponent* ParticleSpawnerComponent::GetSpawnTransform()
{
	return m_po_SpawnTransform;
}


