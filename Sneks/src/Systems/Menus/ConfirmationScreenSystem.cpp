#include "../../stdafx.h"
#include "ConfirmationScreenSystem.h"


State ConfirmationScreenSystem::m_e_PrevState = kStateErrorState;

ConfirmationScreenSystem::ConfirmationScreenSystem()
{
}

void ConfirmationScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}


ConfirmationScreenSystem::~ConfirmationScreenSystem()
{

	m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "CreditsMenuEntity"));
}

void YesClick(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	if(ConfirmationScreenSystem::m_e_PrevState == kStatePause)
		GameStateManager::SetState(kStateMainMenu);
	else if(ConfirmationScreenSystem::m_e_PrevState == kStateMainMenu)
		GameStateManager::SetState(kStateExit);
}

void NoClick(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(ConfirmationScreenSystem::m_e_PrevState);
}


void ConfirmationScreenSystem::Initialize()
{

	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "ConfirmationScreen");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();


	//CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	Events::EV_NEW_UI_ELEMENT ConfirmationImage =
	{ canvasComponent,HTVector2{ 0.5f ,0.3f } ,kCanvasBasicSprite,"ConfirmationImage" ,"ConfirmationLogo" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT YesElement =
	{ canvasComponent,HTVector2{ 0.3f , 0.6f } ,kCanvasButton,"YesButton" ,"UIBack" ,"Yes","UIBack_Hover","UIBack_Click",YesClick };

	Events::EV_NEW_UI_ELEMENT NoElement =
	{ canvasComponent,HTVector2{ 0.7f , 0.6f } ,kCanvasButton,"NoButton" ,"UIBack" ,"No","UIBack_Hover","UIBack_Click",NoClick };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvasComponent, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"ConfirmationBackground" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ConfirmationImage);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(YesElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(NoElement);

	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
}


void ConfirmationScreenSystem::SetNextState(State nextState)
{
	m_e_PrevState = nextState;
}

void ConfirmationScreenSystem::OnEnable()
{
	m_po_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
	m_e_PrevState = GameStateManager::ReturnPreviousState();
	m_b_ClickHold = true;
}

void ConfirmationScreenSystem::OnDisable()
{
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
}

