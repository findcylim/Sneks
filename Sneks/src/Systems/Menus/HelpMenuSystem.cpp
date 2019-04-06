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

void HelpMenuSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_MOUSE_ONCLICK>(this, this);
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "HelpMenuEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	

	Events::EV_NEW_UI_ELEMENT HelpMenuUIElement = { canvas_Component, HTVector2{ 0.5f ,0.5f } ,kCanvasBasicSprite,"UIHelpMenu" ,"UIHelpMenu" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement = { canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(HelpMenuUIElement);
	

	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");

}

HelpMenuSystem::~HelpMenuSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_MOUSE_ONCLICK>(this);
	m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "HelpMenuEntity"));
}

void HelpMenuSystem::Receive(const Events::EV_MOUSE_ONCLICK& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if (m_f_Timer < 0)
		GameStateManager::SetState(m_e_PrevState);
}
void HelpMenuSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
	m_f_Timer -= dt;
}

void HelpMenuSystem::SetNextState(State nextState)
{
	m_e_PrevState = nextState;
}

void HelpMenuSystem::OnEnable()
{
	m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");
	m_e_PrevState = GameStateManager::ReturnPreviousState();
}

void HelpMenuSystem::OnDisable()
{
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("HelpMenuEntity");
}

