#include "ECSEngine.h"


ECSEngine::ECSEngine()
{
	logger = new Logger;
	EngineStatus = false;
}


ECSEngine::~ECSEngine()
{
	delete(logger);
}




/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSEngine::InitializeEngine()
{
	EventManager::Instance()->Initialize(logger);
	/*
		Create and add Events here
	*/



	SystemManager::Instance()->Initialize(logger);
	/*
		Create and add Systems here
	*/
	



	EngineStatus = true;
}

bool ECSEngine::IsEngineOn()
{
	return EngineStatus;
}


void ECSEngine::Update()
{
	EventManager::Instance()->Update();
	SystemManager::Instance()->Update();
}





