#include "ECS/EventManager.h"
#include "ECS/ECSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE,
	LPSTR, int nCmdShow)
{
	//NOTE:: VS check for mem leaks use this before submission
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	AlphaEngineHelper::AlphaInitialize(hInstance, nCmdShow);
	ECSystem* engine = new ECSystem;
	engine->InitializeEngine();

	while (engine->IsEngineOn())
	{
		engine->Update();
		if(GetAsyncKeyState(AEVK_9))
			assert(!LogMemoryLeaks() && "Memory leaks detected! Check log file for more info.");
	}
	delete(engine);
	AESysExit();
	assert(!LogMemoryLeaks() && "Memory leaks detected! Check log file for more info.");
	return 0;
}