#include "PauseMenuSystem.h"
#include "../../Utility/GameStateManager.h"

void Pause_Continue()
{
	GameStateManager::SetState(kStateGame);
}

void Pause_Restart()
{
	GameStateManager::SetState(kStateRestart);
}

void Pause_QuitToMain()
{
	GameStateManager::SetState(kStateMainMenu);
}

PauseMenuSystem::PauseMenuSystem(EntityManager* entityManager, EventManager* eventManager)
	:BaseSystem{ entityManager }
{
	m_po_EventManagerPtr = eventManager;
}

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
}
