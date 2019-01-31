
#include "SystemManager.h"
#include <iostream>

//Errors > 2000

SystemManager::SystemManager(Logger* logger)
{
	m_o_Logger = logger;
}

SystemManager::~SystemManager()
{
	for (std::vector<BaseSystem*>::iterator sys = SystemList.begin(); sys != SystemList.end();)
	{
		delete(*sys);
		SystemList.erase(sys);
		if (SystemList.size() == 0)
			break;
	}
}

void SystemManager::Initialize()
{
	
}

void SystemManager::AddSystem(BaseSystem* NewSystem)
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
void SystemManager::RemoveSystem(BaseSystem* RemSystem)
{
	for (std::vector<BaseSystem*>::iterator sys = SystemList.begin();sys != SystemList.end();)
	{
		if (typeid(*sys) == typeid(*RemSystem))
		{
			try
			{
				delete(*sys);
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
	for (BaseSystem* currSystem : SystemList)
	{
		currSystem->Update();
	}
}




