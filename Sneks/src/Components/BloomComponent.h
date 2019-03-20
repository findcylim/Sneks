#ifndef BLOOM_COMPONENT_H
#define BLOOM_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"


class BloomComponent : public BaseComponent
{
public:
	float m_f_BloomStrength = 0.6f; // 0.33f default alpha of drawn blurs
	float m_f_BloomDiffuse = 2.0f;   // pixel offset of drawn blurs
	int   m_i_BloomIterations = 2;   // number of drawn blurs
	
	bool m_b_FlashingBloom = false;
	float m_f_FlashingMagnitude = 1.0f;
	float m_f_FlashingSpeed = 0.2f;
	float m_f_FlashingStrengthMin = 0.1f;
	float m_f_FlashingStrengthMax = 0.3f;
};
#endif