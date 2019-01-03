#pragma once


#include "EventManager.h"
#include "SystemManager.h"
#include "Logger.h"



class ECSEngine
{
private:
	Logger* logger;
	bool EngineStatus;
public:
	ECSEngine();
	~ECSEngine();
	void InitializeEngine();
	bool IsEngineOn();
	void Update();
};

