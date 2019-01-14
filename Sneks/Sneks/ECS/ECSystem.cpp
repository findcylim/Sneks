
#include "ECSystem.h"
#include "../Systems/PlayerSystem.h"
#include "../Utility/GameStateManager.h"
#include "../Systems/InputSystem.h"


ECSystem::ECSystem()
{
	m_o_Logger				= new Logger("log.txt");
	m_o_EventManager		= new EventManager(m_o_Logger);
	m_o_SystemManager		= new SystemManager(m_o_Logger);
	m_o_GameStateManager	= new GameStateManager(state_Game);
	m_b_EngineStatus		= false;
}


ECSystem::~ECSystem()
{
	//TODO Call logger destroy function here later on
	delete(m_o_Logger);
	delete(m_o_EventManager);
	delete(m_o_SystemManager);
}




/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSystem::InitializeEngine()
{
	//TODO change this state to splash screen/mainmenu in the future
	

	m_o_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	m_o_Logger->LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_o_SystemManager->Initialize();
	/*
		Create and add Systems here
	*/
	m_o_SystemManager->AddSystem(new InputSystem(m_o_EventManager, 0, "InputSystem",m_o_GameStateManager,m_o_Logger));


	//PlayerSystem TEST SYSTEM
	PlayerSystem* playerSystem = new PlayerSystem(m_o_EventManager);
	playerSystem->SetID(1);
	playerSystem->SetName("PlayerSystem");
	m_o_SystemManager->AddSystem(playerSystem);

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
}

