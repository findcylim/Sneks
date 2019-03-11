#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#pragma once

#include "../ECS/System.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"
#include "../ECS/EventListener.h"
#include "../Components/CameraComponent.h"

enum KeyBindsId
{
	HTKEY_W = 0x57,
	HTKEY_A = 0x41,
	HTKEY_S = 0x53,
	HTKEY_D = 0x44,
	HTKEY_LEFT = 0x25,
	HTKEY_RIGHT = 0x27,
	HTKEY_DOWN = 0x28,
	HTKEY_UP = 0x26,
	HTKEY_SPACE = 0x20,
	HTKEY_ENTER = 0x0D,
	HTKEY_ESC = 0x1B,
	HTKEY_E = 0x45,
	HTKEY_P = 0x50,
	HTKEY_C = 0x43,
	HTKEY_B = 0x42,
	HTKEY_NUM_1 = 0x31,
	HTKEY_NUM_2 = 0x32,
	HTKEY_NUM_3 = 0x33,
	HTKEY_NUM_4 = 0x34,
	HTKEY_NUM_5 = 0x35
};

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

struct KeyState
{
	const unsigned m_i_KeyId;                 //the m_i_KeyId
	const bool     m_b_CoolDownEnabled;       //If CoolDown Enabled
	const float    m_f_CoolDown;              //Value in ms
	float          m_f_CurrentCoolDownTimer;  //The timer to keep track of whether it's on CD
	//eventID?
	KeyState(unsigned keyId, float coolDown);
};

class InputSystem : public BaseSystem
{
public:
	InputSystem(EntityManager* entityManagerPtr, EventManager* eventManager, short ID, const char * name, GameStateManager* gameStateManager, Logger* logger);
	~InputSystem();
	void Update(float dt) override;

private:
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
	Logger*				m_o_Logger;
	std::vector<KeyState> m_x_CurrentKeyStates;
	HTVector2 m_o_ScreenSize;
	void SetKeyBinds();
	void ResolveKeyPress(unsigned char keyId);
};

#endif