#pragma once
#include "../ECS/ComponentList.h"
#include "../Components/TransformComponent.h"
class TextRendererComponent :
	public BaseComponent
{
public:
	TransformComponent* m_po_LinkedTransform;
	char * m_p_Text;
	TextRendererComponent() = delete;
	TextRendererComponent(TransformComponent* t_Comp,const char* textIn = nullptr);
	void SetText(const char * textIn);
	~TextRendererComponent();
};

