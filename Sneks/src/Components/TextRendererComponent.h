/* Start Header****************************************************************/
/*!
\file TextRendererComponent.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for components with text.

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/ComponentList.h"
#include "../Components/TransformComponent.h"
#include "../Math/HTColor.h"

/*
	TextRenderer Component data structure
*/
struct TextRendererComponent : public BaseComponent
{
	TransformComponent* m_po_LinkedTransform;
	HTVector2 m_x_TextPosition;
	HTColor m_x_TextColor;

	TextRendererComponent() = delete;
	TextRendererComponent(TransformComponent* t_Comp);
	~TextRendererComponent();

	void CreateText(float xOffset, float yOffset, const char * textIn,HTColor textColor);
	void SetText(const char * textIn);

	char * m_p_Text = nullptr;
};

