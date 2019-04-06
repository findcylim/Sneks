/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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

struct TextRendererComponent : public BaseComponent
{
	TransformComponent* m_po_LinkedTransform;
	HTVector2 m_x_TextPosition;
	HTColor m_x_TextColor;

	char * m_p_Text = nullptr;
	TextRendererComponent() = delete;
	TextRendererComponent(TransformComponent* t_Comp);
	void CreateText(float xOffset, float yOffset, const char * textIn,HTColor textColor);
	void SetText(const char * textIn);
	~TextRendererComponent();
};

