
#ifndef  TRANSFORMCOMPONENT_H
#define  TRANSFORMCOMPONENT_H


#pragma once



#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"

class TransformComponent : public BaseComponent
{
public:

	HTVector2					m_x_Position;
	float		                m_f_Rotation = 0;
	float						m_f_Scale = 1.0f;


	float		GetScale() const;
	void		SetScale(float f);
	void		SetRotation(float f);
	float		GetRotation()		const;
	void		SetPosition(float, float);
	HTVector2	GetPosition() const;
	HTVector2	GetMin(void) const;
	HTVector2	GetMax(void) const;
};

#endif