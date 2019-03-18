#include "WinScreenSystem.h"
#include "../../Utility/GameStateManager.h"


void Restart()
{
	//GameStateManager::SetState()
}

void QuitToMain()
{
	GameStateManager::SetState(kStateMainMenu);
}

WinScreenSystem::WinScreenSystem(EntityManager* entityManager, EventManager* eventManager, char winner)
	:BaseSystem{entityManager}
{
	if (winner)
	{ }
	m_po_EventManagerPtr = eventManager;
	auto canvas = entityManager->NewEntity<CanvasEntity>(kEntityCanvas, "WinScreenEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT WinScreenUIElement;

	WinScreenUIElement = { canvas_Component, HTVector2{ 0.5f ,0.4f } ,kCanvasBasicSprite,"WinScreen" ,"Player1Win" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RestartUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"RestartButton" ,"UIBack" ,"Restart","UIBack_Hover","UIBack_Click", Restart };

	Events::EV_NEW_UI_ELEMENT ReturnToMainUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"ReturnToMainButton" ,"UIBack" ,"Return To Main Menu","UIBack_Hover","UIBack_Click", QuitToMain };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(WinScreenUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RestartUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ReturnToMainUIElement);
}

WinScreenSystem::~WinScreenSystem()
{
	//m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"WinScreenEntity"));
}


void WinScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}
