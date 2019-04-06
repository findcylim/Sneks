/* Start Header****************************************************************/
/*!
\file ECSystem.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 35.53%  (Equal with the rest + Update)
\par Contribution : Javier - 21.32%  (Equal with the rest)
\par Contribution : Edwin  - 21.32%  (Equal with the rest)
\par Contribution : Adam   - 21.32%  (Equal with the rest)

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