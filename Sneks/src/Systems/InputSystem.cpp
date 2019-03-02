#include "InputSystem.h"

/**************************************************

	The benefit of creating an input manager is
	reduce the dependencies of individual files
	which require Windows.h

	Making the System slightly more efficient,
	especially when we have more systems which
	take in more player input. (e.g main menu,
	choose your character screen, game screen,
	pause screen etc...)

***************************************************/
std::vector<KeyState> m_x_CurrentKeyStates ={
	{ HTKEY_W},
	{ HTKEY_A},
	{ HTKEY_S},
	{ HTKEY_D},
	{ HTKEY_LEFT},
	{ HTKEY_RIGHT},
	{ HTKEY_DOWN},
	{ HTKEY_UP},
	{ HTKEY_SPACE},
	{ HTKEY_ENTER},
	{ HTKEY_ESC},
	{ HTKEY_E},
	{ HTKEY_P},
	{ HTKEY_C},
	{ HTKEY_B},
	{ HTKEY_NUM_1},
	{ HTKEY_NUM_2},
	{ HTKEY_NUM_3},
	{ HTKEY_NUM_4},
	{ HTKEY_NUM_5},
	{ AEVK_COMMA}
};

constexpr size_t kKeyCount = sizeof(m_x_CurrentKeyStates) / sizeof(KeyState);

KeyState::KeyState(unsigned keyId, float coolDown = 100):
m_i_KeyId(keyId),
m_b_CoolDownEnabled(coolDown > 0),
m_f_CoolDown(coolDown),
m_f_CurrentCoolDownTimer(0)
{}

InputSystem::InputSystem(EntityManager* entityManagerPtr, EventManager* eventManager, short id, const char * name, GameStateManager* gameStateManager, Logger* logger) :
	BaseSystem(entityManagerPtr)
{
	m_o_Logger				= logger;
	m_o_EventManager		= eventManager;
	m_o_GameStateManager	= gameStateManager;
	this->SetID(id);
	this->SetName(name);

}

InputSystem::~InputSystem() = default;

void InputSystem::SetKeyBinds()
{
	/*
	for (auto num : KeyBindsId)
	{

	}*/
}

void InputSystem::ResolveKeyPress(unsigned char keyId)
{
	switch(keyId)
	{
	case AEVK_W:
	case AEVK_S:
	case AEVK_A:
	case AEVK_D:
		break;
	default:
		break;
	}
}

void InputSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	
	for (unsigned i = 0; i < kKeyCount; i++)
	{
		KeyState *current_key = &m_x_CurrentKeyStates[i];
		//decrement m_i_KeyId cd timers if they are cooling down
		if (current_key->m_b_CoolDownEnabled && current_key->m_f_CurrentCoolDownTimer > 0)
		{
			//subtract by deltatime in ms
			if (current_key->m_f_CurrentCoolDownTimer >= 0.0f)
			{
				current_key->m_f_CurrentCoolDownTimer -= dt * 1000;
			}
			else
			{
				//finished cooling down
				current_key->m_f_CurrentCoolDownTimer = 0;
			}
		}

		// if keydown
		if (GetAsyncKeyState(current_key->m_i_KeyId))
		{
			// and cooldown enabled
			if (current_key->m_b_CoolDownEnabled)
			{
				if (current_key->m_f_CurrentCoolDownTimer > 0)
				{
					// if it's cooling down dont fire off
					continue;
				}
				else
				{
					ResolveKeyPress(current_key->m_i_KeyId);
					//Emit(GEV_KEYPRESS, (void*)&(current_key->m_i_KeyId));
					// fire off but set it to cooling down
					current_key->m_f_CurrentCoolDownTimer = current_key->m_f_CoolDown;
				}
			}




		}
	}

	//State currentState = m_o_GameStateManager->ReturnCurrentState();

	////Player 1 Controls
	////'A' Key (Turn Left)
	//if (GetAsyncKeyState(65) < 0 || GetAsyncKeyState(97) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN LEFT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTKEY);
	//			break;
	//	}
	//}

	////'D' Key (Turn Right)
	//if (GetAsyncKeyState(68) < 0 || GetAsyncKeyState(100) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN RIGHT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_RIGHTKEY);
	//			break;
	//	}
	//}

	////'Left Shift' Key (Activate Power-up)
	//if (GetAsyncKeyState(VK_LSHIFT) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 POWERUP");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTSHIFTKEY);
	//			break;
	//	}
	//}

	////Player 2 Controls
	////'Left Arrow' Key (Turn Left)
	//if (GetAsyncKeyState(VK_LEFT) < 0 )
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN LEFT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_LEFTKEY);
	//			break;
	//	}

	//}

	////'Right Arrow' Key (Turn Right)
	//if (GetAsyncKeyState(VK_RIGHT) < 0)
	//{
	//	switch (currentState)
	//		{
	//		case kStateGame:
	//			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN RIGHT");
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTKEY);
	//			break;
	//	}
	//}

	////'Right Shift' Key (Activate Power-up)
	//if (GetAsyncKeyState(VK_RSHIFT) < 0)
	//{
	//	switch (currentState)
	//	{
	//		case kStateGame:
	//			m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTSHIFTKEY);
	//			break;
	//		}
	//}
}
