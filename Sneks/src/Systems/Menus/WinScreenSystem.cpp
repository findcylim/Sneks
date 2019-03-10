#include "WinScreenSystem.h"


WinScreenSystem::WinScreenSystem(EntityManager* entityManager, EventManager* eventManager, char winner)
	:BaseSystem{entityManager}
{
	if (winner)
	{ }
	m_o_EventManagerPtr = eventManager;
	auto canvas = entityManager->NewEntity<CanvasEntity>(kEntityCanvas, "WinScreenEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT WinScreenUIElement;

	WinScreenUIElement = { canvas_Component, HTVector2{ 0.5f ,0.4f } ,kCanvasBasicSprite,"WinScreen" ,"Player1Win" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RestartUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"RestartButton" ,"UIBack" ,"Restart","UIBack_Hover","UIBack_Click", nullptr };

	Events::EV_NEW_UI_ELEMENT ReturnToMainUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"ReturnToMainButton" ,"UIBack" ,"Return To Main Menu","UIBack_Hover","UIBack_Click", nullptr };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(WinScreenUIElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(RestartUIElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(ReturnToMainUIElement);
}

WinScreenSystem::~WinScreenSystem()
{

}


void WinScreenSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}
