/* Start Header****************************************************************/
/*!
\file ParticleSystem.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


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
	int m_i_CreatedTrails = 0;

	void Initialize();
	void ResetTrails();
	void Update(float dt);
	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
};

#endif // !PARTICLESYSTEM_H
