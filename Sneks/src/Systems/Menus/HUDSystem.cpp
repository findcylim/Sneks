/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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

	Events::EV_NEW_UI_ELEMENT LBarElement ={ canvasComponent,HTVector2{ X , Y } ,kCanvasBasicSprite, "LBar", "LeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sLBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sLBar", "SmallLeftBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT RBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "RBar", "RightBar","","","", nullptr };
	Events::EV_NEW_UI_ELEMENT sRBarElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "sRBar", "SmallRightBar","","","", nullptr };

	Events::EV_NEW_UI_ELEMENT HUDElement ={ canvasComponent,HTVector2{ X, Y } ,kCanvasBasicSprite, "Display", "HUD","","","", nullptr };

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
		if (!strncmp(element->m_pc_EntityName, "LL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(1.0f);
		}
	}

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strncmp(element->m_pc_EntityName, "RL", 2))
		{
			DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
			draw_Comp->SetAlpha(1.0f);
		}
	}

	switch (snek->GetLives(0)) {
	case 0:
		for (auto& element : can_Comp->m_x_CanvasElementList)
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

	case 1:
		for (auto& element : can_Comp->m_x_CanvasElementList)
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

	case 2:
		for (auto& element : can_Comp->m_x_CanvasElementList)
		{
			if (!strcmp(element->m_pc_EntityName, "RL1"))
			{
				DrawComponent * draw_Comp = element->GetComponent<DrawComponent>();
				draw_Comp->SetAlpha(0);
			}
		}
		break;
	}

	switch (snek->GetLives(1)) {
	case 0:
		for (auto& element : can_Comp->m_x_CanvasElementList)
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

	case 1:
		for (auto& element : can_Comp->m_x_CanvasElementList)
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

	case 2:
		for (auto& element : can_Comp->m_x_CanvasElementList)
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

			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);

			oldScale1 = snek->GetGrowthPercentage(0);
		}

		if (!strcmp(element->m_pc_EntityName, "LBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetGrowthPercentage(1) * 960);

			difference = (960.0f / screenX * snek->GetGrowthPercentage(1) - 960.0f / screenX * oldScale2) / 2.0f;

			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);

			oldScale2 = snek->GetGrowthPercentage(1);
		}
	}

	for (auto& element : can_Comp->m_x_CanvasElementList)
	{
		if (!strcmp(element->m_pc_EntityName, "sRBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetSpecialAttackPercentage(0) * 960);

			difference = (960.0f / screenX * snek->GetSpecialAttackPercentage(0) - 960.0f / screenX * oldScale1) / 2.0f;

			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);

			oldScale1 = snek->GetSpecialAttackPercentage(0);
		}

		if (!strcmp(element->m_pc_EntityName, "sLBar"))
		{
			TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();
			trans_Comp->m_f_Scale.x=(snek->GetSpecialAttackPercentage(1) * 960);

			difference = (960.0f / screenX * snek->GetSpecialAttackPercentage(1) - 960.0f / screenX * oldScale2) / 2.0f;

			if (difference > 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x - difference);
			else if (difference < 0)
				trans_Comp->m_x_Position.x = (trans_Comp->m_x_Position.x + difference);

			oldScale2 = snek->GetSpecialAttackPercentage(1);
		}
	}

}
