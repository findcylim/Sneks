/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 05/04/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "../../stdafx.h"
#include "CreditsScreenSystem.h"


CreditsScreenSystem::CreditsScreenSystem(GraphicsSystem* graphicsSystem)
{
	m_po_GraphicsSystem = graphicsSystem;
}

void CreditsScreenSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_MOUSE_ONCLICK>(this, this);
	auto canvas				= m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "CreditsMenuEntity");
	auto canvas_Component	= canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT CreditsBackgroundUIElement = { canvas_Component, HTVector2{ 0.5f ,0.5f } ,
													   kCanvasBasicSprite,"CreditBackground" ,"Credits-Background" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT CreditsMenuUIElement = { canvas_Component, HTVector2{ 0.622f ,0.499f } ,
													   kCanvasBasicSprite,"CreditPortrait" ,"Spoodermun" ,"","","", nullptr };
	

	Events::EV_NEW_UI_ELEMENT MemberNameUIElement = { canvas_Component, HTVector2{ 0.37f , 0.5f } ,
														 kCanvasTextLabel,"NameTextLabelEntity" ,"" ,"Javier Foo","","", nullptr ,HTColor{1.0f,1.0f,1.0f,0.0f} };
	Events::EV_NEW_UI_ELEMENT MemberRoleUIElement = { canvas_Component, HTVector2{ 0.37f , 0.45f } ,
														 kCanvasTextLabel,"RoleTextLabelEntity" ,"" ,"Lead Designer","","", nullptr,HTColor{1.0f,1.0f,1.0f,0.0f} };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement = { canvas_Component, HTVector2{ 0.5f , 0.5f } ,
														 kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CreditsBackgroundUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CreditsMenuUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(MemberNameUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(MemberRoleUIElement);
	
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("CreditsMenuEntity");
}

CreditsScreenSystem::~CreditsScreenSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_MOUSE_ONCLICK>(this);
	m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "CreditsMenuEntity"));
}

void CreditsScreenSystem::Receive(const Events::EV_MOUSE_ONCLICK& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	if(!m_b_ClickHold)
		GameStateManager::SetState(m_e_PrevState);
}

void CreditsScreenSystem::UpdateText(CanvasTextLabelEntity* textEntity,const char * nameText,HTVector2 ScreenOffset)
{
	auto NameTextRenderer = textEntity->GetComponent<TextRendererComponent>();
	auto NameUIComponent = textEntity->GetComponent<CanvasElementComponent>();
	NameTextRenderer->CreateText(NameUIComponent->m_f_XOffset + static_cast<float>(strlen(nameText) * -7.5f),
		ScreenOffset.y - NameUIComponent->m_f_YOffset + -13.5f, nameText, HTColor{ 1,1,1,1 });

}

void CreditsScreenSystem::Update(float dt)
{
	m_f_Timer -= dt;
	if (m_f_Timer < 0)
	{
		m_b_ClickHold = false;
		CanvasBasicSpriteEntity* PortraitEntity = 
			m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "CreditPortrait");
		CanvasTextLabelEntity* NameEntity = 
			m_po_EntityManager->GetSpecificEntityInstance<CanvasTextLabelEntity>(kEntityCanvasTextLabel, "NameTextLabelEntity");
		CanvasTextLabelEntity* RoleEntity =
			m_po_EntityManager->GetSpecificEntityInstance<CanvasTextLabelEntity>(kEntityCanvasTextLabel, "RoleTextLabelEntity");

		m_f_Timer = 3.0f;
		m_i_PortraitValue++;
		if (m_i_PortraitValue > 8)
			m_i_PortraitValue = 0;


		HTVector2 ScreenSize;
		AlphaEngineHelper::GetScreenSize(&ScreenSize.x, &ScreenSize.y);


		switch (m_i_PortraitValue)
		{
		case 0:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("Spoodermun");
			
			UpdateText(NameEntity, "Javier Foo", ScreenSize);
			UpdateText(RoleEntity, "Lead Designer", ScreenSize);
			break;
		case 1:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("CY");

			UpdateText(NameEntity, "Lim Chu Yan", ScreenSize);
			UpdateText(RoleEntity, "Tech Lead", ScreenSize);
			break;
		case 2:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("Edwin");

			UpdateText(NameEntity, "Edwin Khoo", ScreenSize);
			UpdateText(RoleEntity, "Product Manager", ScreenSize);
			break;
		case 3:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("Adam");

			UpdateText(NameEntity, "Adam Amin", ScreenSize);
			UpdateText(RoleEntity, "Producer", ScreenSize);
			break;
		case 4:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("DigipenSquareLogo");
			UpdateText(RoleEntity, "Game Instructor", ScreenSize);
			UpdateText(NameEntity, "Prior Kevin Andrew", ScreenSize);
			break;
		case 5:
			UpdateText(RoleEntity, "Game Instructor", ScreenSize);
			UpdateText(NameEntity, "Hosry Elie", ScreenSize);
			break;
		case 6:
			UpdateText(RoleEntity, "President", ScreenSize);
			UpdateText(NameEntity, "Claude Comair", ScreenSize);
			break;
		case 7:
			UpdateText(NameEntity, "© 2019 DigiPen Corporation (SG)", ScreenSize);
			UpdateText(RoleEntity, "All Rights Reserved", ScreenSize);
			break;
		case 8:
			PortraitEntity->GetComponent<DrawComponent>()->m_px_Texture = m_po_GraphicsSystem->FetchTexture("FmodLogo");
			UpdateText(RoleEntity, "Made with FMOD Studio By", ScreenSize);
			UpdateText(NameEntity, "Firelight Technologies Pty Ltd.", ScreenSize);
			break; 
		}
	}
}

void CreditsScreenSystem::SetNextState(State nextState)
{
	m_e_PrevState = nextState;
}

void CreditsScreenSystem::OnEnable()
{
	m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("CreditsMenuEntity");
	m_e_PrevState = GameStateManager::ReturnPreviousState();
	m_b_ClickHold = true;
}

void CreditsScreenSystem::OnDisable()
{
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("CreditsMenuEntity");
}


