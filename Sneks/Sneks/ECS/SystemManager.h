#pragma once

#include <vector>
#include "EventManager.h"
#include "../Utility/Utility.h"



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
	std::vector<System*> SystemList;
public:
	Utility* m_UtilityPtr;
	void AddSystem(System* NewSystem);
	void RemoveSystem(System* RemSystem);
	void Update();
	void Initialize();
	SystemManager(Utility*);
	~SystemManager();
};

