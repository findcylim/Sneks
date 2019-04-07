/* Start Header****************************************************************/
/*!
\file HelpMenuSystem.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for Help menu tutorial

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "../../stdafx.h"
#include "HelpMenuSystem.h"

HelpMenuSystem::HelpMenuSystem()
{}

/*
	Initialize function
*/
void HelpMenuSystem::Initialize()
{
	//Add listener
	m_po_EventManagerPtr->AddListener<Events::EV_MOUSE_ONCLICK>(this, this);
	//Create the canvas
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "HelpMenuEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	

	Events::EV_NEW_UI_ELEMENT HelpMenuUIElement = { canvas_Component, HTVector2{ 0.5f ,0.5f } ,kCanvasBasicSprite,"UIHelpMenu" ,"UIHelpMenu" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement = { canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	//Creates the canvas elements
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HelpMenuUIElement);
	
	//Disables the help menu entity initially
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");

}

/*
	Destructor
*/
HelpMenuSystem::~HelpMenuSystem()
{
	//	Remove listener
	m_po_EventManagerPtr->RemoveListener<Events::EV_MOUSE_ONCLICK>(this);

	//	Deletes the canvas
	m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "HelpMenuEntity"));
}

/*
	On click it will skip the tutorial
*/
void HelpMenuSystem::Receive(const Events::EV_MOUSE_ONCLICK& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (m_f_Timer < 0)
		GameStateManager::SetState(m_e_PrevState);
}

/*
	Update function
*/
void HelpMenuSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_f_Timer -= dt;
}

/*
	Set next state function
*/
void HelpMenuSystem::SetNextState(State nextState)
{
	m_e_PrevState = nextState;
}

/*
	When the system is enabled
*/
void HelpMenuSystem::OnEnable()
{
	m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");
	m_e_PrevState = GameStateManager::ReturnPreviousState();
}

/*
	When the system is disabled
*/
void HelpMenuSystem::OnDisable()
{
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");
}

