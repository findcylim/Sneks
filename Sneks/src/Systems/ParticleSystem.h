
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/ParticleEffectComponent.h"

class ParticleSystem final : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	GraphicsSystem* m_o_GraphicsSystem;

	void SpawnParticle(ParticleEffectComponent* pec);
	void ChangeParticleEffect(SnekHeadEntity* owner, ParticleType type);

public:
	ParticleSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics);
	~ParticleSystem();

	void Initialize();
	void Update(float dt);
	void receive(const Events::EV_PLAYER_COLLISION& eventData);

	void PlayParticleEffect(SnekHeadEntity* owner, ParticleType type);
};

#endif // !PARTICLESYSTEM_H
