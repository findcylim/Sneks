

#ifndef CANVASCOMPONENT_H
#define CANVASCOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"
#include <list>

class EntityManager;
class SystemManager;

enum CanvasElementEnum
{
	kCanvasButton,
	kCanvasBasicSprite,
	kCanvasTextLabel
};

enum CanvasElementButtonState
{
	kButtonIdle,
	kButtonHover,
	kButtonClick
};

struct CanvasComponent : public BaseComponent
{
public:
	CanvasComponent(){}
	~CanvasComponent()
	{
		/*auto element = m_x_CanvasElementList.begin();
		while (element != m_x_CanvasElementList.end())
		{
			this->m_po_OwnerEntity->m_po_EntityManager->AddToDeleteQueue((*element));
			++element;
		}*/
	}
	CanvasComponent(const CanvasComponent&) = delete;
	CanvasComponent& operator=(const CanvasComponent&) = delete;
	ComponentManager* m_po_ComponentManager;
	std::list<BaseEntity*> m_x_CanvasElementList;
};

class CanvasElementComponent : public BaseComponent
{
public:
	char* m_pc_ElementText = nullptr;
	AEGfxTexture * m_x_BasicSprite,* m_x_HoverSprite,* m_x_ClickSprite;
	void(*ButtonFunction)(SystemManager*);
	float m_f_XOffset = 0.0f; // 0 to 1.0f
	float m_f_YOffset = 0.0f; // 0 to 1.0f
	bool m_b_IsClicked = false;
	CanvasElementButtonState m_e_ButtonState = kButtonIdle;
	
	CanvasElementComponent() {}
	~CanvasElementComponent()
	{
		if (m_pc_ElementText)
			delete[] m_pc_ElementText;
	}
};

#endif