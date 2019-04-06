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

void SplashScreenSystem::Initialize()
{
	auto canvas = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "SplashScreenGUI");
	auto canvas_Component = canvas->GetComponent<CanvasComponent>();

	Events::EV_NEW_UI_ELEMENT BackgroundScreenElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"SplashScreenBackground" ,
		"Background01" ,"","","", nullptr };

	Events::EV_NEW_UI_ELEMENT SplashScreenElement =
	{ canvas_Component, HTVector2{ 0.5f , 0.5f } ,kCanvasBasicSprite,"SplashScreen" ,
		"DigipenLogo" ,"","","", nullptr };

	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(BackgroundScreenElement);
	m_po_EventManagerPtr->EmitEvent<Events::EV_NEW_UI_ELEMENT>(SplashScreenElement);

	m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreenBackground")
		->GetComponent<DrawComponent>()->m_f_RgbaColor = HTColor{ 0,0,0,1 };

	m_po_EventManagerPtr->AddListener<Events::EV_MOUSE_ONCLICK>(this, this);
}

void SplashScreenSystem::Update(float dt)
{
	timer -= dt;
	if(timer < 0)
	{
		timer = 2.0f;
		splashScreenCounter++;
		if (splashScreenCounter > 2)
		{
			GameStateManager::SetState(kStateMainMenu);
			return;
		}
		auto d_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreen")
			->GetComponent<DrawComponent>();
		if (d_Comp)
		{
			if (splashScreenCounter == 1)
			{
				d_Comp->m_po_TransformComponent->m_f_Scale = { 1,1 };
				m_po_GraphicsSystem->InitializeDrawComponent(d_Comp, "TeamLogo");
				//d_Comp->m_px_Texture = m_po_GraphicsSystem->FetchTexture("TeamLogo");
			}
			else if (splashScreenCounter == 2)
			{
				d_Comp->m_po_TransformComponent->m_f_Scale = { 1,1 };
				m_po_GraphicsSystem->InitializeDrawComponent(d_Comp, "FMODLogoSplash");
			}
		}
	}
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

void SplashScreenSystem::Receive(const Events::EV_MOUSE_ONCLICK & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	timer = -1;
	splashScreenCounter = 3;
}

SplashScreenSystem::SplashScreenSystem(GraphicsSystem* graphicsSystem)
{
	m_po_GraphicsSystem = graphicsSystem;
}


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
