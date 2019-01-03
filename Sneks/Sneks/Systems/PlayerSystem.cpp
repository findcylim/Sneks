
#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(EventManager* Emanager)
{
	playerHP = 123;
	EManager = Emanager;
	void*(*f)(void *) = AttackPlayer;
	Callback* cb = new Callback(&AttackPlayer);
	//Emanager->AddEvent(1,"Attack",)
}

PlayerSystem::~PlayerSystem()
{

}

void* PlayerSystem::AttackPlayer(void* dmg)
{
	playerHP -= *((int*)dmg);
}

void PlayerSystem::Update()
{

}



