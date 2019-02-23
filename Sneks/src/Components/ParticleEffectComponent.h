#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"
#include "ParticleComponent.h"
#include "../Systems/GraphicsSystem.h"

enum ParticleType : unsigned char
{
	kParticleEmpty,
	kParticleBasicOneShot
};

class ParticleEffectComponent : public BaseComponent
{
private:
	ParticleType m_x_ParticleType = kParticleEmpty;
	AEGfxTexture* m_px_ParticleTexture = nullptr;
	int m_i_ParticleSpawnDensity = 0;
	float m_f_ParticleSizeMultiplier = 0.0f;
	float m_f_ParticleSizeX = 0.0f;
	float m_f_ParticleSizeY = 0.0f;
	float m_f_ParticleSpeed = 0.0f;
	float m_f_ParticleMaxLifetime = 0.0f;
	float m_f_ParticleSpawnFrequency = 0.0f;
	float m_f_ParticleEffectRemainingLifetime = 0.0f;
	float m_f_ParticleEffectMaxLifetime = 0.0f;
	bool m_b_IsParticleEffectEternal = false;
	bool m_b_IsParticleEffectOneShot = false;
	bool m_b_HasParticleEffectFired = false;

	TransformComponent* m_po_SpawnTransform;

	bool HasParticleEffectOneShot();

public:
	void SetParticleType(ParticleType type, GraphicsSystem* graphics);
	void UpdateTime(float dt);
	bool IsParticleEffectAlive();

	ParticleType GetParticleType();
	AEGfxTexture* GetParticleTexture();
	float GetParticleSizeX();
	float GetParticleSizeY();
	float GetParticleSpeed();
	float GetParticleMaxLifetime();
	int GetParticleSpawnDensity();
	float GetParticleSpawnFrequency();
	bool GetIsParticleEffectOneShot();

	void SetSpawnTransform(TransformComponent* spawnTransform);
	TransformComponent* GetSpawnTransform();
};
