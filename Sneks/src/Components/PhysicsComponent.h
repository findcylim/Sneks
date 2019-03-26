/* Start Header ***************************************************************/
/*!
\file PhysicsComponent.h
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

#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"
#include "TransformComponent.h"


class PhysicsComponent : public BaseComponent
{
public:
	float		               m_f_Speed;
	float		               m_f_MaxSpeed = 1200;
	float							m_f_Acceleration;
	HTVector2					m_x_Velocity ={};
	float						   m_f_Mass = 10;

	TransformComponent*		m_po_TransformComponent;

	void SetVelocity(HTVector2 velocity)
	{
		float newRot = atan2(velocity.y, velocity.x);
		float magnitude = sqrt(velocity.x * velocity.x + velocity.y*velocity.y) ;
		m_po_TransformComponent->SetRotation(newRot);
		m_f_Speed = magnitude * 1.5f;
	}
};
#endif