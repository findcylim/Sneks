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