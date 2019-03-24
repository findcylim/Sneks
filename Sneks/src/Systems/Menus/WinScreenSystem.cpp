#include "WinScreenSystem.h"
#include "../../Utility/GameStateManager.h"


void Restart()
{
	GameStateManager::SetState(kStateGame);
}

void QuitToMain()
{
	GameStateManager::SetState(kStateMainMenu);
}

WinScreenSystem::WinScreenSystem(EntityManager* entityManager, EventManager* eventManager)
	:BaseSystem{entityManager}
{
	m_po_EventManagerPtr = eventManager;
	m_po_EntityManager = entityManager;

	auto canvas = entityManager->NewEntity<CanvasEntity>(kEntityCanvas, "WinScreenEntity");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT WinScreenUIElement, LoseScreenUIElement;

	WinScreenUIElement = { canvas_Component, HTVector2{ 0.25f ,0.5f } ,kCanvasBasicSprite,"WinScreen" ,"WinSprite" ,"","","", nullptr };
	LoseScreenUIElement = { canvas_Component, HTVector2{ 0.75f ,0.5f } ,kCanvasBasicSprite,"LoseScreen" ,"LoseSprite" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT RestartUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.6f } ,kCanvasButton,"RestartButton" ,"UIBack" ,"Restart","UIBack_Hover","UIBack_Click", Restart };

	Events::EV_NEW_UI_ELEMENT ReturnToMainUIElement = 
	{ canvas_Component, HTVector2{ 0.5f ,0.7f } ,kCanvasButton,"ReturnToMainButton" ,"UIBack" ,"Return To Main Menu","UIBack_Hover","UIBack_Click", QuitToMain };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(WinScreenUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LoseScreenUIElement);
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



void WinScreenSystem::SwapWinScreen()
{
	CanvasElementComponent* TWin_Comp =
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "WinScreen")
		->GetComponent<CanvasElementComponent>();
	CanvasElementComponent* TLose_Comp =
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "LoseScreen")
		->GetComponent<CanvasElementComponent>();

	if (TWin_Comp && TLose_Comp)
	{
		HTVector2 tempPosition = { TWin_Comp->m_f_XOffset,TWin_Comp->m_f_YOffset };
		TWin_Comp->m_f_XOffset = TLose_Comp->m_f_XOffset;
		TWin_Comp->m_f_YOffset = TLose_Comp->m_f_YOffset;
		
		TLose_Comp->m_f_XOffset = tempPosition.x;
		TLose_Comp->m_f_YOffset = tempPosition.y;
	}
}