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

void TransformComponent::SetPositionX(const float positionX)
{
	m_x_Position.x = positionX;
}

void TransformComponent::SetPositionY(const float positionY)
{
	m_x_Position.y = positionY;
}

void TransformComponent::SetScale(const float newScale)
{
	m_f_ScaleMultiplier = newScale;
}

HTVector2 TransformComponent::GetPosition() const
{
	return m_x_Position;
}

HTVector2 TransformComponent::GetScale() const
{
	return m_f_ScaleMultiplier * m_f_Scale;
}