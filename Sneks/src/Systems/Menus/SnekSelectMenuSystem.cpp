#include "SnekSelectMenuSystem.h"
#include "../SnekSystem.h"

SnekSelectMenuSystem::SnekSelectMenuSystem()
{
}

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

void PlayGame(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateHelpMenu);
}

void SnekSelectMenuSystem::Initialize()
{
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Snek Select UI");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();

	Events::EV_NEW_UI_ELEMENT LogoElement = { canvasComponent,HTVector2{ 0.3f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"Select-Banner" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT P1UpElement = { canvasComponent,HTVector2{ 0.175f , 0.5f } ,kCanvasButton,"P1Up" ,"Up-Idle" ,"","Up-Hover","Up-Click",P1SnekStateUp };
	Events::EV_NEW_UI_ELEMENT P1DownElement = { canvasComponent,HTVector2{ 0.175f , 0.6125f } ,kCanvasButton,"P1Down" ,"Down-Idle", "" ,"Down-Hover","Down-Click",P1SnekStateDown };
	Events::EV_NEW_UI_ELEMENT P2UpElement = { canvasComponent,HTVector2{ 0.825f , 0.5f } ,kCanvasButton,"P2Up" ,"Up-Idle", "","Up-Hover","Up-Click",P2SnekStateUp };
	Events::EV_NEW_UI_ELEMENT P2DownElement = { canvasComponent, HTVector2{ 0.825f , 0.6125f }, kCanvasButton,"P2Down", "Down-Idle", "", "Down-Hover","Down-Click", P2SnekStateDown };
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