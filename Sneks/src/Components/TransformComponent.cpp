#include "TransformComponent.h"

void TransformComponent::SetRotation(const float f)
{
	m_f_Rotation = f;
}

float TransformComponent::GetRotation() const
{
	return m_f_Rotation;
}

void TransformComponent::SetPosition(const float positionX, const float positionY)
{
	m_x_Position.x = positionX;
	m_x_Position.y = positionY;
}

HTVector2 TransformComponent::GetPosition() const
{
	return m_x_Position;
}