#include "SystemManager.h"
#include "../Utility/Utility.h"
#include <iostream>

//Errors > 2000
void System::Update()
{
	std::cout << "Error 2001 : Updating a non overriden system" << std::endl;
}


short System::GetID()
{
	return SystemID;
}
const char * System::GetName()
{
	return SystemName;
}
void System::SetID(short id)
{
	SystemID = id;
}
void System::SetName(const char* name)
{
	SystemName = name;
}

SystemManager::SystemManager(Utility* UtilityPtr)
{
	m_UtilityPtr = UtilityPtr;
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
		m_UtilityPtr->m_Logger->LogMessage("Error 2002 : System %s has not been initialised", NewSystem->GetName());
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




