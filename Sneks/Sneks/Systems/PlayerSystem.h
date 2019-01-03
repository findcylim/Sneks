#pragma once

#include "../ECS/SystemManager.h"
#include "../ECS/EventManager.h"





//Test
class PlayerSystem : System
{
	int playerHP;
	EventManager* EManager;
	void* AttackPlayer(void* dmg);
	void Update();

	PlayerSystem(EventManager* Emanager);
	~PlayerSystem();
};
