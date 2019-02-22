#ifndef FOLLOW_COMPONENT_H
#define FOLLOW_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "TransformComponent.h"


class FollowComponent : public BaseComponent
{
public:
	TransformComponent*		m_po_TransformComponent;
	BaseEntity *			   m_po_ParentEntity;
};

#endif