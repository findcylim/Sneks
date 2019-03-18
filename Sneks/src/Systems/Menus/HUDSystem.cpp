#include "HUDSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../CameraSystem.h"

HUDSystem::HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GraphicsSystem* graphics)
	:BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
	m_po_EventManagerPtr = eventManager;
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_f_VirtualPosition.x = -AEGfxGetWinMaxX();
	cameraComponent->m_f_VirtualPosition.y = AEGfxGetWinMaxY();
	cameraComponent->m_f_VirtualScale = 1.0f;
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

	//Events::EV_NEW_UI_ELEMENT LBarElement = { canvasComponent,HTVector2{ X + 0.139125f, Y - 0.018f } ,kCanvasBasicSprite, "LBar", "LeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sLBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sLBar", "SmallLeftBar","","","", nullptr };
	//Events::EV_NEW_UI_ELEMENT RBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RBar", "RightBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sRBarElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sRBar", "SmallRightBar","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT HUDElement = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "Display", "HUD","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RLifeElement1 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL1", "LifeR1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement2 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL2", "LifeR2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement3 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL3", "LifeR3","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement1 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL1", "LifeL1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement2 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL2", "LifeL2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement3 = { canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL3", "LifeL3","","","", nullptr };

	//m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LBarElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sLBarElement);
	//m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RBarElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(sRBarElement);

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HUDElement);

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement1);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement2);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RLifeElement3);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement1);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement2);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LLifeElement3);
}

void HUDSystem::CreateBar1()
{
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	if (auto entityCheck = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "Bar1Entity"))
	{
		for (auto& element : entityCheck->GetComponent<CanvasComponent>()->m_x_CanvasElementList)
		{
			m_po_EntityManager->AddToDeleteQueue(element);
		}
		m_po_EntityManager->AddToDeleteQueue(entityCheck);
	}

	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Bar1Entity");
	auto canvasComponent = canvas->GetComponent<CanvasComponent>();

	float barTransformX = 0.5f + 0.034125f;
	float barTransformY = 0.083f;

	for (int i = 0; i <= (int)GetP1GrowthPercentage(); ++i)
	{
		Events::EV_NEW_UI_ELEMENT BarElement;

		barTransformX += i * (0.05f/screenX);

		if (i / 100)
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, "Bar1100", "GrowthBarFill","","","", nullptr };
		else if (i / 10)
		{
			char elementName[7] = { 'B', 'a', 'r', '1', (char)(i / 10 + '0'), (char)(i % 10 + '0'), '\0' };
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, elementName, "GrowthBarFill","","","", nullptr };
		}
		else
		{
			char elementName[6] = { 'B', 'a', 'r', '1', (char)(i + '0'), '\0' };
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, elementName, "GrowthBarFill","","","", nullptr };
		}
		m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(BarElement);
	}
}

void HUDSystem::CreateBar2()
{
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	if (auto entityCheck = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "Bar2Entity"))
	{
		for (auto& element : entityCheck->GetComponent<CanvasComponent>()->m_x_CanvasElementList)
		{
			m_po_EntityManager->AddToDeleteQueue(element);
		}
		m_po_EntityManager->AddToDeleteQueue(entityCheck);
	}

	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Bar2Entity");
	auto canvasComponent = canvas->GetComponent<CanvasComponent>();

	float barTransformX = 0.5f - 0.034125f;
	float barTransformY = 0.083f;

	for (int i = 0; i <= (int)GetP2GrowthPercentage(); ++i)
	{
		Events::EV_NEW_UI_ELEMENT BarElement;

		barTransformX -= i * (0.05f / screenX);

		if (i / 100)
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, "Bar2100", "GrowthBarFill","","","", nullptr };
		else if (i / 10)
		{
			char elementName[7] = { 'B', 'a', 'r', '2', (char)(i / 10 + '0'), (char)(i % 10 + '0'), '\0' };
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, elementName, "GrowthBarFill","","","", nullptr };
		}
		else
		{
			char elementName[6] = { 'B', 'a', 'r', '2', (char)(i + '0'), '\0' };
			BarElement = { canvasComponent,HTVector2{ barTransformX, barTransformY } ,kCanvasBasicSprite, elementName, "GrowthBarFill","","","", nullptr };
		}
		m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(BarElement);
	}
}


void HUDSystem::Update(float dt)
{
	(void)dt;
	CanvasComponent * can_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"Heads Up Display")->GetComponent<CanvasComponent>();

	/* Updating Lives */

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "LL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(100);
		}
	}

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "RL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(100);
		}
	}

	switch (GetP1Lives()) {
	case 0: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "RL3"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
		if (!strcmp(element->m_pc_EntityName, "RL2"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
		if (!strcmp(element->m_pc_EntityName, "RL1"))
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
		if (!strcmp(element->m_pc_EntityName, "RL1"))
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
	}

	switch (GetP2Lives()) {
	case 0: for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "LL3"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
		if (!strcmp(element->m_pc_EntityName, "LL2"))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(0);
		}
		if (!strcmp(element->m_pc_EntityName, "LL1"))
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
		if (!strcmp(element->m_pc_EntityName, "LL1"))
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
	}
	
	/* Updating growth meter */

	CreateBar1();
	CreateBar2();

}
