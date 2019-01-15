
#include "SystemManager.h"
#include <iostream>

//Errors > 2000

SystemManager::SystemManager(Logger* logger)
{
	m_o_Logger = logger;
}

SystemManager::~SystemManager()
{

}

void SystemManager::Initialize()
{
	
}

void SystemManager::AddSystem(System* NewSystem)
{
	if (NewSystem != 0)
	{
		SystemList.push_back(NewSystem);
	}
	else
	{
		m_o_Logger->LogMessage(LOGGER_SYSTEM, "Error 2002 : System %s has not been initialised", NewSystem->GetName());
	}
}

//Needs to be tested. May just erase all systems
void SystemManager::RemoveSystem(System* RemSystem)
{
	for (std::vector<System*>::iterator sys = SystemList.begin();sys != SystemList.end();)
	{
		if (typeid(*sys) == typeid(*RemSystem))
		{
			try
			{
				SystemList.erase(sys);
			}
			catch (...)
			{
				m_o_Logger->LogMessage(LOGGER_SYSTEM, "Error 2003 : Error removing System %s", typeid(RemSystem).name());
			}
		}
		else
		{
			sys++;
		}
	}
}

void SystemManager::Update()
{
	for (System* currSystem : SystemList)
	{
		currSystem->Update();
	}
}




