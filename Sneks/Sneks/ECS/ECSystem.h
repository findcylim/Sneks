#ifndef ECSYSTEM_H
#define ECSYSTEM_H
#pragma once


#include "EventManager.h"
#include "SystemManager.h"
#include "../Utility/GameStateManager.h"
#include "../Utility/Logger.h"



class ECSystem
{
private:
	GameStateManager* m_o_GameStateManager;
	EventManager* m_o_EventManager;
	SystemManager* m_o_SystemManager;
	bool m_b_EngineStatus;
public:
	ECSystem();
	~ECSystem();
	void InitializeEngine();
	bool IsEngineOn();
	void Update();
};

#endif