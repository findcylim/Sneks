#ifndef INVULNERABLE_COMPONENT_H
#define INVULNERABLE_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"

class InvulnerableComponent : public BaseComponent
{
public:
	int m_f_InvulnerableTime;
};

#endif //INVULNERABLE_COMPONENT_H