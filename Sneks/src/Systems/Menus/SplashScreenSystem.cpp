
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
		timer = 1.5f;
		splashScreenCounter++;
		if (splashScreenCounter > 1)
		{
			GameStateManager::SetState(kStateMainMenu);
			return;
		}
		auto d_Comp = m_po_EntityManager->GetSpecificEntityInstance<CanvasBasicSpriteEntity>(kEntityCanvasBasicSprite, "SplashScreen")
			->GetComponent<DrawComponent>();
		if (d_Comp)
		{
			d_Comp->m_po_TransformComponent->m_f_Scale = { 1,1 };
			m_po_GraphicsSystem->InitializeDrawComponent(d_Comp, "TeamLogo");
			//d_Comp->m_px_Texture = m_po_GraphicsSystem->FetchTexture("TeamLogo");

		}
	}
}

void SplashScreenSystem::Receive(const Events::EV_MOUSE_ONCLICK & eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	timer = -1;
}

SplashScreenSystem::SplashScreenSystem(EntityManager* entityManagerPtr,EventManager* eventManager,
									   GraphicsSystem* graphicsSystem)
	: BaseSystem{entityManagerPtr}
{
	m_po_EventManagerPtr = eventManager;
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
