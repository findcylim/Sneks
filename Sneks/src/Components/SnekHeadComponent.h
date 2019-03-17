#ifndef SNEK_HEAD_COMPONENT_H
#define SNEK_HEAD_COMPONENT_H
#pragma once
#include "../ECS/ComponentList.h"
#include "AEInput.h"


class SnekHeadComponent : public BaseComponent
{
public:
	float m_f_AccelerationForce            = 200;
	float m_f_TurnSpeed                    = 6;
	float m_f_IdleSpeed                    = 100;		//default move speed

	//unused for now
	float m_f_Friction                     = 1.5f;
	float m_f_BrakeForce                   = 6;
	float m_f_TurnMinSpeed                 = 60;       //need to be moving at this speed to turn
	float m_f_MinSpeed                     = 300;
	float m_f_MaxVelocity                  = 900;
	float m_f_BoostSetCooldown             = 3;
	float m_f_BoostCooldown                = 0;


	unsigned short m_i_PlayerNumber        = 0;
	unsigned short m_i_SnekType            = 0;
	unsigned int   m_i_AccelerationKey     = AEVK_UP;
	unsigned int   m_i_BrakeKey            = AEVK_DOWN;
	unsigned int   m_i_LeftKey             = AEVK_LEFT;
	unsigned int   m_i_RightKey            = AEVK_RIGHT;
	unsigned int   m_i_BoostKey            = AEVK_RCTRL;
	unsigned int   m_i_SpecialKey          = AEVK_RSHIFT;


	std::vector<SnekBodyEntity*> m_x_BodyParts = {};

};

#endif