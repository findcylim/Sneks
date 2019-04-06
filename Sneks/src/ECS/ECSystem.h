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