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

