/* Start Header****************************************************************/
/*!
\file CanvasUISystem.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains implementation for UI canvas components

\par Contribution : Adam   - 100.00%  

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "CanvasUISystem.h"
#include "../Components/TextRendererComponent.h"
#include "../Utility/FileIO.h"
#include "../Utility/AlphaEngineHelper.h"

/*
	Constructor
*/
CanvasUISystem::CanvasUISystem(GraphicsSystem* graphicsManager)
{
	m_po_GraphicsManager = graphicsManager;
}

/*
	Destructor	
*/
CanvasUISystem::~CanvasUISystem()
{
	//Removing the event listeners
	m_po_EventManagerPtr->RemoveListener<Events::EV_NEW_UI_ELEMENT>(this);
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

/*
	Update Function	
*/
void CanvasUISystem::Update(float dt) 
{
	(void)dt;
	
	// Getting the camera and first canvas component
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	CanvasComponent * can_Comp = m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas);

	while (can_Comp)
	{
		// Check if the canvas is active
		if(can_Comp->m_b_IsActive)
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			//If the inner element is inactive
			if (!element->m_b_IsActive)
			{
				//Enable the entity
				m_po_EntityManager->EnableSpecificEntityType(*element->m_v_AttachedComponentsList.begin());
			}

			//Fetching a bunch of components
			TransformComponent * t_Comp = m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(element, kComponentTransform);
			CanvasElementComponent * canvasElementComponent = element->GetComponent<CanvasElementComponent>();
			CollisionComponent * collisionComponent = element->GetComponent<CollisionComponent>();
			DrawComponent * drawComponent = element->GetComponent<DrawComponent>();

			// Update the position according to the camera position and scale
			// It is a UI element so it should not move in the world space
			float scale = 1.0f / c_Comp->GetScale();
			t_Comp->m_f_ScaleMultiplier=(scale);
			if (strcmp(can_Comp->m_po_OwnerEntity->m_pc_EntityName, "Tutorial UI"))
			{
				t_Comp->m_x_Position.y = (-c_Comp->GetCameraPos().y - (canvasElementComponent->m_f_YOffset  * scale) + m_o_ScreenSize.y  * scale);
				t_Comp->m_x_Position.x = (-c_Comp->GetCameraPos().x + (canvasElementComponent->m_f_XOffset  * scale) - m_o_ScreenSize.x  * scale);
			}
			// Change the textures accordingly to its click and hover status
			if (collisionComponent)
			{
				if (canvasElementComponent->m_b_IsClicked)
				{
					if (canvasElementComponent->m_x_ClickSprite)
					drawComponent->m_px_Texture = canvasElementComponent->m_x_ClickSprite;
				}
				else if (collisionComponent->m_b_Colliding)
				{
					if (canvasElementComponent->m_x_HoverSprite)
					{
						drawComponent->m_px_Texture = canvasElementComponent->m_x_HoverSprite;
					}
					collisionComponent->m_b_Colliding = false;
				}
				else
				{
					drawComponent->m_px_Texture = canvasElementComponent->m_x_BasicSprite;
				}
			}
		}
		else
		{
			// If it is not active
			for (auto& element : can_Comp->m_x_CanvasElementList)
			{
				// Make sure everything is inactive
				if (element->m_b_IsActive)
				{
					m_po_EntityManager->DisableSpecificEntityType(*element->m_v_AttachedComponentsList.begin());
				}
			}
		}
		can_Comp = static_cast<CanvasComponent*>(can_Comp->m_po_NextComponent);
	}
}

/*
	Initialize function	
*/
void CanvasUISystem::Initialize()
{
	//	 Adds event listeners
	m_po_EventManagerPtr->AddListener<Events::EV_NEW_UI_ELEMENT>(this, this);
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);

	//	Sets the screen size
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	m_o_ScreenSize = { screenX *0.5f, screenY*0.5f };
}

/*
	Clears the all canvas elements in a canvas component
*/
void CanvasUISystem::ClearUI(CanvasComponent* canvas)
{
	for (auto& element : canvas->m_x_CanvasElementList)
	{
		m_po_EntityManager->AddToDeleteQueue(element);
	}
	canvas->m_x_CanvasElementList.clear();
}

