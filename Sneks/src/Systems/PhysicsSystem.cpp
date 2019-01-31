#include "PhysicsSystem.h"
#include <iostream>
#include <Windows.h>

PhysicsSystem::PhysicsSystem(EventManager* eventManager, GameStateManager* gameStateManager)
{
	Initialize(eventManager, gameStateManager);
}

PhysicsSystem::~PhysicsSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_RIGHTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_RIGHTKEY>(this);
}

void PhysicsSystem::Initialize(EventManager* eventManager,GameStateManager* gameStateManager)
{
	m_o_EventManagerPtr		= eventManager;
	m_o_GameStateManager	= gameStateManager;
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_RIGHTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_RIGHTKEY>(this);
}

//Currently Debug Messages only
//Change this to your physics velocity
//Probably fill in the Event Data with pointers to snek entities
void PhysicsSystem::receive(const Events::Ev_PLAYER1GAME_LEFTKEY& eventData) 
{
	std::cout << "You pressed A!" << std::endl;
}
void PhysicsSystem::receive(const Events::Ev_PLAYER1GAME_LEFTSHIFTKEY& eventData)
{
	std::cout << "You pressed Left Shift!" << std::endl;
}
void PhysicsSystem::receive(const Events::Ev_PLAYER1GAME_RIGHTKEY& eventData)
{
	std::cout << "You pressed D!" << std::endl;
}
void PhysicsSystem::receive(const Events::Ev_PLAYER2GAME_LEFTKEY& eventData) 
{
	std::cout << "You pressed Left!" << std::endl;
}
void PhysicsSystem::receive(const Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY& eventData)
{
	std::cout << "You pressed Right Shift!" << std::endl;
}
void PhysicsSystem::receive(const Events::Ev_PLAYER2GAME_RIGHTKEY& eventData)
{
	std::cout << "You pressed Right!" << std::endl;

}


void PhysicsSystem::Update()
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
			Events::Ev_PLAYER1GAME_LEFTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER1GAME_LEFTKEY>(temp);
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
			Events::Ev_PLAYER1GAME_RIGHTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER1GAME_RIGHTKEY>(temp);
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
			Events::Ev_PLAYER1GAME_LEFTSHIFTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>(temp);
			break;
		}
	}

	//Player 2 Controls
	//'Left Arrow' Key (Turn Left)
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		switch (currentState)
		{
		case kStateGame:
			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 TURN LEFT");
			Events::Ev_PLAYER2GAME_LEFTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER2GAME_LEFTKEY>(temp);
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
			Events::Ev_PLAYER2GAME_RIGHTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER2GAME_RIGHTKEY>(temp);
			break;
		}
	}
	//'Right Shift' Key (Activate Power-up)
	if (GetAsyncKeyState(VK_RSHIFT) < 0)
	{
		switch (currentState)
		{
		case kStateGame:
			//m_o_Logger->LogMessage(LOGGER_SYSTEM, "P2 POWERUP");
			Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY temp;
			m_o_EventManagerPtr->EmitEvent<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>(temp);
			break;
		}
	}
}


HTVector2 GetMin() const
{
	HTVector2 min ={};
	min.x = m_x_Position.x - m_x_Size.x * m_f_Scale / 2;
	min.y = m_x_Position.y - m_x_Size.y * m_f_Scale / 2;
	return min;
}

HTVector2 GetMax() const
{
	HTVector2 max ={};
	max.x = m_x_Position.x + m_x_Size.x * m_f_Scale / 2;
	max.y = m_x_Position.y + m_x_Size.y * m_f_Scale / 2;
	return max;
}

HTVector2 DrawObject::ApplyVelocity(float dt)
{
	auto forwardVelocity = GetForwardVelocity();
	m_x_Position.x += forwardVelocity.x * dt;
	m_x_Position.y += forwardVelocity.y * dt;
	return forwardVelocity;
}

HTVector2 DrawObject::GetForwardVelocity() const
{
	//apply the velocity
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, m_f_Rotation);
	HTVector2 forwardVelocity ={ forwardVector.x * m_f_Velocity, forwardVector.y * m_f_Velocity };
	return forwardVelocity;
}
