/* Start Header ***************************************************************/
/*!
\file TransformComponent.h
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

#ifndef  TRANSFORMCOMPONENT_H
#define  TRANSFORMCOMPONENT_H
#pragma once

#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"

class TransformComponent : public BaseComponent
{
private:
	float		            m_f_Rotation = 0;

public:
	HTVector2				m_x_Position = {0,0};
	HTVector2				m_f_Scale    = { 1.0f,1.0f }; //Amount scaled by base normalized mesh
	HTVector2				m_f_ScaleMultiplier = {1.0f,1.0f};

	void		SetRotation(float f);
	float		GetRotation() const;
	void		SetPositionX(float);
	void		SetPositionY(float);
	void        SetScale(float);
	void		SetScaleX(float);
	HTVector2	GetPosition() const;
	HTVector2   GetScale() const;
};

#endif