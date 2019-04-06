/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef ECSYSTEM_H
#define ECSYSTEM_H
#pragma once

#include "EventManager.h"
#include "SystemManager.h"
#include "../Utility/GameStateManager.h"
#include "../Utility/Logger.h"
#include "EntityManager.h"


class ECSystem
{
private:
	GameStateManager*	 m_o_GameStateManager;
	Logger*				 m_o_Logger;
	EventManager*		 m_o_EventManager;
	SystemManager*		 m_o_SystemManager;
	EntityManager*		 m_o_EntityComponentManager;
	bool					 m_b_EngineStatus;
public:
	ECSystem();
	~ECSystem();
	void LoadLevel1();
	void InitializeEngine();
	bool IsEngineOn() const;
	void Update();
};

float GetUncappedDt();
float GetCappedDt();
float SetTimeScale(float timeScale);

#endif