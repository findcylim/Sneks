#pragma once
#include "../ECS/ComponentList.h"
#include "../Components/TransformComponent.h"
class TextRendererComponent :
	public BaseComponent
{
public:
	TransformComponent* m_po_LinkedTransform;
	HTVector2 m_o_PositionOffset;
	char * m_p_Text;
	TextRendererComponent() = delete;
	TextRendererComponent(TransformComponent* t_Comp);
	void CreateText(float xOffset, float yOffset, const char * textIn);
	void SetText(const char * textIn);
	~TextRendererComponent();
};

