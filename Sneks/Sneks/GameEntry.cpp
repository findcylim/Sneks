
#include "ECS/EventManager.h"
#include "ECS/Logger.h"
#include <iostream>


EventManager* eventworker;
Logger* EventLogger;


int main() {
	//game goes here



	eventworker = new EventManager;
	EventLogger = new Logger;
	//std::cout << "  a"<< eventworker->GetTest()<<  std::endl;
	EventLogger->LogMessage("Test Message Log");

	delete eventworker;

	return 0;

}