#pragma once
#include "../ECS/ComponentList.h"
#include "../Components/TransformComponent.h"
struct TextRendererComponent : public BaseComponent
{
	TransformComponent* m_po_LinkedTransform;
	HTVector2 m_x_TextPosition;
	
	char * m_p_Text = nullptr;
	TextRendererComponent() = delete;
	TextRendererComponent(TransformComponent* t_Comp);
	void CreateText(float xOffset, float yOffset, const char * textIn);
	void SetText(const char * textIn);
	~TextRendererComponent();
};

