
#include "PhysicsComponent.h"


float PhysicsComponent::GetVelocity() const
{
	return m_f_Velocity;
}

void PhysicsComponent::SetVelocity(const float velocity)
{
	m_f_Velocity = velocity;
	//m_f_Velocity.y = velocityY;
}