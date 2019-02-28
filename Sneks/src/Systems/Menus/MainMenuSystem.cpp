#include "MainMenuSystem.h"



MainMenuSystem::MainMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_o_EventManagerPtr = eventManager;
	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_f_VirtualOffsetX = -AEGfxGetWinMaxX();
	cameraComponent->m_f_VirtualOffsetY = AEGfxGetWinMaxY();
	cameraComponent->m_f_VirtualScale = 1.1f;
}


MainMenuSystem::~MainMenuSystem()
{
}

void MainMenuSystem::Initialize(CanvasComponent* canvasComponent)
{
	Events::EV_NEW_UI_ELEMENT newElement = { canvasComponent,HTVector2{ 483 + AEGfxGetWinMaxX()/8 , -(136 + AEGfxGetWinMaxY() / 8) } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" };
	m_o_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(newElement);
}
void MainMenuSystem::Update(float dt)
{
	(void)dt;
}