
#include "TransformComponent.h"


float TransformComponent::GetScale() const
{
	return m_f_Scale;
}

void TransformComponent::SetScale(float f)
{
	m_f_Scale = f;
}

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

//Move this to Physics Systems
HTVector2 TransformComponent::GetMin() const
{
	HTVector2 min = {};
	min.x = m_x_Position.x - m_x_Size.x * m_f_Scale / 2;
	min.y = m_x_Position.y - m_x_Size.y * m_f_Scale / 2;
	return min;
}

HTVector2 TransformComponent::GetMax() const
{
	HTVector2 max = {};
	max.x = m_x_Position.x + m_x_Size.x * m_f_Scale / 2;
	max.y = m_x_Position.y + m_x_Size.y * m_f_Scale / 2;
	return max;
}

