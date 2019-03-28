/* Start Header ***************************************************************/
/*!
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef INVULNERABLE_COMPONENT_H
#define INVULNERABLE_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"

class InvulnerableComponent : public BaseComponent
{
public:
	float m_f_InvulnerableTime = 0;

	float m_f_MaxAlphaBlinking = 0.5f;
	float m_f_MinAlphaBlinking = 0.1f;
	float m_f_BlinkSpeed = 2.0f;
};

#endif //INVULNERABLE_COMPONENT_H