/* Start Header****************************************************************/
/*!
\file SplashScreenSystem.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for the intro splash screen images

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "SplashScreenSystem.h"
#include "../GraphicsSystem.h"
#include "../../Utility/GameStateManager.h"

/*
	Initialize function
*/
void SplashScreenSystem::Initialize()
{
	//	Creates the canvas object
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "SplashScreenGUI");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();

	Events::EV_NEW_UI_ELEMENT BackgroundScreenElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"SplashScreenBackground" ,
		"Background01" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT SplashScreenElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"SplashScreen" ,
		"DigipenLogo" ,"","","", nullptr };

	//	Creates the elements
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(BackgroundScreenElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(SplashScreenElement);

	//	Sets the background to black
	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreenBackground")
		->GetComponent<DrawComponent>()->m_f_RgbaColor = HTColor{ 0,0,0,1 };

	//	Adds the event listener
	m_po_EventManagerPtr->AddListener<Events::EV_MOUSE_ONCLICK>(this, this);
}

/*
	Update function
*/
void SplashScreenSystem::Update(float dt)
{
	timer -= dt;
	// After 2 seconds it will change the image
	if(timer < 0)
	{
		timer = 2.0f;
		splashScreenCounter++;
		// After all the images it will transition to the main menu
		if (splashScreenCounter > 2)
		{
			GameStateManager::SetState(kStateMainMenu);
			return;
		}
		auto d_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreen")
			->GetComponent<DrawComponent>();
		if (d_Comp)
		{
			//	Changes the images
			if (splashScreenCounter == 1)
			{
				d_Comp->m_po_TransformComponent->m_f_Scale = { 1,1 };
				m_po_GraphicsSystem->InitializeDrawComponent(d_Comp, "TeamLogo");
			}
			else if (splashScreenCounter == 2)
			{
				d_Comp->m_po_TransformComponent->m_f_Scale = { 1,1 };
				m_po_GraphicsSystem->InitializeDrawComponent(d_Comp, "FMODLogoSplash");
			}
		}
	}
	// IF ANY OF THESE BUTTONS ARE PRESSED IT WILL BE PASSED
	if (GetAsyncKeyState(VK_RBUTTON) < 0)
	{
		timer = -1;
		splashScreenCounter = 3;
	}
	else if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		timer = -1;
		splashScreenCounter = 3;
	}
	else if (GetAsyncKeyState(VK_RETURN) < 0)
	{
		timer = -1;
		splashScreenCounter = 3;
	}
	else if (GetAsyncKeyState(VK_ESCAPE) < 0)
	{
		timer = -1;
		splashScreenCounter = 3;
	}
}

/*
	Proper way to read mouse input events
*/
void SplashScreenSystem::Receive(const Events::EV_MOUSE_ONCLICK & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	timer = -1;
	splashScreenCounter = 3;
}

/*
	Constructor
*/
SplashScreenSystem::SplashScreenSystem(GraphicsSystem* graphicsSystem)
{
	m_po_GraphicsSystem = graphicsSystem;
}

/*
	Removes the listeners and deletes the elements and canvas	
*/
SplashScreenSystem::~SplashScreenSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_MOUSE_ONCLICK>(this);
	m_po_EntityManager->AddToDeleteQueue(
		m_po_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "SplashScreenGUI"));
	m_po_EntityManager->AddToDeleteQueue(
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreenBackground"));
	m_po_EntityManager->AddToDeleteQueue(
		m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreen"));
}
