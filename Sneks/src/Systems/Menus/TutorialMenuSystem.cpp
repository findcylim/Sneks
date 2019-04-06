/* Start Header****************************************************************/
/*!
\file TutorialMenuSystem.cpp
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier     - 100.00% 

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "TutorialMenuSystem.h"
#include "../../Utility/AlphaEngineHelper.h"

TutorialMenuSystem::TutorialMenuSystem(GameStateManager* gameStateManager)
{
	m_po_GameStateManager = gameStateManager;
	timer = 0.0f;
}

TutorialMenuSystem::~TutorialMenuSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_MOVEMENT_KEY>(this);
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

void TutorialMenuSystem::Initialize()
{
	auto snekHead1 = m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	auto snekHead2 = static_cast<SnekHeadComponent*>(snekHead1->m_po_NextComponent);

	snekHead1->accelerationTutorial = true;
	snekHead2->accelerationTutorial = true;
	snekHead1->turningTutorial = true;
	snekHead2->turningTutorial = true;
	snekHead1->specialTutorial = true;
	snekHead2->specialTutorial = true;

	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Tutorial UI");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();

	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);

	auto snekHead1pos = 200.0f / screenX + 0.125f;
	auto snekHead2pos = 1 - 200.0f / screenX - 0.125f;

	Events::EV_NEW_UI_ELEMENT P1TurningTutorialElement = { canvasComponent, HTVector2{ snekHead1pos ,0.5f } ,kCanvasBasicSprite,"P1TurnTut" ,"P1TurnTut" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT P2TurningTutorialElement = { canvasComponent, HTVector2{ snekHead2pos ,0.5f } ,kCanvasBasicSprite,"P2TurnTut" ,"P2TurnTut" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT P1AccelTutorialElement = { canvasComponent, HTVector2{ snekHead1pos ,0.5f } ,kCanvasBasicSprite,"P1AccelTut" ,"P1AccelTut" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT P2AccelTutorialElement = { canvasComponent, HTVector2{ snekHead2pos ,0.5f } ,kCanvasBasicSprite,"P2AccelTut" ,"P2AccelTut" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT P1SpecialTutorialElement = { canvasComponent, HTVector2{ snekHead1pos ,0.5f } ,kCanvasBasicSprite,"P1SpecialTut" ,"P1SpecialTut" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT P2SpecialTutorialElement = { canvasComponent, HTVector2{ snekHead2pos ,0.5f } ,kCanvasBasicSprite,"P2SpecialTut" ,"P2SpecialTut" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P1TurningTutorialElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P2TurningTutorialElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P1AccelTutorialElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P2AccelTutorialElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P1SpecialTutorialElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(P2SpecialTutorialElement);

	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1TurnTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2TurnTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1AccelTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2AccelTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(0);

	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_MOVEMENT_KEY>(this, this);
}

void TutorialMenuSystem::Receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData)
{
	auto snekHeadComponent = m_po_ComponentManager->
		GetSpecificComponentInstance<SnekHeadComponent>(eventData.caller, kComponentSnekHead);

	if (eventData.key == Events::MOVE_KEY_UP) 
	{
		snekHeadComponent->accelerationTutorial = false;
	}
	else if (eventData.key == Events::MOVE_KEY_DOWN)
	{
		snekHeadComponent->accelerationTutorial = false;
	}
	else if (eventData.key == Events::MOVE_KEY_LEFT) 
	{
		snekHeadComponent->turningTutorial = false;
	}
	else if (eventData.key == Events::MOVE_KEY_RIGHT) 
	{
		snekHeadComponent->turningTutorial = false;
	}
}

void TutorialMenuSystem::Update(float dt)
{
	if (m_po_GameStateManager->ReturnCurrentState() == kStateGame)
		timer += dt;
	else
		timer = 0;

	auto snekHead1 = m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	auto snekHead2 = static_cast<SnekHeadComponent*>(snekHead1->m_po_NextComponent);

	if (m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvas, "Tutorial UI")->m_b_IsActive)
	{
		if (snekHead1->accelerationTutorial)
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1AccelTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1AccelTut")->GetComponent<DrawComponent>()->SetAlpha(0);

		if (snekHead1->turningTutorial && !(snekHead1->accelerationTutorial))
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1TurnTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1TurnTut")->GetComponent<DrawComponent>()->SetAlpha(0);

		if (snekHead1->specialTutorial && !(snekHead1->turningTutorial))
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else 
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P1SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(0);

		if (snekHead2->accelerationTutorial)
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2AccelTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2AccelTut")->GetComponent<DrawComponent>()->SetAlpha(0);

		if (snekHead2->turningTutorial && !(snekHead2->accelerationTutorial))
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2TurnTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2TurnTut")->GetComponent<DrawComponent>()->SetAlpha(0);

		if (snekHead2->specialTutorial && !(snekHead2->turningTutorial))
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(1.0f);
		else
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "P2SpecialTut")->GetComponent<DrawComponent>()->SetAlpha(0);
	}

	CanvasComponent * tutorial = m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "Tutorial UI")->GetComponent<CanvasComponent>();

	/* Update according to snake position */
	for (auto& element : tutorial->m_x_CanvasElementList)
	{
		// skip inactive entity
		if (!(element->m_b_IsActive))
			continue;

		TransformComponent * trans_Comp = element->GetComponent<TransformComponent>();

		if (!strncmp(element->m_pc_EntityName, "P1", 2))
		{
			trans_Comp->m_x_Position.x = (snekHead1->GetComponent<TransformComponent>()->m_x_Position.x);
			trans_Comp->m_x_Position.y = (snekHead1->GetComponent<TransformComponent>()->m_x_Position.y);
		}
		else if (!strncmp(element->m_pc_EntityName, "P2", 2))
		{
			trans_Comp->m_x_Position.x = (snekHead2->GetComponent<TransformComponent>()->m_x_Position.x);
			trans_Comp->m_x_Position.y = (snekHead2->GetComponent<TransformComponent>()->m_x_Position.y);
		}
	}

	if (timer >= 5)
		m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
	else 
		m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
}