#pragma once

#include <vector>
#include "EventManager.h"
#include "../Utility/Logger.h"


using namespace std;

//Virtual System Class
//To be overriden by actual systems
//Needs to be used to manage different systems in the SystemManager
class System
{
public:
	short SystemID;
	const char* SystemName;
	virtual void Update();
};




class SystemManager
{
private:
	static std::vector<System*> SystemList;
	static SystemManager * instance;
	SystemManager();
	~SystemManager();
public:
	static SystemManager * Instance();
	void AddSystem(System* NewSystem);
	void RemoveSystem(System* RemSystem);
	static void Update();
	void Initialize();
};

