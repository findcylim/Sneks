#pragma once

#include <vector>
#include "Logger.h"


using namespace std;
class System
{

public:
	virtual void Update();
};

//Test
class PlayerSystem : System
{

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
	void Update();
};

