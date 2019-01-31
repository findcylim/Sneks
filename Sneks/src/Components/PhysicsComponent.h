#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"

class PhysicsComponent : public BaseComponent
{
public:
	float		 m_f_Velocity;
	HTVector2 m_x_Acceleration;
};

#endif