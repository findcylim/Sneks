#include "TransformComponent.h"
#include <AEExport.h>

void TransformComponent::SetRotation(const float f)
{
	m_f_Rotation = f;
	while (m_f_Rotation < 0)
		m_f_Rotation+=2 * PI;
	while (m_f_Rotation > 2 * PI)
		m_f_Rotation-= 2 * PI;
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