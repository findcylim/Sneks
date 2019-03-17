#include "HUDSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../CameraSystem.h"


HUDSystem::HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_po_EventManagerPtr = eventManager;
	//auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	//cameraComponent->m_f_VirtualOffset.x = -AEGfxGetWinMaxX();
	//cameraComponent->m_f_VirtualOffset.y = AEGfxGetWinMaxY();
	//cameraComponent->m_f_VirtualScale = 1.0f;
}

HUDSystem::~HUDSystem()
{
	auto UI = m_po_ComponentManager->GetFirstComponentInstance<CanvasComponent>(kComponentCanvas);
	while (UI)
	{
		for (auto& element : UI->m_x_CanvasElementList)
		{
			m_po_EntityManager->AddToDeleteQueue(element);
		}
		UI->m_x_CanvasElementList.clear();
		UI = static_cast<CanvasComponent*>(UI->m_po_NextComponent);
	}
}

void HUDSystem::Initialize(CanvasComponent* canvasComponent)
{
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	float X = 0.5f;
	float Y = 0.083f;

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

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LBarElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sLBarElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RBarElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sRBarElement);

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HUDElement);

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement1);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement2);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement3);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement1);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement2);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement3);
}

void HUDSystem::Update(float dt)
{
	(void)dt;
	CanvasComponent * can_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"Heads Up Display")->GetComponent<CanvasComponent>();

	switch (GetP1Lives()) {
	case 0: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "RL3"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}

	}
	break;

	case 1: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "RL2"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
	}
	break;

	case 2: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "RL1"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
	}
	break;

	case 3: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "RL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(100);
		}
	}
	break;
	}


	switch (GetP2Lives()) {
	case 0: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "LL3"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
	}
	break;

	case 1: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "LL2"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
	}
	break;

	case 2: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "LL1"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}

	}
	break;

	case 3: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "LL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(100);
		}
	}
	break;
	}
}
