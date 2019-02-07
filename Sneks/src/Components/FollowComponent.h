#ifndef FOLLOW_COMPONENT_H
#define FOLLOW_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"
#include "TransformComponent.h"


class PhysicsComponent : public BaseComponent
{
public:
	TransformComponent*		m_po_TransformComponent;
};

#endif