#include "InputSystem.h"
#include <windows.h>

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

InputSystem::InputSystem(EventManager* eventManager, short id, const char * name, GameStateManager* gameStateManager,Logger* logger)
{
	m_o_Logger				= logger;
	m_o_EventManager		= eventManager;
	m_o_GameStateManager	= gameStateManager;
	this->SetID(id);
	this->SetName(name);

}

InputSystem::~InputSystem()
{

}

void InputSystem::SetKeyBinds()
{
	for (std::map<unsigned char, ButtonNames>::iterator i_KeyBind = m_m_KeyBinds.begin();
														i_KeyBind != m_m_KeyBinds.end();
														i_KeyBind++)
	{

	}
}


void InputSystem::Update()
{
	State currentState = m_o_GameStateManager->ReturnCurrentState();

	//Player 1 Controls
	//'A' Key (Turn Left)
	if (GetAsyncKeyState(65) < 0 || GetAsyncKeyState(97) < 0)
	{
		switch (currentState)
		{
			case kStateGame:
				//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN LEFT");
				m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTKEY);
				break;
		}
	}

	//'D' Key (Turn Right)
	if (GetAsyncKeyState(68) < 0 || GetAsyncKeyState(100) < 0)
	{
		switch (currentState)
		{
			case kStateGame:
				//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 TURN RIGHT");
				m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_RIGHTKEY);
				break;
		}
	}

	//'Left Shift' Key (Activate Power-up)
	if (GetAsyncKeyState(VK_LSHIFT) < 0)
	{
		switch (currentState)
		{
			case kStateGame:
				//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P1 POWERUP");
				m_o_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTSHIFTKEY);
				break;
		}
	}

	//Player 2 Controls
	//'Left Arrow' Key (Turn Left)
	if (GetAsyncKeyState(VK_LEFT) < 0 )
	{
		switch (currentState)
		{
			case kStateGame:
				//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN LEFT");
				m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_LEFTKEY);
				break;
		}

	}

	//'Right Arrow' Key (Turn Right)
	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		switch (currentState)
			{
			case kStateGame:
				//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN RIGHT");
				m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTKEY);
				break;
		}
	}

	//'Right Shift' Key (Activate Power-up)
	if (GetAsyncKeyState(VK_RSHIFT) < 0)
	{
		switch (currentState)
		{
			case kStateGame:
				m_o_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTSHIFTKEY);
				break;
			}
	}
}
