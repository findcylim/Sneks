/* Start Header****************************************************************/
/*!
\file PhysicsComponent.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains data that allows entities to move around using
world physics.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"
#include "TransformComponent.h"


struct PhysicsComponent : public BaseComponent
{
	float		               m_f_Speed;
	float		               m_f_MaxSpeed = 1200;
	float					   m_f_Acceleration;
	float					   m_f_TurnSpeedMultiplier = 1.0f;
	HTVector2					m_x_Velocity = {0,0};
	float						   m_f_Mass = 10;

	TransformComponent*		m_po_TransformComponent;

	inline void SetVelocity(HTVector2 velocity)
	{
		float newRot = atan2(velocity.y, velocity.x);
		float magnitude = sqrt(velocity.x * velocity.x + velocity.y*velocity.y) ;
		m_po_TransformComponent->SetRotation(newRot);
		m_f_Speed = magnitude * 1.5f;
	}
};
#endif