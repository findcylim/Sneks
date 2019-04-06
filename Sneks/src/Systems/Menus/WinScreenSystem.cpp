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

#include "WinScreenSystem.h"
#include "../../Utility/GameStateManager.h"


void Restart(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateGame);
}

void QuitToMain(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateMainMenu);
}

WinScreenSystem::WinScreenSystem()
{

}
WinScreenSystem::~WinScreenSystem()
{
	//m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"WinScreenEntity"));
}

void WinScreenSystem::Initialize()
{
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "WinScreenEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT WinScreenUIElement, LoseScreenUIElement;

	WinScreenUIElement ={ canvas_Component, HTVector2{ 0.25f ,0.5f } ,kCanvasBasicSprite,"WinScreen" ,"WinSprite" ,"","","", nullptr };
	LoseScreenUIElement ={ canvas_Component, HTVector2{ 0.75f ,0.5f } ,kCanvasBasicSprite,"LoseScreen" ,"LoseSprite" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RestartUIElement =
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"RestartButton" ,"UIBack" ,"Restart","UIBack_Hover","UIBack_Click", Restart };

	Events::EV_NEW_UI_ELEMENT ReturnToMainUIElement =
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"ReturnToMainButton" ,"UIBack" ,"Return To Main Menu","UIBack_Hover","UIBack_Click", QuitToMain };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(WinScreenUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LoseScreenUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RestartUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ReturnToMainUIElement);
}


void WinScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}



void WinScreenSystem::SwapWinScreen()
{
	CanvasElementComponent* TWin_Comp =
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "WinScreen")
		->GetComponent<CanvasElementComponent>();
	CanvasElementComponent* TLose_Comp =
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "LoseScreen")
		->GetComponent<CanvasElementComponent>();

	if (TWin_Comp && TLose_Comp)
	{
		HTVector2 tempPosition = { TWin_Comp->m_f_XOffset,TWin_Comp->m_f_YOffset };
		TWin_Comp->m_f_XOffset = TLose_Comp->m_f_XOffset;
		TWin_Comp->m_f_YOffset = TLose_Comp->m_f_YOffset;
		
		TLose_Comp->m_f_XOffset = tempPosition.x;
		TLose_Comp->m_f_YOffset = tempPosition.y;
	}
}