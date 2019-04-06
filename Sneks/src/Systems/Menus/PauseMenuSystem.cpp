/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 19/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "PauseMenuSystem.h"
#include "../../Utility/GameStateManager.h"

void Pause_Continue(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateGame);
	
}

void Pause_Restart(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateRestart);
}

void Pause_QuitToMain(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateMainMenu);
}

PauseMenuSystem::PauseMenuSystem()
{}

void PauseMenuSystem::Initialize()
{
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "PauseMenuEntity");

	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	   
	Events::EV_NEW_UI_ELEMENT PauseMenuUIElement = { canvas_Component, HTVector2{ 0.5f ,0.4f } ,kCanvasBasicSprite,"PauseScreen" ,"UIGame_Paused" ,"","","", nullptr }; //placeholder, replace with PAUSED

	Events::EV_NEW_UI_ELEMENT ContinueUIElement =
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"PauseContinueButton" ,"UIBack" ,"Continue","UIBack_Hover","UIBack_Click", Pause_Continue };

	Events::EV_NEW_UI_ELEMENT RestartUIElement =
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"PauseRestartButton" ,"UIBack" ,"Restart","UIBack_Hover","UIBack_Click", Pause_Restart };

	Events::EV_NEW_UI_ELEMENT ReturnToMainUIElement =
	{ canvas_Component, HTVector2{ 0.5f ,0.8f } ,kCanvasButton,"PauseReturnToMainButton" ,"UIBack" ,"Return To Main Menu","UIBack_Hover","UIBack_Click", Pause_QuitToMain };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"PauseBackground" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(PauseMenuUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ContinueUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RestartUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ReturnToMainUIElement);
}

PauseMenuSystem::~PauseMenuSystem()
{
	//m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"PauseMenuEntity"));
}


void PauseMenuSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	/*if (AEInputCheckTriggered(AEVK_ESCAPE))
		Pause_Continue();*/
}
