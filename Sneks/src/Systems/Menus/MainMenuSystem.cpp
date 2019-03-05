#include "MainMenuSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"


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

}

void PlayGame()
{
	
}

void MainMenuSystem::Initialize(CanvasComponent* canvasComponent)
{
	//CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	float screenX =0 , screenY =0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	//Events::EV_NEW_UI_ELEMENT LogoElement = { canvasComponent,HTVector2{ screenX / 16 , screenY / 16 } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" ,"","","", nullptr};
	Events::EV_NEW_UI_ELEMENT PlayElement = { canvasComponent,HTVector2{ screenX / 8 * 5.5f , screenY /8 * 4} ,kCanvasButton,"PlayButton" ,"UIBack" ,"","","",PlayGame };
	Events::EV_NEW_UI_ELEMENT CreditsElement = { canvasComponent,HTVector2{ screenX / 8 * 5.5f , screenY / 8 * 4.9f} ,kCanvasButton,"PlayButton" ,"UIBack" ,"","","",PlayGame };
	Events::EV_NEW_UI_ELEMENT QuitElement = { canvasComponent,HTVector2{ screenX / 8 * 5.5f , screenY / 8 * 5.8f} ,kCanvasButton,"PlayButton" ,"UIBack" ,"","","",PlayGame };
	//m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LogoElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(PlayElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CreditsElement);
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(QuitElement);
}

void MainMenuSystem::Update(float dt)
{
	(void)dt;
}

