#include "SystemManager.h"



SystemManager::SystemManager()
{

}

SystemManager* SystemManager::instance = 0;

SystemManager::~SystemManager()
{

}


SystemManager* SystemManager::Instance()
{
	if (!SystemManager::instance)
	{
		SystemManager::instance = new SystemManager;
	}
	return SystemManager::instance;
}

void SystemManager::AddSystem(System* NewSystem)
{
	if (NewSystem != 0)
	{
		SystemManager::SystemList.push_back(NewSystem);
	}
	else
	{

	}
}

void SystemManager::Update()
{

}




