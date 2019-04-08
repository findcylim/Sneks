/* Start Header****************************************************************/
/*!
\file OptionsMenuSystem.cpp
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "OptionsMenuSystem.h"
#include "../AudioSystem.h"
#include "AESystem.h"

/* Constructor */
OptionsMenuSystem::OptionsMenuSystem()
{
}

/* Destructor */
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

/* Fullscreen/Windowed Mode */
void ToggleFullScreen(SystemManager* systemManager)
{
	auto options = systemManager->GetSystem<OptionsMenuSystem>("Options");

	options->ToggleFullscreen();

	if (options->ReturnFullscreen())
		AEToogleFullScreen(true); // set to fullscreen if windowed
	else
		AEToogleFullScreen(false); // set to windowed if fullscreen
}

/* Mute/Unmute Audio */
void ToggleMute(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	systemManager->GetSystem<AudioSystem>("Audio")->ToggleMute();
}

/* Return to Pause/Main Menu */
void BackToMenu(SystemManager* systemManager)
{
	UNREFERENCED_PARAMETER(systemManager);
	GameStateManager::SetState(GameStateManager::ReturnPreviousState());
}


void OptionsMenuSystem::Initialize()
{
	auto canvasEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "OptionsMenuEntity");
	auto canvasComponent = canvasEntity->GetComponent<CanvasComponent>();

	/* Game Logo */
	Events::EV_NEW_UI_ELEMENT LogoElement =
	{ canvasComponent,HTVector2{ 0.3f ,0.2f } ,kCanvasBasicSprite,"Logo" ,"MainMenuLogo" ,"","","", nullptr };

	/* Buttons */
	Events::EV_NEW_UI_ELEMENT FullscreenElement =
	{ canvasComponent,HTVector2{ 0.5f ,  0.5f } ,kCanvasButton,"FullscreenButton" ,"UIBack" ,"Toggle Fullscreen","UIBack_Hover","UIBack_Click", ToggleFullScreen};

	Events::EV_NEW_UI_ELEMENT MuteElement =
	{ canvasComponent,HTVector2{ 0.5f , 0.6125f } ,kCanvasButton,"MuteButton" ,"UIBack" ,"Mute/Unmute","UIBack_Hover","UIBack_Click",ToggleMute };

	Events::EV_NEW_UI_ELEMENT BackToMenuElement =
	{ canvasComponent, HTVector2{ 0.5f , 0.725f }, kCanvasButton, "QuitButton", "UIBack", "Back","UIBack_Hover","UIBack_Click", BackToMenu };

	/* Dim Background */
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

/* Fullscreen Toggle Helper Functions */
bool OptionsMenuSystem::ReturnFullscreen()
{
	return fullscreen;
}

void OptionsMenuSystem::ToggleFullscreen()
{
	fullscreen = !fullscreen;
}