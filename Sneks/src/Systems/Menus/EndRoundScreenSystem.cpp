/* Start Header****************************************************************/
/*!
\file EndRoundScreenSystem.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "EndRoundScreenSystem.h"
#include "../../Utility/GameStateManager.h"

void EndRound_Continue(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateCountdown);
	
}

void EndRound_Select(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateCharacterSelection);
}

EndRoundScreenSystem::EndRoundScreenSystem()
{
}

void EndRoundScreenSystem::Initialize()
{
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "EndRoundEntity");

	auto canvas_Component = canvas->GetComponent<CanvasComponent>();

	//Events::EV_NEW_UI_ELEMENT endRound =
	//{ canvas_Component, HTVector2{ 0.5f ,0.3f } ,kCanvasBasicSprite,"EndText" ,"Countdown" ,"Snek Destroyed!","","", nullptr,{1,1,1,1},4,1 };

	Events::EV_NEW_UI_ELEMENT endNextButton =
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"EndNextButton" ,"UIBack" ,"Next Round","UIBack_Hover","UIBack_Click", EndRound_Continue };

	Events::EV_NEW_UI_ELEMENT endSelectButton =
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"EndSelectButton" ,"UIBack" ,"Character Select","UIBack_Hover","UIBack_Click", EndRound_Select };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"ConfirmationBackground" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	//m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(pauseMenuUiElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(endNextButton);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(endSelectButton);
}

EndRoundScreenSystem::~EndRoundScreenSystem()
{
	//m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"PauseMenuEntity"));
}


void EndRoundScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}
