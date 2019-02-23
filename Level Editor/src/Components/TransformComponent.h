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
	HTVector2				m_f_Scale	 = {1.0f,1.0f};


	void		SetRotation(float f);
	float		GetRotation() const;
	void		SetPosition(float, float);
	HTVector2	GetPosition() const;

};

#endif