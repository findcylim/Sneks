/* Start Header ***************************************************************/
/*!
\file SnekHeadComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef SNEK_HEAD_COMPONENT_H
#define SNEK_HEAD_COMPONENT_H
#pragma once
#include "../ECS/ComponentList.h"
#include "AEInput.h"

enum SnekType : unsigned char
{
	kSnekTypeSpeed,
	kSnekTypeShoot,
	kSnekTypeFlip
};


struct SnekHeadComponent : public BaseComponent
{
	float m_f_AccelerationForce            = 200;
	float m_f_TurnSpeed                    = 6;
	float m_f_IdleSpeed                    = 100;		//default move speed

	float m_f_BoostSetCooldown             = 3;
	float m_f_BoostCooldown                = m_f_BoostSetCooldown;

	float m_f_Friction                     = 1.5f;

	//unused for now
	//float m_f_BrakeForce                   = 6;
	//float m_f_TurnMinSpeed                 = 60;       //need to be moving at this speed to turn
	//float m_f_MinSpeed                     = 300;
	//float m_f_MaxVelocity                  = 900;


	float m_f_CurrentGrowth = 0;
	float m_f_GrowthMeter   = 5;

	int m_i_LivesLeft = 3;
	int m_i_CurrentDamage = 3;
	int m_i_BaseDamage = 3;

	unsigned short m_i_PlayerNumber        = 0;
	SnekType m_x_SnekType                  = kSnekTypeSpeed;
	unsigned int   m_i_AccelerationKey     = AEVK_UP;
	unsigned int   m_i_BrakeKey            = AEVK_DOWN;
	unsigned int   m_i_LeftKey             = AEVK_LEFT;
	unsigned int   m_i_RightKey            = AEVK_RIGHT;
	unsigned int   m_i_BoostKey            = AEVK_RCTRL;
	//unsigned int   m_i_SpecialKey          = AEVK_RSHIFT;


	std::vector<SnekBodyEntity*> m_x_BodyParts = {};

};

#endif