/* Start Header****************************************************************/
/*!
\file ConfirmationScreenSystem.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the implementation for the Confirmation screen menu

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "../../stdafx.h"
#include "ConfirmationScreenSystem.h"


State ConfirmationScreenSystem::m_e_PrevState = kStateErrorState;

/*
	Constructor	
*/
ConfirmationScreenSystem::ConfirmationScreenSystem()
{
}

/*
	Update function
*/
void ConfirmationScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

/*
	Destructor
*/
ConfirmationScreenSystem::~ConfirmationScreenSystem()
{
	m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "CreditsMenuEntity"));
}

/*
	Function for yes button
*/
void YesClick(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	if(ConfirmationScreenSystem::m_e_PrevState == kStatePause)
		GameStateManager::SetState(kStateMainMenu);
	else if(ConfirmationScreenSystem::m_e_PrevState == kStateMainMenu)
		GameStateManager::SetState(kStateExit);
}

/*
	Function for no button
*/
void NoClick(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(ConfirmationScreenSystem::m_e_PrevState);
}

/*
	Initialize function 
*/
void ConfirmationScreenSystem::Initialize()
{
	//	Creates the confirmation canvas
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "ConfirmationScreen");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();


	Events::EV_NEW_UI_ELEMENT ConfirmationImage =
	{ canvasComponent,HTVector2{ 0.5f ,0.3f } ,kCanvasBasicSprite,"ConfirmationImage" ,"ConfirmationLogo" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT YesElement =
	{ canvasComponent,HTVector2{ 0.3f , 0.6f } ,kCanvasButton,"YesButton" ,"UIBack" ,"Yes","UIBack_Hover","UIBack_Click",YesClick };

	Events::EV_NEW_UI_ELEMENT NoElement =
	{ canvasComponent,HTVector2{ 0.7f , 0.6f } ,kCanvasButton,"NoButton" ,"UIBack" ,"No","UIBack_Hover","UIBack_Click",NoClick };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvasComponent, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"ConfirmationBackground" ,"TransitionBack" ,"","","", nullptr };

	//	Create the canvas elements
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ConfirmationImage);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(YesElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(NoElement);

	//	Disables the canvas initially
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
}

/*
	Set the next state
*/
void ConfirmationScreenSystem::SetNextState(State nextState)
{
	m_e_PrevState = nextState;
}

/*
	When the system is enabled
*/
void ConfirmationScreenSystem::OnEnable()
{
	m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
	m_e_PrevState = GameStateManager::ReturnPreviousState();
	m_b_ClickHold = true;
}

/*
	When the system is disabled
*/
void ConfirmationScreenSystem::OnDisable()
{
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
}

