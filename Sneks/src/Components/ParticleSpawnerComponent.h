/* Start Header****************************************************************/
/*!
\file ParticleSpawnerComponent.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that sets the prefabs of particles.

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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
	kParticleSparks,
	kParticleText
};

struct ParticleSpawnerComponent : public BaseComponent
{
	//texture of to feed into the particle's draw
	AEGfxTexture* m_px_ParticleTexture = nullptr;
	//draw order of the particle
	int m_i_ParticleDrawOrder = 3;
	//number of split the particles should have;
	//number of streams the particle will split into
	int m_i_SplitNumber = 0;
	//determines which stream the current particle should be in
	int m_i_CurrentSplit = -1;

	//bool to check if the offset and direction particle should move and spawn
	//is fixed
	//for transform location
	bool m_b_ParticleFixOffset = false;
	bool m_b_ParticleFixDirection = false;

	//spreading distance for the spawn;
	//how far it spreads when it's spawning
	float m_f_SpreadDistance = 0.0f;
	//angle for its spread
	//Angle stored as radian.
	float m_f_AngleForSpreadDistance = 0.0f;
	//angle to spread
	//Angle stored as radian.
	float m_f_SpreadAngle = 0.0f;
	//the distance offset to spawn
	float m_f_OffsetDistance = 0.0f;
	//the angle for the distance offset
	//Angle stored as radian.
	float m_f_AngleForOffsetDistance = 0.0f;
	//angle to offset from its spawner
	//Angle stored as radian.
	float m_f_OffsetAngle = 0.0f;
	//speed particle moves
	float m_f_ParticleSpeed = 0.0f;
	//frequency the particle spawns
	float m_f_ParticleSpawnFrequency = 0.0f;
	//total density per frequency
	int m_i_ParticleSpawnDensity = 0;
	//max life of the particle
	float m_f_ParticleMaxLifetime = 0.0f;

	//starting alpha of the particle
	float m_f_ParticleStartingAlpha = 1.0f;
	//fade amount of the particle
	float m_f_ParticleFadeAmount = 1.0f;

	//max life of the spawner
	//float m_f_ParticleEffectMaxLifetime = 0.0f;
	//remaining life of the spawner
	float m_f_ParticleEffectRemainingLifetime = 0.0f;
	float m_f_ParticleEffectSpawnTimer = 0.0f;
	//if the particle spawner never dies
	bool m_b_IsParticleEffectEternal = false;
	//if the particle spawner only shoots one time
	bool m_b_IsParticleEffectOneShot = false;
	//if the particle has fired yet
	bool m_b_HasParticleEffectFired = false;
	//pointer to the transform component the spawner is attached to
	TransformComponent* m_po_SpawnTransform = nullptr;
	//size of the particle
	HTVector2 m_f_ParticleSize ={ 1.0f, 1.0f };
	bool m_b_UseStaticLocation = false;
	int m_i_SpriteCountX = 0;
	int m_i_SpriteCountY = 0;
	float m_i_SecondsPerFrame = 0.5f;
	bool m_b_AlphaScalingEnabled = true;
	int m_i_RandomStartFrame = 0;
	HTColor m_x_ParticleEffectColor ={ 1.0f,1.0f,1.0f,1.0f };

	void SetParticleType(ParticleType type, GraphicsSystem* graphics);
	void SetPickUpText(AEGfxTexture* texture);
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
	//type of particle to spawn
	ParticleType m_x_ParticleType = kParticleEmpty;

};
