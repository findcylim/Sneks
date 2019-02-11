#pragma once
#include "../ECS/EntityManager.h"
#include <vector>
#include "DrawComponent.h"
#include "ParticleEffectComponent.h"

class ParticleComponent : public BaseComponent
{
private:
	float m_f_ParticleRemainingLifetime = 0;
	float m_f_ParticleMaxLifetime = 0;

public:
	void SetParticleMaxLifetime(float maxLifetime);
	void UpdateTime(float dt);
	float GetAlphaValue();
	bool IsAlive();
};
