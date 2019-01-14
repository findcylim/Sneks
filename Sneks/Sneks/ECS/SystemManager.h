#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#pragma once

#include <vector>
#include "EventManager.h"
#include "System.h"
#include "../Utility/Logger.h"




class SystemManager
{
private:
	std::vector<System*> SystemList;
	Logger* m_o_Logger;
public:
	void AddSystem(System* NewSystem);
	void RemoveSystem(System* RemSystem);
	void Update();
	void Initialize();
	SystemManager(Logger* logger);
	~SystemManager();
};

#endif
