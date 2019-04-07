/* Start Header****************************************************************/
/*!
\file ECSystem.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY     - 35.53%  (Equal with the rest + Update)
\par Contribution : Javier - 21.32%  (Equal with the rest)
\par Contribution : Edwin  - 21.32%  (Equal with the rest)
\par Contribution : Adam   - 21.32%  (Equal with the rest)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include <queue>

#include "ECSystem.h"
#include "../Utility/GameStateManager.h"
#include "../Utility/AlphaEngineHelper.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/LevelLoaderSystem.h"
#include "../Systems/GraphicsSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/SnekSystem.h"
#include "../Systems/BackgroundSystem.h"
#include "../Systems/BuildingsSystem.h"
#include "../Systems/ProjectileSystem.h"
#include "../Systems/ParticleSystem.h"
#include "../Systems/AudioSystem.h"
#include "../Systems/Menus/MainMenuSystem.h"
#include "../Systems/Menus/HUDSystem.h"
#include "../Systems/Menus/WinScreenSystem.h"
#include "../Systems/Menus/HelpMenuSystem.h"
#include "../Systems/PowerUpSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/Menus/PauseMenuSystem.h"
#include "../Systems/TintSystem.h"
#include "../Systems/FollowSystem.h"
#include "../Systems/Menus/CreditsScreenSystem.h"
#include "../Systems/Menus/SplashScreenSystem.h"
#include "../Systems/Menus/SnekSelectMenuSystem.h"
#include "../Systems/Menus/TutorialMenuSystem.h"
#include "../Systems/Menus/OptionsMenuSystem.h"
#include "../Systems/Menus/ConfirmationScreenSystem.h"
#include "../Systems/Menus/EndRoundScreenSystem.h"

ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_po_EventManager			   = new EventManager();
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_po_EntityManager	   = new EntityManager();
	m_o_GameStateManager		   = new GameStateManager(kStateSplashScreen, m_o_SystemManager, m_po_EntityManager, m_po_EventManager,&m_b_EngineStatus);
	m_b_EngineStatus			   = true;
}


ECSystem::~ECSystem()
{
	delete(m_o_Logger);
	delete(m_o_SystemManager);
	delete(m_po_EventManager);
	delete(m_o_GameStateManager);
	delete(m_po_EntityManager);
	
}


void ECSystem::LoadLevel1()
{
	m_po_EntityManager->NewEntity<CameraEntity>(kEntityCamera, "Camera");
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>();

	snek->CreateSnek(-200, 0, PI * 3 / 4, 20, kSnekTypeSpeed, 0,3);
	snek->CreateSnek(200, 0, PI * 7 / 4, 20, kSnekTypeFlip, 1,3);
}

/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSystem::InitializeEngine()
{
	m_o_SystemManager->Initialize(m_po_EventManager, m_po_EntityManager);

	auto graphics = new GraphicsSystem();
	auto physics = new PhysicsSystem();
	auto camera = new CameraSystem(graphics);
	//auto levelLoader = new LevelLoaderSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager,graphics);
	auto snek = new SnekSystem(graphics);
	auto background = new BackgroundSystem(graphics);
	auto buildings = new BuildingsSystem(graphics);
	auto collisions = new CollisionSystem();
	auto projectile = new ProjectileSystem(graphics);
	auto particle = new ParticleSystem(graphics);
	auto audio = new AudioSystem(m_o_GameStateManager);
	auto powerUp = new PowerUpSystem(graphics);
	auto helpMenu = new HelpMenuSystem();
	auto canvas = new CanvasUISystem(graphics);
	auto anim = new AnimationSystem(graphics);
	auto input = new InputSystem();
	auto mainMenu = new MainMenuSystem();
	auto hud = new HUDSystem(graphics);
	auto pauseMenu = new PauseMenuSystem();
	auto splashScreen = new SplashScreenSystem(graphics);
	auto snekSelect = new SnekSelectMenuSystem();
	auto tutorial = new TutorialMenuSystem(m_o_GameStateManager);
	auto options = new OptionsMenuSystem();
	auto creditsScreen = new CreditsScreenSystem(graphics);
	auto confirmationScreen = new ConfirmationScreenSystem();

	m_o_SystemManager->AddSystem(projectile);
	projectile->SetName("Projectile");

	m_o_SystemManager->AddSystem(particle);
	particle->SetName("Particles");

	m_o_SystemManager->AddSystem(snek);
	snek->SetName("Snek");
	m_o_SystemManager->MakeSystem<FollowSystem>("Follow");

	m_o_SystemManager->AddSystem(physics);
	physics->SetName("Physics");

	m_o_SystemManager->AddSystem(canvas);
	canvas->SetName("Canvas UI");

	m_o_SystemManager->AddSystem(collisions);
	collisions->SetName("Collisions");

	m_o_SystemManager->AddSystem(graphics);
	graphics->SetName("Graphics");

	m_o_SystemManager->AddSystem(anim);
	anim->SetName("Animations");

	m_o_SystemManager->AddSystem(camera);
	camera->SetName("Camera");

	audio->SetName("Audio");
	m_o_SystemManager->AddSystem(audio);

	m_o_SystemManager->AddSystem(background);
	background->SetName("Background");
	background->CreateInstancedBackgrounds(2, 2, "Background01");

	m_o_SystemManager->AddSystem(buildings);
	buildings->SetName("Buildings");

	m_o_SystemManager->MakeSystem<TintSystem>("Tint");
	/*m_o_SystemManager->AddSystem(levelLoader);
	levelLoader->SetName("LevelLoader");*/
	//levelLoader->LoadLevel(kLevel1);

	m_o_SystemManager->AddSystem(input);
	input->SetName("Input");

	LoadLevel1();

	/*************************************************************************/
	// UI & MENUS
	/*************************************************************************/

	m_o_SystemManager->AddSystem(mainMenu);
	mainMenu->SetName("Main Menu"); 

	m_o_SystemManager->AddSystem(snekSelect);
	snekSelect->SetName("Snek Select");

	m_o_SystemManager->AddSystem(hud);
	hud->SetName("HUD");
	
	m_o_SystemManager->AddSystem(helpMenu);
	helpMenu->SetName("HelpMenu");

	m_o_SystemManager->AddSystem(powerUp);
	powerUp->SetName("Power Up");
	
	m_o_SystemManager->AddSystem(tutorial);
	tutorial->SetName("Tutorial");

	m_o_SystemManager->AddSystem(options);
	options->SetName("Options");

	m_o_SystemManager->AddSystem(pauseMenu);
	pauseMenu->SetName("PauseMenu");

	m_o_SystemManager->MakeSystem<EndRoundScreenSystem>("EndRound");
	m_o_SystemManager->MakeSystem<WinScreenSystem>("WinScreen");

	m_o_SystemManager->AddSystem(creditsScreen);
	creditsScreen->SetName("CreditsScreen");

	m_o_SystemManager->AddSystem(splashScreen);
	splashScreen->SetName("SplashScreen");

	m_o_SystemManager->AddSystem(confirmationScreen);
	confirmationScreen->SetName("ConfirmationScreen");

	m_o_GameStateManager->InitializeTransitionEntity(graphics);


	/*************************************************************************/
	//\\\\\\\\\\END UI & MENUS
	/*************************************************************************/


	/*************************************************************************/
	// Other Entity Creation
	/*************************************************************************/

	MouseEntity* mouseEntity = m_po_EntityManager->NewEntity<MouseEntity>(kEntityMouse, "MouseEntity");
	mouseEntity->GetComponent<CollisionComponent>()->m_i_CollisionGroupVec.push_back(kCollGroupMouse);
	graphics->InitializeDrawComponent(mouseEntity->GetComponent<DrawComponent>(), "MouseCollider");
	m_po_EntityManager->GetSpecificEntityInstance<MouseEntity>(kEntityMouse, "MouseEntity")
		->GetComponent<DrawComponent>()->m_f_RgbaColor.alpha = 0;

	auto countDownEntity = m_po_EntityManager->NewEntity<CanvasEntity>(kEntityCanvas, "CountDownEntity");
	auto canvas_Component = countDownEntity->GetComponent<CanvasComponent>();
	Events::EV_NEW_UI_ELEMENT countDown =
		{ canvas_Component, HTVector2{ 0.5f ,0.5f } ,kCanvasBasicSprite,"EndText" ,
			"Countdown" ,"","","", nullptr,
			{1,1,1,1},4,1 };

	m_po_EventManager->EmitEvent<Events::EV_NEW_UI_ELEMENT>(countDown);

	/*************************************************************************/
	//\\\\\\\\\\END Other Entity Creation
	/*************************************************************************/

	//m_o_EntityComponentManager->DisableSpecificEntityType<CanvasTextLabelEntity , kEntityCanvasTextLabel>("Main Menu UI");

	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("CountDownEntity");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Main Menu UI");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Snek Select UI");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("EndRoundEntity");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
	m_po_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("OptionsMenuEntity");

	m_o_SystemManager->DisableSystem<HUDSystem>();
	m_o_SystemManager->DisableSystem<WinScreenSystem>();
	m_o_SystemManager->DisableSystem<HelpMenuSystem>();
	m_o_SystemManager->DisableSystem<PauseMenuSystem>();
	m_o_SystemManager->DisableSystem<CreditsScreenSystem>();
	m_o_SystemManager->DisableSystem<ConfirmationScreenSystem>();
	m_o_SystemManager->DisableSystem<EndRoundScreenSystem>();
	//m_o_SystemManager->DisableSystem<HUDSystem, CameraComponent, kComponentCamera>();
	//m_o_SystemManager->DisableSystem<HUDSystem, CanvasElementComponent, kComponentCanvasElement>();
	m_o_SystemManager->DisableSystem<PhysicsSystem>();
	m_o_SystemManager->DisableSystem<PowerUpSystem>();
	m_o_SystemManager->DisableSystem<BackgroundSystem>();
	m_o_SystemManager->DisableSystem<BuildingsSystem>();
	m_o_SystemManager->DisableSystem<LevelLoaderSystem>();
	m_o_SystemManager->DisableSystem<SnekSystem>();
	m_o_SystemManager->DisableSystem<ProjectileSystem>();
	m_o_SystemManager->DisableSystem<ParticleSystem>();
}

