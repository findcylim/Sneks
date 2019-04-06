/* Start Header ***************************************************************/
/*!
\file TintComponent.h
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