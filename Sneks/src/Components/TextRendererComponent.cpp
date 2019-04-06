/* Start Header****************************************************************/
/*!
\file TextRendererComponent.cpp
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

#include "TextRendererComponent.h"
#include <iostream>


TextRendererComponent::~TextRendererComponent()
{
	delete[] m_p_Text;
	m_p_Text = nullptr;
}

TextRendererComponent::TextRendererComponent(TransformComponent* t_Comp)
{
	m_po_LinkedTransform = t_Comp;
	m_p_Text = nullptr;
}

void TextRendererComponent::CreateText(float xTextPosition, float yTextPosition, const char * textIn,HTColor textColor)
{
	SetText(textIn);
	m_x_TextColor = textColor;
	m_x_TextPosition.x = xTextPosition;
	m_x_TextPosition.y = yTextPosition;
}


void TextRendererComponent::SetText(const char * textIn)
{
	int len = static_cast<int>(strlen(textIn)) + 1;
	delete[] m_p_Text;
	m_p_Text = new char[len];
	if (textIn)
		strcpy_s(m_p_Text, len, textIn);
}