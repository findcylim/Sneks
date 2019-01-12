#include "InputSystem.h"
#include "../Utility/Logger.h"
#include "../Utility/GameStateManager.h"
#include <Windows.h>

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

InputSystem::InputSystem(EventManager* eventmanager, short ID, const char * name)
{
	m_EventManager = eventmanager;
	this->SetID(ID);
	this->SetName(name);
	m_EventManager->AddCallback(Ev_PLAYER1GAME_LEFTKEY, (FunctionP)&Printspeed, (void*)this);
}

InputSystem::~InputSystem()
{

}

void InputSystem::Printspeed(void * data,void * callee)
{
	int SpeedR = ((InputSystem*)callee)->speed;
	SpeedR= 20;
	Logger::LogMessage(LOGGER_SYSTEM, "CALL %d",SpeedR);
}

void InputSystem::Update()
{
	State currentState = GameStateManager::ReturnCurrentState();

	//Player 1 Controls
	//'A' Key (Turn Left)
	if (GetAsyncKeyState(65) < 0 || GetAsyncKeyState(97) < 0)
	{
		
		switch (currentState)
		{
			case state_Game:
				Logger::LogMessage(LOGGER_SYSTEM, "P1 TURN LEFT");
				m_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTKEY);
				break;
		}
		
	}
	//'D' Key (Turn Right)
	if (GetAsyncKeyState(68) < 0 || GetAsyncKeyState(100) < 0)
	{
		switch (currentState)
		{
			case state_Game:
				Logger::LogMessage(LOGGER_SYSTEM, "P1 TURN RIGHT");
				m_EventManager->EmitEvent(Ev_PLAYER1GAME_RIGHTKEY);
				break;
		}
	}
	//'Left Shift' Key (Activate Power-up)
	if (GetAsyncKeyState(VK_LSHIFT) < 0)
	{
		switch (currentState)
		{
			case state_Game:
				Logger::LogMessage(LOGGER_SYSTEM, "P1 POWERUP");
				m_EventManager->EmitEvent(Ev_PLAYER1GAME_LEFTSHIFTKEY);
				break;
		}
	}

	//Player 2 Controls
	//'Left Arrow' Key (Turn Left)
	if (GetAsyncKeyState(VK_LEFT) < 0 )
	{
		switch (currentState)
		{
		case state_Game:
			Logger::LogMessage(LOGGER_SYSTEM, "P2 TURN LEFT");
			m_EventManager->EmitEvent(Ev_PLAYER2GAME_LEFTKEY);
			break;
		}

	}
	//'Right Arrow' Key (Turn Right)
	if (GetAsyncKeyState(VK_RIGHT) < 0)
	{
		switch (currentState)
		{
		case state_Game:
			Logger::LogMessage(LOGGER_SYSTEM, "P2 TURN RIGHT");
			m_EventManager->EmitEvent(Ev_PLAYER2GAME_RIGHTKEY);
			break;
		}
	}
	//'Right Shift' Key (Activate Power-up)
	if (GetAsyncKeyState(VK_RSHIFT) < 0)
	{
		switch (currentState)
		{
		case state_Game:
			Logger::LogMessage(LOGGER_SYSTEM, "P2 POWERUP");
			m_EventManager->EmitEvent(Ev_PLAYER2GAME_LEFTSHIFTKEY);
			break;
		}
	}
}
