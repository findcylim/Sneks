

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
		m_x_CanvasElementList.clear();
	}
	CanvasComponent(const CanvasComponent&) = delete;
	CanvasComponent& operator=(const CanvasComponent&) = delete;

	std::list<BaseEntity*> m_x_CanvasElementList;
};

class CanvasElementComponent : public BaseComponent
{
	char* m_pc_ElementText = nullptr;
public:
	CanvasElementComponent() {}
	~CanvasElementComponent()
	{
		if (m_pc_ElementText)
			delete m_pc_ElementText;
	}
};

#endif