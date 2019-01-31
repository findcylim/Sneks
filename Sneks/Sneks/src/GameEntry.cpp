#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSystem.h"
#include <iostream>
#include "Audio/AudioManager.h"





int main() {
	//game goes here
	int lol;
	ECSystem* Engine = new ECSystem;
	Engine->InitializeEngine();
	Audio_Init();

	std::cout << "What" << std::endl;
	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}

	delete(Engine);
	Audio_Cleanup();

	std::cin >> lol;

	return 0;

}