/* Start Header****************************************************************/
/*!
\file InvulnerableComponent.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains a component that allows entities to become
impervious to other enemy attacks for a set time. 

\par Contribution : CY - 100.00%

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

struct InvulnerableComponent : public BaseComponent
{
	float m_f_InvulnerableTime = 0;
	float m_f_MaxAlphaBlinking = 0.5f;
	float m_f_MinAlphaBlinking = 0.1f;
	float m_f_BlinkSpeed = 2.0f;
	bool m_b_IsAlive = false;
};

#endif //INVULNERABLE_COMPONENT_H