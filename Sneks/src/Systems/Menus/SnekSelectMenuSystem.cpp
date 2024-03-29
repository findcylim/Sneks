/* Start Header****************************************************************/
/*!
\file SnekSelectMenuSystem.cpp
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

#include "SnekSelectMenuSystem.h"
#include "../SnekSystem.h"

/* Constructor */
SnekSelectMenuSystem::SnekSelectMenuSystem()
{
}

/* Destructor */
SnekSelectMenuSystem::~SnekSelectMenuSystem()
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

/* Iterate through P1 Snek States */
void P1SnekStateUp(SystemManager* systemManager)
{
	auto snekSystem = systemManager->GetSystem<SnekSystem>("Snek");
	auto snekHead = snekSystem->m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	if (snekHead->m_x_SnekType == kSnekTypeFlip)
		snekSystem->SetSnekType(0, kSnekTypeSpeed);
	else if (snekHead->m_x_SnekType == kSnekTypeSpeed)
		snekSystem->SetSnekType(0, kSnekTypeShoot);
	else if (snekHead->m_x_SnekType == kSnekTypeShoot)
		snekSystem->SetSnekType(0, kSnekTypeFlip);
}

void P1SnekStateDown(SystemManager* systemManager)
{
	auto snekSystem = systemManager->GetSystem<SnekSystem>("Snek");
	auto snekHead = snekSystem->m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);

	if (snekHead->m_x_SnekType == kSnekTypeFlip)
		snekSystem->SetSnekType(0, kSnekTypeShoot);
	else if (snekHead->m_x_SnekType == kSnekTypeShoot)
		snekSystem->SetSnekType(0, kSnekTypeSpeed);
	else if (snekHead->m_x_SnekType == kSnekTypeSpeed)
		snekSystem->SetSnekType(0, kSnekTypeFlip);
}

/* Iterate through P2 Snek States */
void P2SnekStateUp(SystemManager* systemManager)
{
	auto snekSystem = systemManager->GetSystem<SnekSystem>("Snek");
	auto snekHead = snekSystem->m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	snekHead = static_cast<SnekHeadComponent*>(snekHead->m_po_NextComponent);

	if (snekHead->m_x_SnekType == kSnekTypeFlip)
		snekSystem->SetSnekType(1, kSnekTypeSpeed);
	else if (snekHead->m_x_SnekType == kSnekTypeSpeed)
		snekSystem->SetSnekType(1, kSnekTypeShoot);
	else if (snekHead->m_x_SnekType == kSnekTypeShoot)
		snekSystem->SetSnekType(1, kSnekTypeFlip);
}

void P2SnekStateDown(SystemManager* systemManager)
{
	auto snekSystem = systemManager->GetSystem<SnekSystem>("Snek");
	auto snekHead = snekSystem->m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	snekHead = static_cast<SnekHeadComponent*>(snekHead->m_po_NextComponent);

	if (snekHead->m_x_SnekType == kSnekTypeFlip)
		snekSystem->SetSnekType(1, kSnekTypeShoot);
	else if (snekHead->m_x_SnekType == kSnekTypeShoot)
		snekSystem->SetSnekType(1, kSnekTypeSpeed);
	else if (snekHead->m_x_SnekType == kSnekTypeSpeed)
		snekSystem->SetSnekType(1, kSnekTypeFlip);
}

/* Start Game */
void PlayGame(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateHelpMenu);
}

void SnekSelectMenuSystem::Initialize()
{
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Snek Select UI");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();

	/*Snek Select Banner */
	Events::EV_NEW_UI_ELEMENT LogoElement = { canvasComponent,HTVector2{ 0.5f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"Select-Banner" ,"","","", nullptr };

	/* Buttons */
	Events::EV_NEW_UI_ELEMENT P1UpElement = { canvasComponent,HTVector2{ 0.175f , 0.5f } ,kCanvasButton,"P1Up" ,"Up-Idle" ,"","Up-Hover","Up-Click",P1SnekStateUp };
	Events::EV_NEW_UI_ELEMENT P1DownElement = { canvasComponent,HTVector2{ 0.175f , 0.655f } ,kCanvasButton,"P1Down" ,"Down-Idle", "" ,"Down-Hover","Down-Click",P1SnekStateDown };
	Events::EV_NEW_UI_ELEMENT P2UpElement = { canvasComponent,HTVector2{ 0.825f , 0.5f } ,kCanvasButton,"P2Up" ,"Up-Idle", "","Up-Hover","Up-Click",P2SnekStateUp };
	Events::EV_NEW_UI_ELEMENT P2DownElement = { canvasComponent, HTVector2{ 0.825f , 0.655f }, kCanvasButton,"P2Down", "Down-Idle", "", "Down-Hover","Down-Click", P2SnekStateDown };
	Events::EV_NEW_UI_ELEMENT ConfirmElement = { canvasComponent, HTVector2{ 0.5f , 0.9f }, kCanvasButton,"Confirm", "Confirm-Idle", "", "Confirm-Hover","", PlayGame };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LogoElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P1UpElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P1DownElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P2UpElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P2DownElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ConfirmElement);
}

void SnekSelectMenuSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}