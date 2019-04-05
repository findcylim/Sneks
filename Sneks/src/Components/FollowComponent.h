/* Start Header ***************************************************************/
/*!
\file FollowComponent.h
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