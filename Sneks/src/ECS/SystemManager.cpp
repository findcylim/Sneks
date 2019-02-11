
#include "SystemManager.h"
#include <iostream>

//Errors > 2000

SystemManager::SystemManager(Logger* logger)
{
	m_o_Logger = logger;
}

SystemManager::~SystemManager()
{
	while (!m_v_SystemList.empty())
	{
		delete(m_v_SystemList.back());
		m_v_SystemList.pop_back();
	}
}

void SystemManager::Initialize(EventManager* eventManager, EntityManager* entManager)
{
	m_o_EventManager = eventManager;
	m_o_EntityManager = entManager;
}

void SystemManager::AddSystem(BaseSystem* NewSystem)
{
	if (NewSystem != nullptr)
	{
		NewSystem->m_o_EventManagerPtr = m_o_EventManager;
		NewSystem->m_po_EntityManager = m_o_EntityManager;
		NewSystem->m_po_ComponentManager = m_o_EntityManager->GetComponentManager();
		m_v_SystemList.push_back(NewSystem);
	}
	else
	{
		m_o_Logger->LogMessage(LOGGER_SYSTEM, "Error 2002 : System %s has not been initialised", NewSystem->GetName());
	}
}

//Needs to be tested. May just erase all systems
void SystemManager::RemoveSystem(BaseSystem* RemSystem)
{
	for (std::vector<BaseSystem*>::iterator sys = m_v_SystemList.begin();sys != m_v_SystemList.end();)
	{
		if (typeid(*sys) == typeid(*RemSystem))
		{
			try
			{
				delete(*sys);
				m_v_SystemList.erase(sys);
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

void SystemManager::Update(float dt)
{
	for (BaseSystem* currSystem : m_v_SystemList)
	{
		currSystem->Update(dt);
	}
}



BaseSystem* SystemManager::GetSystem(int ID)
{
	for (BaseSystem* currSystem : m_v_SystemList)
	{
		if (currSystem->GetID() == ID)
		{
			return currSystem;
		}
	}
	return nullptr;
}

