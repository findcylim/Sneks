

#ifndef CANVASCOMPONENT_H
#define CANVASCOMPONENT_H
#pragma once

#include "../ECS/ComponentList.h"
#include "../Math/HTVector2.h"
#include <list>

enum CanvasElementEnum
{
	kCanvasButton,
	kCanvasBasicSprite,
	kCanvasTextLabel
};

class CanvasComponent : public BaseComponent
{
public:
	CanvasComponent(){}
	~CanvasComponent()
	{
		
	}
	CanvasComponent(const CanvasComponent&) = delete;
	CanvasComponent& operator=(const CanvasComponent&) = delete;
	std::list<BaseEntity*> m_x_CanvasElementList;
};

class CanvasElementComponent : public BaseComponent
{
public:
	char* m_pc_ElementText = nullptr;
	AEGfxTexture * m_x_BasicSprite,* m_x_HoverSprite,* m_x_ClickSprite;
	float m_f_XOffset = 0.0f; // 0 to 1.0f
	float m_f_YOffset = 0.0f; // 0 to 1.0f
	void(*ButtonFunction)(void);
	CanvasElementComponent() {}
	~CanvasElementComponent()
	{
		if (m_pc_ElementText)
			delete[] m_pc_ElementText;
	}
};

#endif