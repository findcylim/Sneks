#include "CountdownSystem.h"

CountdownSystem::CountdownSystem(EntityManager* entityManager, EventManager* eventManager)
	:BaseSystem{ entityManager }
{
	m_po_EventManagerPtr = eventManager;
	auto canvas = entityManager->NewEntity<CanvasEntity>(kEntityCanvas, "CountdownEntity");

	auto canvas_Component = canvas->GetComponent<CanvasComponent>();

	Events::EV_NEW_UI_ELEMENT CountdownElement = { canvas_Component, HTVector2{ 0.5f ,0.4f } ,kCanvasBasicSprite,"Countdown" ,"Player1Win" ,"","","", nullptr }; //placeholder, replace with countdown animation

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"CountdownBackground" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CountdownElement);
}

CountdownSystem::~CountdownSystem()
{
	//m_po_EntityManager->AddToDeleteQueue(m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas,"CountdownEntity"));
}


void CountdownSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}
#pragma once
