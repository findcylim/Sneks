/* Start Header****************************************************************/
/*!
\file TransformComponent.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that defines where an entity is in world space.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef  TRANSFORMCOMPONENT_H
#define  TRANSFORMCOMPONENT_H
#pragma once

#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"

struct TransformComponent : public BaseComponent
{
	HTVector2				m_x_Position ={ 0,0 };
	HTVector2				m_f_Scale    ={ 1.0f,1.0f }; //Amount scaled by base normalized mesh
	HTVector2				m_f_ScaleMultiplier ={ 1.0f,1.0f };

	void		SetRotation(float f);
	float		GetRotation() const;
	HTVector2   GetDrawScale() const;

private:
	float		            m_f_Rotation = 0;
};

#endif