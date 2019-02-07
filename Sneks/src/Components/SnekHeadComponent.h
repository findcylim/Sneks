#ifndef SNEK_HEAD_COMPONENT_H
#define SNEK_HEAD_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"


class SnekHeadComponent : public BaseComponent
{
public:
	float m_f_MaxVelocity       = 900;
	float m_f_AccelerationForce = 6;
	float m_f_BrakeForce        = 6;
	float m_f_TurnSpeed         = 6;
	float m_f_Friction          = 1.5f;	   //natural slowdown
	float m_f_TurnMinSpeed      = 60;      //need to be moving at this speed to turn
	float m_f_MinSpeed          = 300;	   //if speed lower than this then clamp to 0
	float m_f_IdleSpeed         = 450;		//default move speed
};

#endif