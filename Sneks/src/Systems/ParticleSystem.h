
#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/ParticleSpawnerComponent.h"

class ParticleSystem final : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	GraphicsSystem* m_o_GraphicsSystem;

	void SpawnParticle(ParticleSpawnerComponent* particleEffectComp);
	ParticleSpawnerComponent* CreateParticleSpawner(TransformComponent* spawnTransform, ParticleType particleType) const;

	float CalculateRotation(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp);
	float CalculatePositionX(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp);
	float CalculatePositionY(ParticleSpawnerComponent* particleEffectComp, TransformComponent* transformComp);
	bool CollisionCheckForParticleSystem(CollisionGroupName collisionGroupName1, TransformComponent* transComp_1,
		CollisionGroupName collisionGroupName2, TransformComponent* transComp_2, CollisionGroupName collGroupToCheck,
		TransformComponent** outTransformComponent);

public:
	ParticleSystem(GraphicsSystem* graphics);
	~ParticleSystem();

	void Initialize();
	void Update(float dt);
	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};

#endif // !PARTICLESYSTEM_H
