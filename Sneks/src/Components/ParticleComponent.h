#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"
#include "ParticleSpawnerComponent.h"

struct ParticleComponent : public BaseComponent
{
public:
	bool  m_b_AlphaScalingEnabled = true;
	float m_f_BaseAlpha = 1.0f;
	void SetParticleMaxLifetime(float maxLifetime);
	void UpdateTime(float dt);
	float GetAlphaValue();
	bool IsAlive();
	float m_f_ParticleStartingAlpha = 1.0f;
	float m_f_ParticleFadeAmount = 1.0f;

private:
	float m_f_ParticleRemainingLifetime = 0;
	float m_f_ParticleMaxLifetime = 0;
};
