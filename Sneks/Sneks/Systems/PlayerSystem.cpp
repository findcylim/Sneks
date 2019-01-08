
#include "PlayerSystem.h"
#include <iostream>


PlayerSystem::PlayerSystem(EventManager* Emanager)
{
	playerHP = 123;
	EManager = Emanager;
	Emanager->AddCallback(0, (FunctionP)&AttackPlayer, (void*)this);
}

PlayerSystem::~PlayerSystem()
{

}

void PlayerSystem::AttackPlayer(void* dmg, void* callee)
{
	int lol = *((int*)dmg);
	((PlayerSystem*)callee)->playerHP -= lol;
	cout << lol;
}

void PlayerSystem::Update()
{
	
}



