#pragma once


#include "EventManager.h"
#include "SystemManager.h"
#include "../Utility/Logger.h"



class ECSEngine
{
private:
	
	Logger* logger; //Logger object to log objects into the log file. 
	bool EngineStatus;
public:
	ECSEngine();
	~ECSEngine();
	void InitializeEngine();
	bool IsEngineOn();
	void Update();
};

