
#include "ECSystem.h"
#include "../Systems/PlayerSystem.h"
#include "../Utility/GameStateManager.h"
#include "../Systems/InputSystem.h"


ECSystem::ECSystem()
{
	m_EventManager	= new EventManager();
	m_SystemManager = new SystemManager();
	EngineStatus	= false;
}


ECSystem::~ECSystem()
{
	//TODO Call logger destroy function here later on
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
	GameStateManager::GameStateInit(state_Game);

	m_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	Logger::LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_SystemManager->Initialize();
	/*
		Create and add Systems here
	*/
	m_SystemManager->AddSystem(new InputSystem(m_EventManager, 0, "InputSystem"));


	//PlayerSystem TEST SYSTEM
	PlayerSystem* PlayerS = new PlayerSystem(m_EventManager);
	PlayerS->SetID(1);
	PlayerS->SetName("PlayerSystem");
	m_SystemManager->AddSystem(PlayerS);

	EngineStatus = true;
}

bool ECSystem::IsEngineOn()
{
	return EngineStatus;
}


void ECSystem::Update()
{
	m_EventManager->Update();
	m_SystemManager->Update();
}





