/* Start Header****************************************************************/
/*!
\file BloomComponent.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef BLOOM_COMPONENT_H
#define BLOOM_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"


struct BloomComponent : public BaseComponent
{
public:
	float m_f_BaseBloomStrength = 0.6f;
	float m_f_BloomStrength = m_f_BaseBloomStrength; // 0.33f default alpha of drawn blurs
	float m_f_BloomDiffuse = 2.0f;   // pixel offset of drawn blurs
	int   m_i_BloomIterations = 2;   // number of drawn blurs
	
	bool m_b_FlashingBloom = false;
	float m_f_FlashingMagnitude = 1.0f;
	float m_f_FlashingSpeed = 0.2f;
	float m_f_FlashingStrengthMin = 0.1f;
	float m_f_FlashingStrengthMax = 0.3f;
};
#endif