#include "CanvasUISystem.h"
#include "../Utility/FileIO.h"


CanvasUISystem::CanvasUISystem(EntityManager* entityManagerPtr,GraphicsSystem* graphicsManager,EventManager* eventManager) :
	BaseSystem(entityManagerPtr)
{
	m_po_GraphicsManager = graphicsManager;
	m_o_EventManagerPtr = eventManager;
}


CanvasUISystem::~CanvasUISystem()
{
	m_o_EventManagerPtr->RemoveListener<Events::EV_NEW_UI_ELEMENT>(this);
	m_o_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void CanvasUISystem::Update(float dt) 
{
	(void)dt;
	/*for (auto uiElement : m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas)->m_x_CanvasElementList)
	{
		if (uiElement->m_b_IsActive)
		{
			CollisionComponent* col_Component = uiElement->GetComponent<CollisionComponent>();
			if (col_Component)
			{
				
			}
		}
	}*/
}


void CanvasUISystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_NEW_UI_ELEMENT>(this);
	m_o_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this);
}

void CanvasUISystem::ClearUI(CanvasComponent* canvas)
{
	for (auto& element : canvas->m_x_CanvasElementList)
	{
		m_po_EntityManager->AddToDeleteQueue(element);
	}
	canvas->m_x_CanvasElementList.clear();
}

void CanvasUISystem::AddElement(CanvasComponent* canvasComponent, HTVector2 initPosition,CanvasElementEnum num, const char * name, 
								const char * uiElementSprite, const char* uiText,const char * uiHoverSprite, const char * uiClickSprite,void(*ButtonFunction)())
{
	CanvasElementComponent* ui_Component = nullptr;
	TransformComponent *    t_Component = nullptr;
	DrawComponent *         d_Component = nullptr;
	switch (num)
	{
		case kCanvasTextLabel:
		{
			CanvasTextLabelEntity* newElement1 = m_po_EntityManager->NewEntity<CanvasTextLabelEntity>(kEntityCanvasTextLabel, name);
			t_Component = newElement1->GetComponent<TransformComponent>();
			ui_Component = newElement1->GetComponent<CanvasElementComponent>();
			d_Component = newElement1->GetComponent<DrawComponent>();
			break;
		}
		case kCanvasBasicSprite:
		{
			CanvasBasicSpriteEntity* newElement2 = m_po_EntityManager->NewEntity<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, name);
			t_Component = newElement2->GetComponent<TransformComponent>();
			ui_Component = newElement2->GetComponent<CanvasElementComponent>();
			d_Component = newElement2->GetComponent<DrawComponent>();
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
			break;
		}
	}
	if (ui_Component)
	{
		int x = 0, y = 0;
		
		m_po_GraphicsManager->InitializeDrawComponent(d_Component, uiElementSprite);
		
		ui_Component->m_x_BasicSprite = m_po_GraphicsManager->FetchTexture(uiElementSprite);
		if (strcmp(uiHoverSprite, "") != 0)
			ui_Component->m_x_HoverSprite = m_po_GraphicsManager->FetchTexture(uiHoverSprite);
		if (strcmp(uiClickSprite, "") != 0)
			ui_Component->m_x_ClickSprite = m_po_GraphicsManager->FetchTexture(uiClickSprite);
		//Adjusts the origin to the top left corner of the sprite
		FileIO::ReadPngDimensions(d_Component->m_px_Texture->mpName, &x, &y);
		x /=2;
		y /=2;
		t_Component->m_x_Position = { initPosition.x + x, -(initPosition.y) - y };
		t_Component->SetRotation(0);

		if (strcmp(uiText, "") != 0)
		{
			int length = static_cast<int>(strlen(uiText) + 1);
			ui_Component->m_pc_ElementText = new char[length];
			strcpy_s(ui_Component->m_pc_ElementText, length, uiText);
		}
		canvasComponent->m_x_CanvasElementList.push_back(ui_Component->m_po_OwnerEntity);
	}
	else
	{
		
	}
}

void CanvasUISystem::Receive(const Events::EV_NEW_UI_ELEMENT& eventData)
{
	AddElement(eventData.canvas,			eventData.initialPosition,		eventData.elementType, 
			   eventData.elementEntityName, eventData.uiElementSpriteName, 
			   eventData.uiTextLabel,		eventData.uiHoverSpriteName,	
			   eventData.uiClickSpriteName,eventData.ButtonPressFunc);
}

void CanvasUISystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	if (eventData.object1->m_i_CollisionGroupVec[0] == kCollGroupUIButton && eventData.object2->m_i_CollisionGroupVec[0] == kCollGroupMouse)
	{
		auto comp = eventData.object1->m_po_OwnerEntity->GetComponent<CanvasElementComponent>();
		//TODO
		//Wait for Chus input system
		//On mouse click carry on with this 
		if(comp->ButtonFunction)
			comp->ButtonFunction();
	}
}