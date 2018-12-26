
#include "ECS/EventHandler.h"
#include "ECS/Logger.h"
#include <iostream>


EventHandler* eventworker;
Logger* EventLogger;


int main() {
	//game goes here



	eventworker = new EventHandler;
	EventLogger = new Logger;
	//std::cout << "  a"<< eventworker->GetTest()<<  std::endl;
	EventLogger->LogMessage("Test Message Log");

	delete eventworker;

	return 0;

}