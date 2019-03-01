
#include "SystemManager.h"
#include <iostream>
#include "../Utility/FileIO.h"

#define LOG_SYSTEM_UPDATE_TIME //DEFINE THIS IF YOU WANT SYSTEM LOGGING

#ifdef LOG_SYSTEM_UPDATE_TIME
#define __STDC_WANT_LIB_EXT1__ 1
#include <chrono>

std::vector<float> fpsLog;
std::vector<const char*> nameLog;
std::vector<f64> timeLog;
std::vector<f64> totalTime(20);
int totalFrames = 0;
f64 timeElapsed = 0;
#endif

SystemManager::SystemManager(Logger* logger)
{
	m_o_Logger = logger;
}

SystemManager::~SystemManager()
{
#ifdef LOG_SYSTEM_UPDATE_TIME
	{
		int counter = 0;

		char fileName[100] = {'\0'};
		char timeDate[100] = {'\0'};

		strcpy_s(fileName, sizeof(fileName), "../Logs/time_log_");

		auto time = std::time(nullptr);

		struct tm timeinfo;

		localtime_s(&timeinfo, &time);

		strftime(timeDate, sizeof(timeDate), "%d%m%y %H.%M.%S", &timeinfo);

		strcat_s(fileName, sizeof(fileName),timeDate);
		strcat_s(fileName, sizeof(fileName), ".txt\0");
		
		std::ofstream outFile(fileName, std::ios::trunc);
		//Asset if file is open
		if (outFile.is_open())
		{
			float lowestFps = 60;
			float avgFps = 60;
			for (unsigned int i = 0; i < fpsLog.size(); i ++)
			{
				if (fpsLog[i] <= lowestFps)
				{
					lowestFps = fpsLog[i];
				}
				avgFps = (avgFps * i + fpsLog[i]) / (i + 1);
			}
			//Write string to file
			for (unsigned int i = 0; i < timeLog.size(); i++) {
				outFile << nameLog[i] << " took " << timeLog[i] << " ms " << std::endl;
				totalTime[counter]+=timeLog[i];
				counter = ++counter % m_v_SystemList.size();
			}

			outFile << std::endl << std::endl << "Breakdown: " << std::endl << std::endl;

			f64 totalTotalTime = 0;
			for (unsigned i = 0; i < m_v_SystemList.size(); i++)
			{
				totalTotalTime+=totalTime[i];
				outFile << nameLog[i] << " took a total of " << totalTime[i] << " ms." << std::endl;
			}

			outFile << std::endl << "There were " << m_v_SystemList.size() << " systems. " << std::endl << std::endl;

			for (unsigned i = 0; i < m_v_SystemList.size(); i++)
			{
				outFile << nameLog[i] << " took " << totalTime[i] / totalTotalTime * 100 << "%% of the time." << std::endl;
			}
			outFile << "A total of " << timeElapsed << " ms elapsed during this session. " << std::endl
					  <<  "Average FPS was " << avgFps << std::endl
					  <<  "Lowest FPS was "  << lowestFps << std::endl;
		}
		nameLog.clear();
		timeLog.clear();
		fpsLog.clear();
		outFile.close();
	}
#endif

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

//TODO:: MOVE STUFF TO LOGGING 

void SystemManager::Update(float dt)
{
#ifdef LOG_SYSTEM_UPDATE_TIME
	fpsLog.push_back( 1.0f / static_cast<float>(AEFrameRateControllerGetFrameTime() ) );
#endif

	for (BaseSystem* currSystem : m_v_SystemList)
	{
#ifdef LOG_SYSTEM_UPDATE_TIME
		auto preTime = AEGetTime(nullptr);
#endif

		currSystem->Update(dt);

#ifdef LOG_SYSTEM_UPDATE_TIME
		auto timeToUpdate = AEGetTime(nullptr) - preTime;
		totalFrames++;
		timeToUpdate *= 1000;
		timeElapsed += timeToUpdate;
		nameLog.push_back(currSystem->GetName());
		timeLog.push_back(timeToUpdate);
#endif

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

