#include "ECSEngine.h"
#include "../Systems/PlayerSystem.h"


ECSEngine::ECSEngine()
{
	m_Utility		= new Utility();
	m_EventManager	= new EventManager(m_Utility);
	m_SystemManager = new SystemManager(m_Utility);
	EngineStatus	= false;
}


ECSEngine::~ECSEngine()
{
	//TODO Call logger destroy function here later on
}




/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSEngine::InitializeEngine()
{
	m_EventManager->Initialize();
	/*
		Create and add Events here
	*/



	m_SystemManager->Initialize();
	/*
		Create and add Systems here
	*/

	//PlayerSystem
	PlayerSystem* PlayerS = new PlayerSystem(m_EventManager);
	PlayerS->SetID(0);
	PlayerS->SetName("PlayerSystem");
	m_SystemManager->AddSystem(PlayerS);

	EngineStatus = true;
}

bool ECSEngine::IsEngineOn()
{
	return EngineStatus;
}


void ECSEngine::Update()
{
	m_EventManager->Update();
	m_SystemManager->Update();
}





