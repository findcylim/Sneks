#include "ECSEngine.h"
#include "../Systems/PlayerSystem.h"


ECSEngine::ECSEngine()
{
	Logger::Instance();
	EngineStatus = false;
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
	EventManager::Instance()->Initialize();
	/*
		Create and add Events here
	*/



	SystemManager::Instance()->Initialize();
	/*
		Create and add Systems here
	*/

	//PlayerSystem
	PlayerSystem* PlayerS = new PlayerSystem(EventManager::Instance());
	PlayerS->SystemID = 0;
	PlayerS->SystemName = "PlayerSystem";
	SystemManager::Instance()->AddSystem(PlayerS);

	EngineStatus = true;
}

bool ECSEngine::IsEngineOn()
{
	return EngineStatus;
}


void ECSEngine::Update()
{
	EventManager::Update();
	Logger::LogMessage("%d %f %s", 23, 43.54f, "sdsfsfas");
	//SystemManager::Update();
}





