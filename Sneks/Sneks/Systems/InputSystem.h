#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#pragma once
#include "../ECS/System.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"
class InputSystem : public System
{
public:
	InputSystem(EventManager* eventmanager,short ID,const char * name,GameStateManager* gsm);
	~InputSystem();
	void Update();

	static void Printspeed(void * data, void * callee); //TEst
private:
	int speed = 10;//Test
	EventManager* m_o_EventManager;
	GameStateManager* m_o_GameStateManager;
};

#endif