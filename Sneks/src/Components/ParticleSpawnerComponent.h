#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"
#include "ParticleComponent.h"
#include "../Systems/GraphicsSystem.h"

enum ParticleType : unsigned char
{
	kParticleEmpty,
	kParticleBasicOneShot,
	kParticleTrailEffect,
	kParticleSnekImpactSparks,
	kParticleLargeOneShot,
	kParticleExplosion,
	kParticleHit,
	kParticleSpark
};

class ParticleSpawnerComponent : public BaseComponent
{
private:
	ParticleType m_x_ParticleType = kParticleEmpty;

	AEGfxTexture* m_px_ParticleTexture = nullptr;

	int m_i_ParticleDrawOrder = 3;
	int m_i_SplitNumber = 0;
	int m_i_CurrentSplit = -1;


	float m_f_SpreadDistance = 0.0f;
	//Angle stored as radian.
	float m_f_AngleForSpreadDistance = 0.0f;
	//Angle stored as radian.
	float m_f_SpreadAngle = 0.0f;
	float m_f_OffsetDistance = 0.0f;
	//Angle stored as radian.
	float m_f_AngleForOffsetDistance = 0.0f;
	//Angle stored as radian.
	float m_f_OffsetAngle = 0.0f;

	float m_f_ParticleSpeed = 0.0f;
	float m_f_ParticleSpawnFrequency = 0.0f;
	int m_i_ParticleSpawnDensity = 0;
	float m_f_ParticleMaxLifetime = 0.0f;

	float m_f_ParticleStartingAlpha = 1.0f;
	float m_f_ParticleFadeAmount = 1.0f;

	//float m_f_ParticleEffectMaxLifetime = 0.0f;
	float m_f_ParticleEffectRemainingLifetime = 0.0f;
	float m_f_ParticleEffectSpawnTimer = 0.0f;
	bool m_b_IsParticleEffectEternal = false;
	bool m_b_IsParticleEffectOneShot = false;
	bool m_b_HasParticleEffectFired = false;

	TransformComponent* m_po_SpawnTransform = nullptr;
	TransformComponent  m_x_LastKnown;
public:
	HTVector2 m_f_ParticleSize ={ 1.0f, 1.0f };
	bool m_b_UseStaticLocation = false;
	int m_i_SpriteCountX = 0;
	int m_i_SpriteCountY = 0;
	float m_i_SecondsPerFrame = 0.5f;
	bool m_b_AlphaScalingEnabled = true;
	int m_i_RandomStartFrame = 0;
	HTColor m_x_ParticleEffectColor ={ 1.0f,1.0f,1.0f,1.0f };


	void SetParticleType(ParticleType type, GraphicsSystem* graphics);
	void UpdateTime(float dt);
	bool IsParticleEffectAlive();

	ParticleType GetParticleType();
	AEGfxTexture* GetParticleTexture();
	int GetParticleDrawOrder();
	int GetSplitNumber();
	int GetCurrentSplit();
	//Returns -1 to 1.
	float GetCurrentSplitFactor();
	bool GetSplitBool();
	float GetSpreadDistance();
	float GetAngleForSpreadDistance();
	float GetSpreadAngle();
	float GetOffsetDistance();
	float GetAngleForOffsetDistance();
	float GetOffsetAngle();
	float GetParticleSpeed();
	float GetParticleMaxLifetime();
	int GetParticleSpawnDensity();
	float GetParticleSpawnFrequency();
	float& GetParticleSpawnTimer();
	float GetParticleStartingAlpha();
	float GetParticleFadeAmount();
	bool GetIsParticleEffectOneShot();

	void SetSpawnTransform(TransformComponent* spawnTransform);
	TransformComponent* GetSpawnTransform();
};
