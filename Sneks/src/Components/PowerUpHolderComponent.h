/* Start Header ***************************************************************/
/*!
\file PowerUpHolderComponent.h
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

#ifndef POWERUPHOLDER_COMPONENT_H
#define POWERUPHOLDER_COMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "PowerUpComponent.h"


class PowerUpHolderComponent : public BaseComponent
{
public:
	PowerUpType m_x_Type;
};

#endif