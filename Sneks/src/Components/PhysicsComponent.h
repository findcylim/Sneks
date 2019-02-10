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

	TransformComponent*		m_po_TransformComponent;
};
#endif