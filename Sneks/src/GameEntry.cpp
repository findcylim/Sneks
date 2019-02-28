#include "Utility/MemoryAllocator.h"
#define new new(__FILE__, __LINE__)

#include <cstdlib>

#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSystem.h"
#include <iostream>

#include "Utility/AlphaEngineHelper.h"


constexpr int kNumBodyParts = 20;


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

	return 0;
}