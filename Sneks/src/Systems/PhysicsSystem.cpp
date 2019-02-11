#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{
}

PhysicsSystem::~PhysicsSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER1GAME_RIGHTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER2GAME_RIGHTKEY>(this);
	
	m_o_EventManagerPtr->RemoveListener<Events::Ev_PLAYER_MOVEMENTKEY>(this);
}

void PhysicsSystem::Initialize(GameStateManager* gameStateManager)
{
	m_o_GameStateManager	= gameStateManager;
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_LEFTSHIFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER1GAME_RIGHTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_LEFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_RIGHTSHIFTKEY>(this);
	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER2GAME_RIGHTKEY>(this);

	m_o_EventManagerPtr->AddListener<Events::Ev_PLAYER_MOVEMENTKEY>(this);
}

void PhysicsSystem::receive(const Events::Ev_PLAYER_MOVEMENTKEY& eventData)
{
	//switch (eventData.key)
	//{
	//case 0:

	//}
	auto dt = AEFrameRateControllerGetFrameTime();
	auto phyComp = eventData.caller;
	auto snekHeadComponent = static_cast<SnekHeadComponent*>(m_po_ComponentManager->
		GetSpecificComponentInstance(phyComp, kComponentSnekHead));

	if (eventData.key == Events::MOVEKEY_UP) {
		phyComp->m_f_Acceleration = snekHeadComponent->m_f_AccelerationForce;
	}
	else if (eventData.key == Events::MOVEKEY_LEFT) {
		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() +
			snekHeadComponent->m_f_TurnSpeed * dt
		);
	}
	else if (eventData.key == Events::MOVEKEY_RIGHT) {
		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() -
			snekHeadComponent->m_f_TurnSpeed * dt
		);
	}
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
		//Calculate the stuff
		ApplyAcceleration(i_PhysicsComponent, dt);
		CalculateVelocity(i_PhysicsComponent);

		// Snek checks
		for (auto component : i_PhysicsComponent->m_po_OwnerEntity->m_v_AttachedComponentsList) {
			//only if its a head
			if (component->m_x_ComponentID == kComponentSnekHead) {
				CheckOutOfBounds(i_PhysicsComponent->m_po_TransformComponent,i_PhysicsComponent);
				ClampVelocity(i_PhysicsComponent, static_cast<SnekHeadComponent*>(component));
			}
		}
		//Move the object
		if (!GetAsyncKeyState(AEVK_0)) {
			ApplyVelocity(i_PhysicsComponent, dt);
		}
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
	physicsComponent->m_po_TransformComponent->m_x_Position.x += physicsComponent->m_x_Velocity.x * dt;
	physicsComponent->m_po_TransformComponent->m_x_Position.y += physicsComponent->m_x_Velocity.y * dt;
	return physicsComponent->m_x_Velocity;
}

HTVector2 PhysicsSystem::CalculateVelocity(PhysicsComponent* physicsComponent) const
{
	//apply the velocity
	AEVec2 forwardVector;
	AEVec2FromAngle(&forwardVector, physicsComponent->m_po_TransformComponent->GetRotation());
	HTVector2 forwardVelocity ={ forwardVector.x * physicsComponent->m_f_Speed, forwardVector.y * physicsComponent->m_f_Speed };
	physicsComponent->m_x_Velocity = forwardVelocity;
	return forwardVelocity;
}

void PhysicsSystem::ClampVelocity(PhysicsComponent* physicsComponent, SnekHeadComponent* snekHeadComponent) const
{
	//std::cout << "Accel: " << physicsComponent->m_f_Acceleration << ", " << physicsComponent->m_f_Speed << std::endl;
	if (physicsComponent->m_f_Acceleration == 0) {
		if (physicsComponent->m_f_Speed < snekHeadComponent->m_f_IdleSpeed)
			physicsComponent->m_f_Speed = snekHeadComponent->m_f_IdleSpeed;		
		else if (physicsComponent->m_f_Speed < 0)
			physicsComponent->m_f_Speed += snekHeadComponent->m_f_Friction;
		else if (physicsComponent->m_f_Speed > 0)
			physicsComponent->m_f_Speed -= snekHeadComponent->m_f_Friction;
	}
}

void PhysicsSystem::ApplyAcceleration(PhysicsComponent* physicsComponent, float dt) const
{
	//Clamp percentage higher when speed is higher, so less acceleration when speed high
	float accelClamp = 1.0f - fabsf(physicsComponent->m_f_Speed / physicsComponent->m_f_MaxSpeed);
	
	if (accelClamp < 0)
		accelClamp = 0;
	else if (accelClamp > 1.0f)
		accelClamp = 1.0f;

	auto clampedAccel = physicsComponent->m_f_Acceleration * accelClamp;
	physicsComponent->m_f_Speed += clampedAccel * dt;
}

void PhysicsSystem::CheckOutOfBounds(TransformComponent* transformComponent, PhysicsComponent* physicsComponent) const
{
	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.x > AEGfxGetWinMaxX() + 2 * 1920)
	{
		//physicsComponent->m_x_Velocity.x = 0;
		transformComponent->m_x_Position.x = AEGfxGetWinMaxX() + 2 * 1920;
	}
	else if (transformComponent->m_x_Position.x < AEGfxGetWinMinX() - 2 * 1920) {
		//physicsComponent->m_x_Velocity.x = 0;
		transformComponent->m_x_Position.x = AEGfxGetWinMinX() - 2 * 1920;
	}
	//if out of screen, clamp movement
	if (transformComponent->m_x_Position.y > AEGfxGetWinMaxY() + 2 * 1080) {
		//physicsComponent->m_x_Velocity.y = 0;
		transformComponent->m_x_Position.y = AEGfxGetWinMaxY() + 2 * 1080;
	}
	else if (transformComponent->m_x_Position.y < AEGfxGetWinMinY() - 2 * 1080) {
		//physicsComponent->m_x_Velocity.y = 0;
		transformComponent->m_x_Position.y = AEGfxGetWinMinY() - 2 * 1080;
	}
}
