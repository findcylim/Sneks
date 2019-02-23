
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
	void SpawnParticleEffect(TransformComponent* spawnTransform, ParticleType type);

	bool CollisionCheckForParticleSystem(CollisionGroupName name1, TransformComponent* spawn1,
		CollisionGroupName name2, TransformComponent* spawn2, CollisionGroupName namecheck, ParticleType type);

public:
	ParticleSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics);
	~ParticleSystem();

	void Initialize();
	void UpdateMouseParticles();
	void CreateMouseParticles();
	void Update(float dt);
	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};

#endif // !PARTICLESYSTEM_H
