#include "MainMenuSystem.h"



MainMenuSystem::MainMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_o_EventManagerPtr = eventManager;
}


MainMenuSystem::~MainMenuSystem()
{
}

void MainMenuSystem::Initialize(CanvasComponent* canvasComponent)
{
	auto cameraTransform = m_po_EntityManager->GetFirstEntityInstance<CameraEntity>(kEntityCamera)->GetComponent<TransformComponent>();
	cameraTransform->SetPosition(AEGfxGetWinMaxX() , AEGfxGetWinMaxY() );
	Events::EV_NEW_UI_ELEMENT newElement = { canvasComponent,HTVector2{ AEGfxGetWinMaxX() / 8, AEGfxGetWinMaxY() / 8 } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" };
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(newElement);
}
void MainMenuSystem::Update(float dt)
{
	(void)dt;
}