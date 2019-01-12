#ifndef ECSYSTEM_H
#define ECSYSTEM_H
#pragma once


#include "EventManager.h"
#include "SystemManager.h"
#include "../Utility/Logger.h"



class ECSystem
{
private:
	EventManager* m_EventManager;
	SystemManager* m_SystemManager;
	bool EngineStatus;
public:
	ECSystem();
	~ECSystem();
	void InitializeEngine();
	bool IsEngineOn();
	void Update();
};

#endif