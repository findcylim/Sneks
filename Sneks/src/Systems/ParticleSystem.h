
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

	void SpawnParticle(ParticleEffectComponent* particleEffectComp);
	void SpawnParticleEffect(TransformComponent* spawnTransform, ParticleType particleType);

	float CalculateRotation(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp);
	float CalculatePositionX(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp);
	float CalculatePositionY(ParticleEffectComponent* particleEffectComp, TransformComponent* transformComp);
	bool CollisionCheckForParticleSystem(CollisionGroupName name1, TransformComponent* transComp_1,
		CollisionGroupName collisionGroupName1, TransformComponent* transComp_2, CollisionGroupName collisionGroupName2, ParticleType particleType);

public:
	ParticleSystem(EntityManager* entityManagerPointer, GraphicsSystem* graphics);
	~ParticleSystem();

	void Initialize();
	void Update(float dt);
	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};

#endif // !PARTICLESYSTEM_H
