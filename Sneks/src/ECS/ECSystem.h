#ifndef ECSYSTEM_H
#define ECSYSTEM_H
#pragma once
#include <minwindef.h> //HINSTANCE typdef


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
	void LoadMainMenu();
public:
	ECSystem();
	~ECSystem();
	void InitializeEngine();
	bool IsEngineOn() const;
	void Update();
};

float getDt();

#endif