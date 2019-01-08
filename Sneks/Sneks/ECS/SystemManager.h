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
protected:
	short SystemID;
	const char* SystemName;
	bool isActive;
public:
	virtual void Update();
	short GetID();
	const char * GetName();
	void SetID(short id);
	void SetName(const char* name);
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

