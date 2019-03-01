#include "Utility/MemoryAllocator.h"
#include <cstdlib>
#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSystem.h"
#include <iostream>
#include "Utility/AlphaEngineHelper.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	AlphaEngineHelper::AlphaInitialize(hInstance, nCmdShow);
	ECSystem* engine = new ECSystem;
	engine->InitializeEngine();

	while (engine->IsEngineOn())
	{
		engine->Update();
	}
	delete(engine);
	AESysExit();
	LogMemoryLeaks();
	return 0;
}