/* Start Header ***************************************************************/
/*!
\file TransformComponent.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "TransformComponent.h"
#include <AEExport.h>

void TransformComponent::SetRotation(const float f)
{
	m_f_Rotation = f;
	/*while (m_f_Rotation < 0)
		m_f_Rotation+=2 * PI;
	while (m_f_Rotation > 2 * PI)
		m_f_Rotation-= 2 * PI;*/
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

void TransformComponent::SetScaleX(const float scaleX)
{
	m_f_Scale.x = scaleX;
}

HTVector2 TransformComponent::GetPosition() const
{
	return m_x_Position;
}

HTVector2 TransformComponent::GetScale() const
{
	return m_f_ScaleMultiplier * m_f_Scale;
}