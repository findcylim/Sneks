#include "WinScreenSystem.h"


WinScreenSystem::WinScreenSystem(EntityManager* entityManager, EventManager* eventManager)
	:BaseSystem{entityManager}
{
	m_o_EventManagerPtr = eventManager;
	auto canvas = entityManager->NewEntity<CanvasEntity>(kEntityCanvas, "WinScreenEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT WinScreenUIElement = { canvas_Component, HTVector2{ 0.5f ,0.2f } ,kCanvasBasicSprite,"WinScreen" ,"Player1Win" ,"","","", nullptr };
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(WinScreenUIElement);
}

WinScreenSystem::~WinScreenSystem()
{

}