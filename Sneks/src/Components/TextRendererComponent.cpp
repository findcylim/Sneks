#include "TextRendererComponent.h"
#include <iostream>


TextRendererComponent::~TextRendererComponent()
{
	delete[] m_p_Text;
}

TextRendererComponent::TextRendererComponent(TransformComponent* t_Comp, const char* textIn)
{
	m_po_LinkedTransform = t_Comp;
	int len = strlen(textIn) + 1;
	m_p_Text = new char[len];
	if(textIn)
		strcpy_s(m_p_Text, len, textIn);
}
void TextRendererComponent::SetText(const char * textIn)
{
	int len = strlen(textIn) + 1;
	delete[] m_p_Text;
	m_p_Text = new char[len];
	if (textIn)
		strcpy_s(m_p_Text, len, textIn);
}