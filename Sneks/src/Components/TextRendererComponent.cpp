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
	m_p_Text = new char[1];
}

void TextRendererComponent::CreateText(float xOffset, float yOffset, const char * textIn)
{
	SetText(textIn);
	m_o_PositionOffset = HTVector2{ xOffset,yOffset };
}

void TextRendererComponent::SetText(const char * textIn)
{
	int len = static_cast<int>(strlen(textIn)) + 1;
	delete[] m_p_Text;
	m_p_Text = new char[len];
	if (textIn)
		strcpy_s(m_p_Text, len, textIn);
}