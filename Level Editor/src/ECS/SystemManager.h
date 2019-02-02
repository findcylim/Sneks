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
	std::vector<BaseSystem*> SystemList;
	Logger* m_o_Logger;
public:
	void AddSystem(BaseSystem* NewSystem);
	void RemoveSystem(BaseSystem* RemSystem);
	BaseSystem* GetSystem(int id);
	void Update();
	void Initialize();
	SystemManager(Logger* logger);
	~SystemManager();
};

#endif
