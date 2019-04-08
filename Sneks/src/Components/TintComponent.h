/* Start Header****************************************************************/
/*!
\file TintComponent.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that allows an entity to be tinted red when 
getting hit to show feedback to player.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/
#include "../Math/HTColor.h"

#ifndef TINT_COMPONENT_H
#define TINT_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"


struct TintComponent : public BaseComponent
{
	HTColor m_x_OriginalColor = {1,1,1,1};
	HTColor m_x_TintColor ={ 1.0f, 0,0,1.0f };
	float m_f_CurrentTintTimer = 0;
	float m_f_TintDuration = 0;

};

#endif