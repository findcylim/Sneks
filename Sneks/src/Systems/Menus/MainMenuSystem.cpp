#include "MainMenuSystem.h"
#include "../../Math/HTVector2.h"
#include "../../Utility/AlphaEngineHelper.h"
#include "../../Utility/GameStateManager.h"
#include "../SnekSystem.h"



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

void SnekSelect(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateCharacterSelection);
}

void OpenCredits(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateCreditsScreen);
}

void QuitGame(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateConfirmationScreen);
}

void OpenOptions(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(kStateOptions);
}

void MainMenuSystem::Initialize()
{
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "Main Menu UI");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();


	//CameraComponent * c_Comp = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	float screenX = 0, screenY = 0;
	AlphaEngineHelper::GetScreenSize(&screenX, &screenY);
	Events::EV_NEW_UI_ELEMENT LogoElement = 
	{ canvasComponent,HTVector2{ 0.3f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT PlayElement = 
	{ canvasComponent,HTVector2{ 0.825f , 0.3875f } ,kCanvasButton,"PlayButton" ,"UIBack" ,"Play","UIBack_Hover","UIBack_Click",SnekSelect };

	Events::EV_NEW_UI_ELEMENT OptionsElement = 
	{ canvasComponent,HTVector2{ 0.825f , 0.5f } ,kCanvasButton,"OptionsButton" ,"UIBack" ,"Options","UIBack_Hover","UIBack_Click",OpenOptions };

	Events::EV_NEW_UI_ELEMENT CreditsElement =
	{ canvasComponent,HTVector2{ 0.825f , 0.6125f } ,kCanvasButton,"OptionsButton" ,"UIBack" ,"Credits","UIBack_Hover","UIBack_Click",OpenCredits };

	Events::EV_NEW_UI_ELEMENT QuitElement = 
	{ canvasComponent, HTVector2{ 0.825f , 0.725f }, kCanvasButton, "QuitButton", "UIBack", "Quit","UIBack_Hover","UIBack_Click", QuitGame };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement = 
	{ canvasComponent, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LogoElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(PlayElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(OptionsElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(CreditsElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(QuitElement);
}


bool zoomIn = true;
float zoomPause = 0.2f;
float zoomSpeed = 0.05f;
void MainMenuSystem::Update(float dt)
{
	
	//auto snekSystem = m_po_SystemManager->GetSystem<SnekSystem>("Snek");

	auto cameraComponent = m_po_ComponentManager->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_x_CameraAttributes.speedDecay = 0.99f;
	cameraComponent->m_b_TrackObjects = false;

	if (zoomPause > 0.0f)
	{
		if (fabsf(cameraComponent->m_f_ZoomVelocity) <= 0.02f) {
			zoomPause -= dt;
		}
	}
	else {
		if (zoomIn) {
			cameraComponent->m_f_ZoomVelocity += zoomSpeed * dt;
			if (cameraComponent->m_f_ZoomVelocity >= 0.05f)
				cameraComponent->m_f_ZoomVelocity = 0.05f;
			if (cameraComponent->m_f_VirtualScale >= 1.5f)
			{
				zoomPause = 1.5f;
				zoomIn = !zoomIn;
			}
		}
		else
		{
			cameraComponent->m_f_ZoomVelocity -= zoomSpeed * dt;
			if (cameraComponent->m_f_ZoomVelocity <= -0.5f)
				cameraComponent->m_f_ZoomVelocity = -0.5f;
			if (cameraComponent->m_f_VirtualScale <= 1.0f) 
			{
				zoomPause = 1.5f;
				zoomIn = !zoomIn;
			}
		}
	}
}

