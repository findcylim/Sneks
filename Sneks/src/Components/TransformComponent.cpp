/* Start Header****************************************************************/
/*!
\file TransformComponent.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code to manipulate some data in transform component
that does not use public/default getters/setters

\par Contribution : CY - 100.00%

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
