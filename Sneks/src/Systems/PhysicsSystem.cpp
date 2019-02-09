#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GameStateManager* gameStateManager):
BaseSystem(entityManagerPtr)
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


void PhysicsSystem::Update(float dt)
{
	State currentState = m_o_GameStateManager->ReturnCurrentState();

	auto i_PhysicsComponent = static_cast<PhysicsComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentPhysics));

	while (i_PhysicsComponent)
	{
		ApplyAcceleration(i_PhysicsComponent, dt);
		ApplyVelocity(i_PhysicsComponent, dt);
		i_PhysicsComponent = static_cast<PhysicsComponent*>(i_PhysicsComponent->m_po_NextComponent);
	}
	/*
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
	}*/
}




HTVector2 PhysicsSystem::ApplyVelocity(PhysicsComponent* physicsComponent, float dt) const
{
	auto forwardVelocity = CalculateVelocity(physicsComponent);
	physicsComponent->m_po_TransformComponent->m_x_Position.x += forwardVelocity.x * dt;
	physicsComponent->m_po_TransformComponent->m_x_Position.y += forwardVelocity.y * dt;
	return forwardVelocity;
}

HTVector2 PhysicsSystem::CalculateVelocity(PhysicsComponent* physicsComponent) const
{
	//apply the velocity
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, physicsComponent->m_po_TransformComponent->GetRotation());
	HTVector2 forwardVelocity ={ forwardVector.x * physicsComponent->m_f_Speed, forwardVector.y * physicsComponent->m_f_Speed };
	return forwardVelocity;
}

void PhysicsSystem::ClampVelocity(PhysicsComponent* physicsComponent, const SnekHeadComponent snekHeadComponent) const
{
	if (physicsComponent->m_f_Speed >= -snekHeadComponent.m_f_MinSpeed &&
			physicsComponent->m_f_Speed <= snekHeadComponent.m_f_MinSpeed)
		physicsComponent->m_f_Speed = -snekHeadComponent.m_f_IdleSpeed;
	else if (physicsComponent->m_f_Speed < 0)
		physicsComponent->m_f_Speed += snekHeadComponent.m_f_Friction;
	else if (physicsComponent->m_f_Speed > 0)
		physicsComponent->m_f_Speed -= snekHeadComponent.m_f_Friction;
}

void PhysicsSystem::ApplyAcceleration(PhysicsComponent* physicsComponent, float dt) const
{
	//Clamp percentage higher when speed is higher, so less acceleration when speed high
	float accelClamp = 1.0f - physicsComponent->m_f_Speed / physicsComponent->m_f_MaxSpeed;
	if (accelClamp < 0)
		accelClamp = 0;
	else if (accelClamp > 1.0f)
		accelClamp = 1.0f;
	physicsComponent->m_x_Acceleration *= accelClamp;
	physicsComponent->m_f_Speed += physicsComponent->m_x_Acceleration * dt;
}