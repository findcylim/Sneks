#ifndef INVULNERABLE_COMPONENT_H
#define INVULNERABLE_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"

class InvulnerableComponent : public BaseComponent
{
public:
	float m_f_InvulnerableTime = 0;
};

#endif //INVULNERABLE_COMPONENT_H