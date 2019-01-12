#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#pragma once
#include "../ECS/System.h"
#include "../ECS/EventManager.h"
class InputSystem : public System
{
public:
	InputSystem(EventManager* eventmanager,short ID,const char * name);
	~InputSystem();
	void Update();

	static void Printspeed(void * data, void * callee);
private:
	int speed = 10;
	EventManager* m_EventManager;
};

#endif