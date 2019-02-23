#include "PhysicsSystem.h"
#include <iostream>
#include "SnekSystem.h"

PhysicsSystem::PhysicsSystem(EntityManager* entityManagerPtr):
BaseSystem(entityManagerPtr)
{
}

PhysicsSystem::~PhysicsSystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_MOVEMENT_KEY>(this);
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);

}

void PhysicsSystem::Initialize(GameStateManager* gameStateManager)
{
	m_o_GameStateManager	= gameStateManager;
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_MOVEMENT_KEY>(this);
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this);

}
void PhysicsSystem::receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
}

void PhysicsSystem::receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData)
{
	float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());
	auto phyComp = eventData.caller;
	auto snekHeadComponent = m_po_ComponentManager->
		GetSpecificComponentInstance<SnekHeadComponent>(phyComp, kComponentSnekHead);

	if (eventData.key == Events::MOVE_KEY_UP) {
		phyComp->m_f_Acceleration = snekHeadComponent->m_f_AccelerationForce;
	}
	else if (eventData.key == Events::MOVE_KEY_LEFT) {
		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() +
			snekHeadComponent->m_f_TurnSpeed * dt
		);
	}
	else if (eventData.key == Events::MOVE_KEY_RIGHT) {
		phyComp->m_po_TransformComponent->SetRotation(
			phyComp->m_po_TransformComponent->GetRotation() -
			snekHeadComponent->m_f_TurnSpeed * dt
		);
	}
}


void PhysicsSystem::Update(float dt)
{
	State currentState = m_o_GameStateManager->ReturnCurrentState();

	UNREFERENCED_PARAMETER(currentState);

	auto i_PhysicsComponent = m_po_ComponentManager
		->GetFirstComponentInstance<PhysicsComponent>(kComponentPhysics);

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
	if (physicsComponent->m_f_Speed > physicsComponent->m_f_MaxSpeed)
	{
		physicsComponent->m_f_Speed *= 0.95f;
	}
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
	UNREFERENCED_PARAMETER(physicsComponent);
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
