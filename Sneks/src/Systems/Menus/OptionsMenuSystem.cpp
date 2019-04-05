#include "OptionsMenuSystem.h"
#include "../AudioSystem.h"
#include "AESystem.h"

OptionsMenuSystem::OptionsMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager)
	:BaseSystem(entityManagerPtr)
{
	m_po_EventManagerPtr = eventManager;
}

OptionsMenuSystem::~OptionsMenuSystem()
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

bool OptionsMenuSystem::ReturnFullscreen()
{
	return fullscreen;
}

void OptionsMenuSystem::ToggleFullscreen()
{
	fullscreen = !fullscreen;
}

void ToggleFullScreen(SystemManager* systemManager)
{
	auto options = systemManager->GetSystem<OptionsMenuSystem>("Options");

	options->ToggleFullscreen();

	if (options->ReturnFullscreen())
		AEToogleFullScreen(true);
	else
		AEToogleFullScreen(false);
}

void ToggleMute(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	systemManager->GetSystem<AudioSystem>("Audio")->ToggleMute();
}

void BackToMenu(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(GameStateManager::ReturnPreviousState());
}


void OptionsMenuSystem::Initialize(CanvasComponent* canvasComponent)
{
	Events::EV_NEW_UI_ELEMENT LogoElement =
	{ canvasComponent,HTVector2{ 0.3f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT FullscreenElement =
	{ canvasComponent,HTVector2{ 0.5f ,  0.5f } ,kCanvasButton,"FullscreenButton" ,"UIBack" ,"Toggle Fullscreen","UIBack_Hover","UIBack_Click", ToggleFullScreen};

	Events::EV_NEW_UI_ELEMENT MuteElement =
	{ canvasComponent,HTVector2{ 0.5f , 0.6125f } ,kCanvasButton,"MuteButton" ,"UIBack" ,"Mute/Unmute","UIBack_Hover","UIBack_Click",ToggleMute };

	Events::EV_NEW_UI_ELEMENT BackToMenuElement =
	{ canvasComponent, HTVector2{ 0.5f , 0.725f }, kCanvasButton, "QuitButton", "UIBack", "Back","UIBack_Hover","UIBack_Click", BackToMenu };

	Events::EV_NEW_UI_ELEMENT TransitonBackUIElement =
	{ canvasComponent, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"Background" ,"TransitionBack" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(TransitonBackUIElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(LogoElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(FullscreenElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(MuteElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(BackToMenuElement);
}

void OptionsMenuSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}