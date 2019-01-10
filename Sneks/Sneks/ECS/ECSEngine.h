#pragma once


#include "EventManager.h"
#include "SystemManager.h"
#include "../Utility/Utility.h"



class ECSEngine
{
private:
	EventManager* m_EventManager;
	SystemManager* m_SystemManager;
	Utility* m_Utility;
	bool EngineStatus;
public:
	ECSEngine();
	~ECSEngine();
	void InitializeEngine();
	bool IsEngineOn();
	void Update();
};

