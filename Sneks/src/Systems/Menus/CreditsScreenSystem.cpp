#include "../../stdafx.h"
#include "CreditsScreenSystem.h"


CreditsScreenSystem::CreditsScreenSystem(EntityManager* entityManager,EventManager* eventManager, GraphicsSystem* graphicsSystem)
	:BaseSystem{ entityManager }
{
	m_po_EventManagerPtr = eventManager;
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
													   kCanvasBasicSprite,"CreditPortrait" ,"CY" ,"","","", nullptr };
	

	Events::EV_NEW_UI_ELEMENT MemberNameUIElement = { canvas_Component, HTVector2{ 0.37f , 0.5f } ,
														 kCanvasTextLabel,"NameTextLabelEntity" ,"" ,"Lim Chu Yan","","", nullptr ,HTColor{1.0f,1.0f,1.0f,0.0f} };
	Events::EV_NEW_UI_ELEMENT MemberRoleUIElement = { canvas_Component, HTVector2{ 0.37f , 0.45f } ,
														 kCanvasTextLabel,"RoleTextLabelEntity" ,"" ,"Tech Lead","","", nullptr,HTColor{1.0f,1.0f,1.0f,0.0f} };

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
		if (m_i_PortraitValue > 3)
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