bool ECSystem::IsEngineOn() const
{
	return m_b_EngineStatus;
}

float actualDt = 0;
float cappedDt = 0;
float m_f_TimeScale = 1.0f;

void ECSystem::Update()
{
	constexpr float dtCap = 1.0f / 45.0f;

	if (actualDt <= 0)
		actualDt = static_cast<float>(AEFrameRateControllerGetFrameTime());

	do {

		AESysFrameStart();
		AEInputUpdate();

		cappedDt = min(actualDt, dtCap);

		if (actualDt > dtCap)
			++m_o_SystemManager->m_i_DroppedFrames;

		m_o_GameStateManager->Update(cappedDt);
		m_po_EventManager->Update();
		m_o_SystemManager->Update(cappedDt);

		//DEBUG
		//DISABLE TODO
		/*if (GetAsyncKeyState(AEVK_ESCAPE))
		{
			m_b_EngineStatus = false;
		}*/
		/*if (AEInputCheckTriggered(AEVK_COMMA))
		{
			AEToogleFullScreen(false);
		}
		if (AEInputCheckTriggered(AEVK_PERIOD))
		{
			AEToogleFullScreen(true);
		}*/
		m_po_EntityManager->ResolveDeletes();

		actualDt -= dtCap;
		
		AESysFrameEnd();

	} while (actualDt > 0.0f && m_b_EngineStatus);
}

float GetUncappedDt()
{
	return actualDt;
}

float GetCappedDt()
{
	return cappedDt;
}

float SetTimeScale(float timeScale)
{
	m_f_TimeScale = timeScale;
	return m_f_TimeScale;
}

float GetTimeScale()
{
	return m_f_TimeScale;
}