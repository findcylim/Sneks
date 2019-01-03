
#include "ECS/EventManager.h"
#include "ECS/Logger.h"
#include "ECS/ECSEngine.h"
#include <iostream>


Logger* EventLogger;


using namespace std;


int main() {
	//game goes here
	int lol;
	ECSEngine* Engine = new ECSEngine;
	Engine->InitializeEngine();

	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}


	cout << "What" << endl;
	cin >> lol;

	return 0;

}