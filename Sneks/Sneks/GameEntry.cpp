
#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSystem.h"
#include <iostream>






int main() {
	//game goes here
	int lol;
	ECSystem* Engine = new ECSystem;
	Engine->InitializeEngine();

	std::cout << "What" << std::endl;
	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}


	std::cin >> lol;

	return 0;

}