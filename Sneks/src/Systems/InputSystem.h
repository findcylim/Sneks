#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#pragma once
#include <map>
#include "../ECS/System.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"

enum ButtonNames
{
	kInputPlayer1TurnLeft,
	kInputPlayer1TurnRight,
	kInputPlayer1Powerup,
	kInputPlayer2TurnLeft,
	kInputPlayer2TurnRight,
	kInputPlayer2Powerup,
	kEscape,
};

class InputSystem : public System
{
public:
	InputSystem(EventManager* eventManager,short ID,const char * name,GameStateManager* gameStateManager,Logger* logger);
	~InputSystem();
	void Update();

	static void Printspeed(void * data, void * callee); //TEst
private:
	int speed = 10;//Test
	std::map<unsigned char, ButtonNames> m_m_KeyBinds;
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
	Logger*				m_o_Logger;
	void SetKeyBinds();
};

#endif