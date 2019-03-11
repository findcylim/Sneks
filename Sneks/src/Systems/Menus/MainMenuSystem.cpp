#include "MainMenuSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../../Utility/GameStateManager.h"


MainMenuSystem::MainMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_o_EventManagerPtr = eventManager;
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_f_VirtualOffsetX = -AEGfxGetWinMaxX();
	cameraComponent->m_f_VirtualOffsetY = AEGfxGetWinMaxY();
	cameraComponent->m_f_VirtualScale = 1.0f;
}

MainMenuSystem::~MainMenuSystem()
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

void PlayGame()
{
	// Change game state
	// TODO
	GameStateManager::SetState(kStateGame);
}

void QuitGame()
{
	GameStateManager::SetState(kStateExit);
}
void MainMenuSystem::Initialize(CanvasComponent* canvasComponent)
{
	//CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	Events::EV_NEW_UI_ELEMENT LogoElement = { canvasComponent,HTVector2{ 0.3f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" ,"","","", nullptr };
	Events::EV_NEW_UI_ELEMENT PlayElement = { canvasComponent,HTVector2{ 0.825f , 0.5f } ,kCanvasButton,"PlayButton" ,"UIBack" ,"Play","UIBack_Hover","UIBack_Click",PlayGame };
	Events::EV_NEW_UI_ELEMENT CreditsElement = { canvasComponent,HTVector2{ 0.825f , 0.6125f } ,kCanvasButton,"CreditsButton" ,"UIBack" ,"Credits","UIBack_Hover","UIBack_Click",PlayGame };
	Events::EV_NEW_UI_ELEMENT QuitElement = { canvasComponent, HTVector2{ 0.825f , 0.725f }, kCanvasButton, "QuitButton", "UIBack", "Quit","UIBack_Hover","UIBack_Click", QuitGame };
	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement = { canvasComponent, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LogoElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(PlayElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CreditsElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(QuitElement);
}

void MainMenuSystem::Update(float dt)
{
	(void)dt;
}

