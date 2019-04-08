/* Start Header****************************************************************/
/*!
\file PowerUpHolderComponent.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the holder component in charge of the powerup holder
in terms of which powerup and its life.

\par Contribution : Edwin  - 100.00%

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
	//type of powerup
	PowerUpType m_x_Type;
	//remaining life of the powerup holder
	float m_f_RemainingLife;
};

#endif