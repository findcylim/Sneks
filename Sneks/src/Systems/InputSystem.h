#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#pragma once
#include <map>
#include "../ECS/System.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"
#include "../ECS/EventListener.h"

enum ButtonNames
{
	kInputPlayer1TurnLeft,
	kInputPlayer1TurnRight,
	kInputPlayer1PowerUp,
	kInputPlayer2TurnLeft,
	kInputPlayer2TurnRight,
	kInputPlayer2PowerUp,
	kEscape,
};

class InputSystem : public BaseSystem
{
public:
	InputSystem(EntityManager* entityManagerPtr, EventManager* eventManager,short ID,const char * name,GameStateManager* gameStateManager,Logger* logger);
	~InputSystem();
	void Update(float dt) override;

private:
	std::map<unsigned char, ButtonNames> m_m_KeyBinds;
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
	Logger*				m_o_Logger;
	void SetKeyBinds();
};

#endif