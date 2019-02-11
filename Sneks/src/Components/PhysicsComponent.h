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
	float		               m_f_MaxSpeed = 900;
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