/*
	Handles all the creation of UI elements
*/
void CanvasUISystem::AddElement(CanvasComponent* canvasComponent, HTVector2 initialOffset,CanvasElementEnum num, const char * name, 
								const char * uiElementSprite, const char* uiText,const char * uiHoverSprite, const char * uiClickSprite,
								void(*ButtonFunction)(SystemManager*),HTColor textColor)
{
	CanvasElementComponent* ui_Component = nullptr;
	TransformComponent *    t_Component = nullptr;
	DrawComponent *         d_Component = nullptr;
	TextRendererComponent* text_Component = nullptr;

	float ScreenSizeX, ScreenSizeY;
	AlphaEngineHelper::GetScreenSize(&ScreenSizeX, &ScreenSizeY);

	//Gets the components according to the type of UI element
	switch (num)
	{
		case kCanvasTextLabel:
		{
			CanvasTextLabelEntity* newElement1 = m_po_EntityManager->NewEntity<CanvasTextLabelEntity>(kEntityCanvasTextLabel, name);
			t_Component = newElement1->GetComponent<TransformComponent>();
			ui_Component = newElement1->GetComponent<CanvasElementComponent>();
			d_Component = newElement1->GetComponent<DrawComponent>();
			text_Component = newElement1->GetComponent<TextRendererComponent>();
			break;
		}
		case kCanvasBasicSprite:
		{
			CanvasBasicSpriteEntity* newElement2 = m_po_EntityManager->NewEntity<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, name);
			t_Component = newElement2->GetComponent<TransformComponent>();
			ui_Component = newElement2->GetComponent<CanvasElementComponent>();
			d_Component = newElement2->GetComponent<DrawComponent>();
			text_Component = newElement2->GetComponent<TextRendererComponent>();
			break;
		}
		case kCanvasButton:
		{
			CanvasButtonEntity* newElement3 = m_po_EntityManager->NewEntity<CanvasButtonEntity>(kEntityCanvasButton, name);
			t_Component = newElement3->GetComponent<TransformComponent>();
			ui_Component = newElement3->GetComponent<CanvasElementComponent>();
			d_Component = newElement3->GetComponent<DrawComponent>();
			ui_Component->ButtonFunction = ButtonFunction;
			newElement3->GetComponent<CollisionComponent>()->m_i_CollisionGroupVec.push_back(kCollGroupUIButton);
			text_Component = newElement3->GetComponent<TextRendererComponent>();
			break;
		}
	}
	// Check if null
	if (ui_Component)
	{
		if (num != kCanvasTextLabel)
		{
			//	Set the draw depth and initializes the draw component
			m_po_GraphicsManager->InitializeDrawComponent(d_Component, uiElementSprite);
			d_Component->m_f_DrawPriority = 1;
		}

		//	Sets the texture pointers
		ui_Component->m_x_BasicSprite = m_po_GraphicsManager->FetchTexture(uiElementSprite);
		if (strcmp(uiHoverSprite, "") != 0)
			ui_Component->m_x_HoverSprite = m_po_GraphicsManager->FetchTexture(uiHoverSprite);
		else
			ui_Component->m_x_HoverSprite = nullptr;
		if (strcmp(uiClickSprite, "") != 0)
			ui_Component->m_x_ClickSprite = m_po_GraphicsManager->FetchTexture(uiClickSprite);
		else
			ui_Component->m_x_ClickSprite = nullptr;

		ui_Component->m_f_XOffset = initialOffset.x * m_o_ScreenSize.x * 2;
		ui_Component->m_f_YOffset = initialOffset.y * m_o_ScreenSize.y * 2;
		t_Component->SetRotation(0);
		
		//	Calculates the offset text label for center alignment
		if (strcmp(uiText, "") != 0)
		{
			int length = static_cast<int>(strlen(uiText) + 1);
			ui_Component->m_pc_ElementText = new char[length];
			strcpy_s(ui_Component->m_pc_ElementText, length, uiText);
			if (text_Component)
			{
				int stringLen = static_cast<int>(strlen(ui_Component->m_pc_ElementText) + 1);

				//TODO FIGURE OUT A WAY TO MEASURE FONT SIZES
				text_Component->CreateText(ui_Component->m_f_XOffset + static_cast<float>(-stringLen  * 7.5f),
							  ScreenSizeY -ui_Component->m_f_YOffset + -13.5f, ui_Component->m_pc_ElementText,textColor);
			}
		}
		canvasComponent->m_x_CanvasElementList.push_back(ui_Component->m_po_OwnerEntity);
	}
}

/*
	Event Receive for creation of new elements
*/
void CanvasUISystem::Receive(const Events::EV_NEW_UI_ELEMENT& eventData)
{
	AddElement(eventData.canvas,			eventData.initialPosition,		eventData.elementType, 
			   eventData.elementEntityName, eventData.uiElementSpriteName, 
			   eventData.uiTextLabel,		eventData.uiHoverSpriteName,	
			   eventData.uiClickSpriteName,eventData.ButtonPressFunc,eventData.textColor);
}

/*
	Event Receive for collision events
*/
void CanvasUISystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	// Mouse Collision 
	if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupMouse && eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupUIButton)
	{
		auto elementComp = eventData.object2->m_po_OwnerEntity->GetComponent<CanvasElementComponent>();
		auto collisionComponent = eventData.object2->m_po_OwnerEntity->GetComponent<CollisionComponent>();
		collisionComponent->m_b_Colliding = true;

		//TODO
		//Wait for Chus input system
		//On mouse click carry on with this 
		if (GetAsyncKeyState(VK_LBUTTON) < 0)
		{
			auto drawComponent = eventData.object1->m_po_OwnerEntity->GetComponent<DrawComponent>();
			drawComponent->m_px_Texture = elementComp->m_x_HoverSprite;
			elementComp->m_b_IsClicked = true;
			if (elementComp && elementComp->ButtonFunction && !m_b_IsMousePressed)
			{
				m_b_IsMousePressed = true;
				elementComp->ButtonFunction(m_po_SystemManager);
			}
			//TODO Reform this to variadic functions
		}
		else
		{
			elementComp->m_b_IsClicked = false;
			m_b_IsMousePressed = false;
		}
	}
}