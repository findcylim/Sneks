#include "SystemManager.h"
#include "../Utility.cpp"
#include <iostream>

//Errors > 2000
void System::Update()
{
	cout << "Error 2001 : Updating a non overriden system" << endl;
}

SystemManager::SystemManager()
{

}

SystemManager::~SystemManager()
{

}

SystemManager* SystemManager::instance = 0;


SystemManager* SystemManager::Instance()
{
	if (!instance)
	{
		instance = new SystemManager;
	}
	return instance;
}

void SystemManager::Initialize(Logger* logger)
{
	LogObj = logger;
}

void SystemManager::AddSystem(System* NewSystem)
{
	if (NewSystem != 0)
	{
		SystemList.push_back(NewSystem);
	}
	else
	{
		LogObj->LogMessage("Error 2002 : System has not been initialised");
	}
}

//Needs to be tested. May just erase all systems
void SystemManager::RemoveSystem(System* RemSystem)
{
	/*for (std::vector<System*>::iterator sys = SystemList.begin;sys != SystemList.end;sys++)
	{
		if (typeid(*sys) == typeid(*RemSystem))
		{
			SystemList.erase(sys);
		}
	}*/
}



void SystemManager::Update()
{
	for (System* currSystem : SystemList)
	{
		currSystem->Update();
	}
}




