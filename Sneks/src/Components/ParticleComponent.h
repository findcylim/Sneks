#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"
#include "ParticleSpawnerComponent.h"

struct ParticleComponent : public BaseComponent
{
	bool  m_b_AlphaScalingEnabled = true;
	float m_f_BaseAlpha = 1.0f;
	void SetParticleMaxLifetime(float maxLifetime);
	void UpdateTime(float dt);
	float GetAlphaValue();
	bool IsAlive();

private:
	float m_f_ParticleRemainingLifetime = 0;
	float m_f_ParticleMaxLifetime = 0;
};
