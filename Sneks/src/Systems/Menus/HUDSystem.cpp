/* Start Header****************************************************************/
/*!
\file HUDSystem.cpp
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "HUDSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../CameraSystem.h"

float oldScale1 = 0, oldScale2 = 0;

HUDSystem::HUDSystem(GraphicsSystem* graphics)
{
	m_o_GraphicsSystem = graphics;
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

void HUDSystem::Initialize()
{
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Heads Up Display");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();

	/* position of HUD: each texture already has the appropriate alpha offset so they all use the same position */
	float X = 0.5f;
	float Y = 0.083f;

	/* Bar Meters */
	Events::EV_NEW_UI_ELEMENT LBarElement ={ canvasComponent,HTVector2{ X , Y } ,kCanvasBasicSprite, "LBar", "LeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sLBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sLBar", "SmallLeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RBar", "RightBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sRBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sRBar", "SmallRightBar","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT HUDElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "Display", "HUD","","","", nullptr };

	/* Life Orbs */
	Events::EV_NEW_UI_ELEMENT RLifeElement1 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL1", "LifeR1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement2 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL2", "LifeR2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RLifeElement3 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RL3", "LifeR3","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement1 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL1", "LifeL1","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement2 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL2", "LifeL2","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT LLifeElement3 ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "LL3", "LifeL3","","","", nullptr };

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
	CanvasComponent * can_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "Heads Up Display")->GetComponent<CanvasComponent>();

	auto snek = m_po_SystemManager->GetSystem<SnekSystem>("Snek");

	/* Updating Lives */

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "LL", 2)) // Enable all orbs
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(1.0f);
		}
	}

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "RL", 2)) // Enable all orbs
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(1.0f);
		}
	}

	/* Check for number of P1 lives */
	switch (snek->GetLives(0)) {
	case 0:
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "RL3")) // Remove third orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "RL2")) // Remove second orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "RL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;

	case 1:
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "RL2")) // Remove second orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "RL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;

	case 2:
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "RL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;
	}

	/* Check for number of P2 lives */
	switch (snek->GetLives(1)) {
	case 0: // no lives
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "LL3")) // Remove third orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "LL2")) // Remove second orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "LL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;

	case 1: // 1 life
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "LL2")) // Remove second orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
			if (!strcmp(element->m_pc_EntityName, "LL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;

	case 2: // 2 lives
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "LL1")) // Remove first orb
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;
	}

	/* Updating growth meter */

	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	float difference;

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "RBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetGrowthPercentage(0) * 960);

			difference = (960.0f / screenX * snek->GetGrowthPercentage(0) - 960.0f / screenX * oldScale1) / 2.0f;

			// Bar offset
			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);

			oldScale1 = snek->GetGrowthPercentage(0); // keep track of old scale
		}

		if (!strcmp(element->m_pc_EntityName, "LBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetGrowthPercentage(1) * 960);

			difference = (960.0f / screenX * snek->GetGrowthPercentage(1) - 960.0f / screenX * oldScale2) / 2.0f;

			// Bar offset
			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);

			oldScale2 = snek->GetGrowthPercentage(1); // keep track of old scale
		}
	}

	/* Updating special attack meter */

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "sRBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetSpecialAttackPercentage(0) * 960);

			difference = (960.0f / screenX * snek->GetSpecialAttackPercentage(0) - 960.0f / screenX * oldScale1) / 2.0f;

			// Bar offset
			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);

			oldScale1 = snek->GetSpecialAttackPercentage(0); // keep track of old scale
		}

		if (!strcmp(element->m_pc_EntityName, "sLBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetSpecialAttackPercentage(1) * 960);

			difference = (960.0f / screenX * snek->GetSpecialAttackPercentage(1) - 960.0f / screenX * oldScale2) / 2.0f;

			// Bar offset
			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);

			oldScale2 = snek->GetSpecialAttackPercentage(1); // keep track of old scale
		}
	}
}
