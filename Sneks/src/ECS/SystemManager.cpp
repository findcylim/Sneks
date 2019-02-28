
#include "SystemManager.h"
#include <iostream>
#include "../Utility/FileIO.h"

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

size_t SystemManager::GetSystemCount() const
{
	return m_v_SystemList.size();
}

void SystemManager::AddSystem(BaseSystem* NewSystem)
{
	if (NewSystem != nullptr)
	{
		NewSystem->m_o_EventManagerPtr = m_o_EventManager;
		NewSystem->m_po_EntityManager = m_o_EntityManager;
		NewSystem->m_po_ComponentManager = m_o_EntityManager->GetComponentManager();
		NewSystem->SetID(static_cast<short>(GetSystemCount()));
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
std::vector<const char*> nameLog;
std::vector<f64> timeLog;
std::vector<f64> totalTime(20);
f64 timeElapsed = 0;
bool written = false;
void SystemManager::Update(float dt)
{
	
	for (BaseSystem* currSystem : m_v_SystemList)
	{
		auto preTime = AEGetTime(nullptr);
		currSystem->Update(dt);
		auto timeToUpdate = AEGetTime(nullptr) - preTime;
		timeToUpdate *= 1000;
		timeElapsed += timeToUpdate;
		nameLog.push_back(currSystem->GetName());
	//	timeLog.push_back(timeToUpdate);
	//	if (GetAsyncKeyState(AEVK_K) && !written) {
	//		written = true;
	//		int counter = 0;
	//		std::ofstream outFile("./timeLog.txt", std::ios::app);
	//		//Asset if file is open
	//		if (outFile.is_open())
	//		{
	//			//Write string to file
	//			for (unsigned int i = 0; i < timeLog.size(); i++) {
	//				outFile << nameLog[i] << " took " << timeLog[i] << " ms " << std::endl;
	//				totalTime[counter]+=timeLog[i];
	//				counter = ++counter % m_v_SystemList.size();
	//			}

	//			outFile << std::endl << std::endl << "Breakdown: " << std::endl << std::endl;

	//			f64 totalTotalTime = 0;
	//			for (unsigned i = 0; i < m_v_SystemList.size(); i++)
	//			{
	//				totalTotalTime+=totalTime[i];
	//				outFile << nameLog[i] << " took a total of " << totalTime[i] << " ms." << std::endl;
	//			}

	//			outFile << std::endl << "There were " << m_v_SystemList.size() << " systems. " << std::endl << std::endl;

	//			for (unsigned i = 0; i < m_v_SystemList.size(); i++)
	//			{
	//				outFile << nameLog[i] << " took " << totalTime[i] / totalTotalTime * 100 << "%% of the time." << std::endl;
	//			}
	//			outFile << "A total of " << timeElapsed << " ms elapsed during this session. " << std::endl;
	//		}
	//		nameLog.clear();
	//		timeLog.clear();
	//		outFile.close();
	//	}else
	//	{
	//		written = false;
	//	}
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

