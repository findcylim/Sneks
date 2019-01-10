
#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSEngine.h"
#include <iostream>






int main() {
	//game goes here
	int lol;
	ECSEngine* Engine = new ECSEngine;
	Engine->InitializeEngine();

	std::cout << "What" << std::endl;
	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}


	std::cin >> lol;

	return 0;

}