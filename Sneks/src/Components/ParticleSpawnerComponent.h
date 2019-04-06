/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 01/04/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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
	AEGfxTexture* m_px_ParticleTexture = nullptr;
	int m_i_ParticleDrawOrder = 3;
	int m_i_SplitNumber = 0;
	int m_i_CurrentSplit = -1;

	//for transform location
	bool m_b_ParticleFixOffset = false;
	bool m_b_ParticleFixDirection = false;

	float m_f_SpreadDistance = 0.0f;
	//Angle stored as radian.
	float m_f_AngleForSpreadDistance = 0.0f;
	//Angle stored as radian.
	float m_f_SpreadAngle = 0.0f;
	float m_f_OffsetDistance = 0.0f;
	//Angle stored as radian.
	float m_f_AngleForOffsetDistance = 0.0f;
	//Angle stored as radian.
	float m_f_OffsetAngle = 0.0f;
	float m_f_ParticleSpeed = 0.0f;
	float m_f_ParticleSpawnFrequency = 0.0f;
	int m_i_ParticleSpawnDensity = 0;
	float m_f_ParticleMaxLifetime = 0.0f;

	float m_f_ParticleStartingAlpha = 1.0f;
	float m_f_ParticleFadeAmount = 1.0f;

	//float m_f_ParticleEffectMaxLifetime = 0.0f;
	float m_f_ParticleEffectRemainingLifetime = 0.0f;
	float m_f_ParticleEffectSpawnTimer = 0.0f;
	bool m_b_IsParticleEffectEternal = false;
	bool m_b_IsParticleEffectOneShot = false;
	bool m_b_HasParticleEffectFired = false;
	TransformComponent* m_po_SpawnTransform = nullptr;
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
	ParticleType m_x_ParticleType = kParticleEmpty;

};
