
#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSEngine.h"
#include <iostream>




using namespace std;


int main() {
	//game goes here
	int lol;
	ECSEngine* Engine = new ECSEngine;
	Engine->InitializeEngine();

	cout << "What" << endl;
	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}


	cin >> lol;

	return 0;

}