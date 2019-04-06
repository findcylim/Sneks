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
}

float TransformComponent::GetRotation() const
{
	return m_f_Rotation;
}

HTVector2 TransformComponent::GetDrawScale() const
{
	return m_f_ScaleMultiplier * m_f_Scale;
}
