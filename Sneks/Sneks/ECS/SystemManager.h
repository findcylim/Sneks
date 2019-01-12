#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#pragma once

#include <vector>
#include "EventManager.h"
#include "System.h"





class SystemManager
{
private:
	std::vector<System*> SystemList;
public:
	void AddSystem(System* NewSystem);
	void RemoveSystem(System* RemSystem);
	void Update();
	void Initialize();
	SystemManager();
	~SystemManager();
};

#endif
