/* Start Header****************************************************************/
/*!
\file FollowComponent.h
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

#ifndef FOLLOW_COMPONENT_H
#define FOLLOW_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "TransformComponent.h"


struct FollowComponent : public BaseComponent
{
	TransformComponent*		m_po_FolloweeTransform;
	BaseEntity *			   m_po_ParentEntity;
	float						   m_f_ScaleFactorMultiplier = 0.3f;
	float							m_f_StretchThreshold = 100.0f;
};

#endif