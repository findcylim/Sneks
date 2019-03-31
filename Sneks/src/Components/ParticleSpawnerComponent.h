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
	kParticleSpark,
	kParticleSparks
};

struct ParticleSpawnerComponent : public BaseComponent
{
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
	float m_f_ParticleEffectRemainingLifetime = 0.0f;
	float m_f_ParticleEffectSpawnTimer = 0.0f;
	bool m_b_IsParticleEffectEternal = false;
	bool m_b_IsParticleEffectOneShot = false;
	bool m_b_HasParticleEffectFired = false;
	TransformComponent* m_po_SpawnTransform = nullptr;
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
	int GetCurrentSplit();
	//Returns -1 to 1.
	float GetCurrentSplitFactor();
	bool GetSplitBool();
	void SetSpawnTransform(TransformComponent* spawnTransform);
	TransformComponent* GetSpawnTransform();

private:
	TransformComponent  m_x_LastKnown;
	ParticleType m_x_ParticleType = kParticleEmpty;

};
