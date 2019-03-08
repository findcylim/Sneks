#include "HUDSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../CameraSystem.h"

float X, Y;

HUDSystem::HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_o_EventManagerPtr = eventManager;
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_f_VirtualOffsetX = -AEGfxGetWinMaxX();
	cameraComponent->m_f_VirtualOffsetY = AEGfxGetWinMaxY();
	cameraComponent->m_f_VirtualScale = 1.0f;
}

HUDSystem::~HUDSystem()
{
	/*
	auto UI = m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas);
	while (UI)
	{
		for (auto& element : UI->m_x_CanvasElementList)
		{
			m_po_EntityManager->AddToDeleteQueue(element);
		}
		UI->m_x_CanvasElementList.clear();
	}*/
}

void HUDSystem::Initialize(CanvasComponent* canvasComponent)
{
	CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);

	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	X = c_Comp->GetOffsetX() + screenX - 480;
	Y = c_Comp->GetOffsetY() - screenY/2 + 87;

	Events::EV_NEW_UI_ELEMENT LBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LBar", "LeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sLBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sLBar", "SmallLeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RBar", "RightBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sRBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sRBar", "SmallRightBar","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT HUDElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "Display", "HUD","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RLifeElement1 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL1", "LifeR1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement2 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL2", "LifeR2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement3 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL3", "LifeR3","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement1 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL1", "LifeL1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement2 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL2", "LifeL2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement3 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL3", "LifeL3","","","", nullptr };

	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LBarElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sLBarElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RBarElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sRBarElement);

	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HUDElement);

	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement1);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement2);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement3);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement1);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement2);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement3);
}

void HUDSystem::Update(float dt)
{
	(void)dt;
	 CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	CanvasComponent * can_Comp = m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas);

	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		TransformComponent * t_Comp = m_po_ComponentManager->GetSpecificComponentInstance<TransformComponent>(element, kComponentTransform);

		t_Comp->SetScale(1.0f / c_Comp->GetScale());
		t_Comp->SetPositionX(t_Comp->GetPosition().x + getCameraMovement().x);
		t_Comp->SetPositionY(t_Comp->GetPosition().y + getCameraMovement().y);

		/* X += (getCameraMovement().x * t_Comp->GetScale().x);
		Y += (getCameraMovement().y * t_Comp->GetScale().y);

		if (t_Comp->GetPosition().x < X)
		{
			while (t_Comp->GetPosition().x < X)
				t_Comp->SetPositionX(t_Comp->GetPosition().x + 0.1f);
		}

		else if (t_Comp->GetPosition().x > X)
		{
			while (t_Comp->GetPosition().x > X)
				t_Comp->SetPositionX(t_Comp->GetPosition().x - 0.1f);
		}

		if (t_Comp->GetPosition().y < Y)
		{
			while (t_Comp->GetPosition().y < Y)
				t_Comp->SetPositionY(t_Comp->GetPosition().y + 0.1f);
		}

		else if (t_Comp->GetPosition().y > Y)
		{
			while (t_Comp->GetPosition().y > Y)
				t_Comp->SetPositionY(t_Comp->GetPosition().y - 0.1f);
		} */
	}

	/*
	switch (GetP1Lives())
	{
	case 1: break;
	case 2: break;
	}

	switch (GetP2Lives()) 
	{
	case 1: break;
	case 2: break;
	}
	*/
}
