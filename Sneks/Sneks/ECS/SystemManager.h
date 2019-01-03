#pragma once

#include <vector>
#include "Logger.h"


using namespace std;

//Virtual System Class
//To be overriden by actual systems
//Needs to be used to manage different systems in the SystemManager
class System
{
	short SystemID;
public:
	virtual void Update();
};




class SystemManager
{
private:
	std::vector<System*> SystemList;
	Logger* LogObj;
	static SystemManager * instance;
	SystemManager();
	~SystemManager();
public:
	static SystemManager * Instance();
	void AddSystem(System* NewSystem);
	void RemoveSystem(System* RemSystem);
	void Update();
	void Initialize(Logger*);
};

