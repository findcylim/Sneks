#ifndef INVULNERABLE_COMPONENT_H
#define INVULNERABLE_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"

class InvulnerableComponent : public BaseComponent
{
public:
	float m_f_InvulnerableTime = 0;

	float m_f_MaxAlphaBlinking = 0.5f;
	float m_f_MinAlphaBlinking = 0.1f;
	float m_f_BlinkSpeed = 2.0f;
};

#endif //INVULNERABLE_COMPONENT_H