#pragma once

#include "../ECS/SystemManager.h"
#include "../ECS/EventManager.h"





//Test
class PlayerSystem : public System
{
public:
	int playerHP;
	PlayerSystem(EventManager* Emanager);
	~PlayerSystem();
	static void AttackPlayer(void* data, void* callee);
	void Update();
private:
	EventManager* EManager;

};
