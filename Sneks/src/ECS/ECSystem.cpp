
#include "ECSystem.h"
#include "../Utility/GameStateManager.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"


#include <Windows.h>
ECSystem::ECSystem()
{
	m_o_Logger					= new Logger("log.txt");
	m_o_EventManager			= new EventManager(m_o_Logger);
	m_o_SystemManager			= new SystemManager(m_o_Logger);
	m_o_GameStateManager		= new GameStateManager(kStateGame);
	m_o_EntityComponentManager	= new EntityManager();
	m_b_EngineStatus			= false;
}


ECSystem::~ECSystem()
{
	delete(m_o_Logger);
	delete(m_o_SystemManager);
	delete(m_o_EventManager);
	delete(m_o_GameStateManager);
	delete(m_o_EntityComponentManager);
}

/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSystem::InitializeEngine()
{
	//TODO change this state to splash screen/main menu in the future
	

	m_o_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	m_o_Logger->LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_o_SystemManager->Initialize();
	/*
		Create and add Systems here
	*/
	m_o_SystemManager->AddSystem(new PhysicsSystem(m_o_EventManager,m_o_GameStateManager));


	m_b_EngineStatus = true;
}

bool ECSystem::IsEngineOn()
{
	return m_b_EngineStatus;
}

void ECSystem::Update()
{
	m_o_EventManager->Update();
	m_o_SystemManager->Update();

	if (GetAsyncKeyState(VK_LSHIFT) < 0)
	{
		m_b_EngineStatus = false;
	}
}

