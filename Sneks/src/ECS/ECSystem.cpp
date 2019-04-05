#include "ECSystem.h"
#include "../Utility/GameStateManager.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/LevelLoaderSystem.h"
#include "../Systems/GraphicsSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Utility/AlphaEngineHelper.h"
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
#include <queue>
#include "../Systems/AnimationSystem.h"
#include "../Systems/Menus/PauseMenuSystem.h"
#include "../Systems/Menus/CreditsScreenSystem.h"
#include "../Systems/Menus/SplashScreenSystem.h"
#include "../Systems/Menus/SnekSelectMenuSystem.h"
#include "../Systems/Menus/TutorialMenuSystem.h"
#include "../Systems/Menus/OptionsMenuSystem.h"

ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_o_EventManager			   = new EventManager();
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_o_EntityComponentManager	   = new EntityManager();
	m_o_GameStateManager           = new GameStateManager(kStateSplashScreen, m_o_EntityComponentManager, m_o_SystemManager, m_o_EventManager,&m_b_EngineStatus);
	m_b_EngineStatus			   = true;
}


ECSystem::~ECSystem()
{
	delete(m_o_Logger);
	delete(m_o_SystemManager);
	delete(m_o_EventManager);
	delete(m_o_GameStateManager);
	delete(m_o_EntityComponentManager);
	
}

/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/

void ECSystem::InitializeEngine()
{
	//TODO change this state to splash screen/main menu in the future
	
	
	//m_o_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	m_o_Logger->LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_o_SystemManager->Initialize(m_o_EventManager, m_o_EntityComponentManager);
	/*
		Create and add Systems here
	*/

	m_o_EntityComponentManager->NewEntity<CameraEntity>(kEntityCamera, "Camera");
	

	auto graphics = new GraphicsSystem(m_o_EntityComponentManager);
	auto physics = new PhysicsSystem(m_o_EntityComponentManager);
	auto camera = new CameraSystem(m_o_EntityComponentManager, graphics);
	auto levelLoader = new LevelLoaderSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager,graphics);
	auto snek = new SnekSystem(m_o_EntityComponentManager, graphics, m_o_GameStateManager);
	auto background = new BackgroundSystem(m_o_EntityComponentManager, graphics);
	auto buildings = new BuildingsSystem(m_o_EntityComponentManager, graphics);
	auto collisions = new CollisionSystem(m_o_EntityComponentManager);
	auto projectile = new ProjectileSystem(m_o_EntityComponentManager, graphics);
	auto particle = new ParticleSystem(m_o_EntityComponentManager, graphics);
	auto audio = new AudioSystem(m_o_EntityComponentManager, m_o_GameStateManager);
	auto powerup = new PowerUpSystem(m_o_EntityComponentManager, graphics, snek);
	auto helpmenu = new HelpMenuSystem(m_o_EntityComponentManager,m_o_EventManager);
	auto canvas = new CanvasUISystem(m_o_EntityComponentManager, graphics, m_o_EventManager);
	auto anim = new AnimationSystem(m_o_EntityComponentManager, graphics);
	auto splashScreen = new SplashScreenSystem(m_o_EntityComponentManager, m_o_EventManager,graphics);
	auto snekSelect = new SnekSelectMenuSystem(m_o_EntityComponentManager, m_o_EventManager);
	auto tutorial = new TutorialMenuSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager);
	auto options = new OptionsMenuSystem(m_o_EntityComponentManager, m_o_EventManager);

	m_o_SystemManager->AddSystem(projectile);
	projectile->SetName("Projectile");
	projectile->Initialize();

	m_o_SystemManager->AddSystem(particle);
	particle->SetName("Particles");
	particle->Initialize();

	m_o_SystemManager->AddSystem(snek);
	snek->SetName("Snek");
	snek->Initialize();

	
	m_o_SystemManager->AddSystem(physics);
	physics->SetName("Physics");
	physics->Initialize(m_o_GameStateManager);


	m_o_SystemManager->AddSystem(canvas);
	canvas->SetName("Canvas UI");
	canvas->Initialize();

	m_o_SystemManager->AddSystem(collisions);
	collisions->Initialize();
	collisions->SetName("Collisions");
	m_b_EngineStatus = true;


	m_o_SystemManager->AddSystem(graphics);
	graphics->SetName("Graphics");
	graphics->Initialize();
	graphics->PreLoadTextures();

	m_o_SystemManager->AddSystem(anim);
	anim->SetName("Animations");
	anim->Initialize();

	m_o_SystemManager->AddSystem(camera);
	camera->SetName("Camera");
	camera->Initialize();

	audio->SetName("Audio");
	m_o_SystemManager->AddSystem(audio);
	audio->Initialize();

	snek->CreateSnek(-200, 0, PI* 3 / 4, 20, "HeadAnim", 0);
	snek->CreateSnek(200, 0, PI* 7 / 4, 20, "SnekHead02", 1);

	auto snekHead1 = snek->m_po_ComponentManager->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	auto snekHead2 = static_cast<SnekHeadComponent*>(snekHead1->m_po_NextComponent);

	snekHead1->accelerationTutorial = true;
	snekHead2->accelerationTutorial = true;
	snekHead1->turningTutorial = true;
	snekHead2->turningTutorial = true;
	snekHead1->specialTutorial = true;
	snekHead2->specialTutorial = true;

	m_o_SystemManager->AddSystem(background);
	background->SetName("Background");
	background->CreateInstancedBackgrounds(2, 2, "Background01");

	m_o_SystemManager->AddSystem(buildings);
	buildings->SetName("Buildings");
	buildings->Initialize();

	m_o_SystemManager->AddSystem(levelLoader);
	levelLoader->SetName("LevelLoader");
	//levelLoader->LoadLevel(kLevel1);

	auto input = new InputSystem(m_o_EntityComponentManager, m_o_EventManager, 5, "Input System", m_o_GameStateManager, m_o_Logger);
	m_o_SystemManager->AddSystem(input);
	input->SetName("Input");

	/*************************************************************************/
	// UI & MENUS
	/*************************************************************************/

	

	CanvasEntity* mainMenuCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Main Menu UI");

	auto mainMenu = new MainMenuSystem(m_o_EntityComponentManager, m_o_EventManager);
	mainMenu->Initialize(mainMenuCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(mainMenu);
	mainMenu->SetName("Main Menu"); 

	CanvasEntity* SelectCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Snek Select UI");

	snekSelect->Initialize(SelectCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(snekSelect);
	snekSelect->SetName("Snek Select");

	CanvasEntity* HUDCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Heads Up Display");

	auto hud = new HUDSystem(m_o_EntityComponentManager, m_o_EventManager, graphics);
	hud->SetName("HUD");
	hud->Initialize(HUDCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(hud);

	helpmenu->Initialize();
	helpmenu->SetName("HelpMenu");
	m_o_SystemManager->AddSystem(helpmenu);

	CanvasEntity* TutorialCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Tutorial UI");

	tutorial->Initialize(TutorialCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(tutorial);
	tutorial->SetName("Tutorial");

	CanvasEntity* OptionsCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "OptionsMenuEntity");

	options->Initialize(OptionsCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(options);
	options->SetName("Options");

	m_o_SystemManager->AddSystem(powerup);
	powerup->SetName("Power Up");
	powerup->Initialize();

	auto PauseMenu = new PauseMenuSystem(m_o_EntityComponentManager, m_o_EventManager);
	PauseMenu->SetName("PauseMenu");
	m_o_SystemManager->AddSystem(PauseMenu);
	PauseMenu->Initialize();

	auto WinScreen = new WinScreenSystem(m_o_EntityComponentManager, m_o_EventManager);
	WinScreen->SetName("WinScreen");
	m_o_SystemManager->AddSystem(WinScreen);

	auto CreditsScreen = new CreditsScreenSystem(m_o_EntityComponentManager, m_o_EventManager,graphics);
	CreditsScreen->SetName("CreditsScreen");
	CreditsScreen->Initialize();
	m_o_SystemManager->AddSystem(CreditsScreen);

	splashScreen->SetName("SplashScreen");
	splashScreen->Initialize();
	m_o_SystemManager->AddSystem(splashScreen);

	m_o_GameStateManager->AddGraphics(graphics);

	/*************************************************************************/
	//\\\\\\\\\\END UI & MENUS
	/*************************************************************************/


	/*************************************************************************/
	// Other Entity Creation
	/*************************************************************************/

	MouseEntity* mouseEntity = m_o_EntityComponentManager->NewEntity<MouseEntity>(kEntityMouse, "MouseEntity");
	mouseEntity->GetComponent<CollisionComponent>()->m_i_CollisionGroupVec.push_back(kCollGroupMouse);
	graphics->InitializeDrawComponent(mouseEntity->GetComponent<DrawComponent>(), "MouseCollider");
	m_o_EntityComponentManager->GetSpecificEntityInstance<MouseEntity>(kEntityMouse, "MouseEntity")
		->GetComponent<DrawComponent>()->m_f_RgbaColor.alpha = 0;



	/*************************************************************************/
	//\\\\\\\\\\END Other Entity Creation
	/*************************************************************************/



	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Main Menu UI");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Snek Select UI");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("OptionsMenuEntity");

	m_o_SystemManager->DisableSystem<HUDSystem>();
	m_o_SystemManager->DisableSystem<WinScreenSystem>();
	m_o_SystemManager->DisableSystem<HelpMenuSystem>();
	m_o_SystemManager->DisableSystem<PauseMenuSystem>();
	m_o_SystemManager->DisableSystem<CreditsScreenSystem>();
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
float timeElapsed = 0;

void ECSystem::Update()
{
	constexpr float dtCap = 1.0f / 45.0f;

	if (actualDt <= 0)
		actualDt = static_cast<float>(AEFrameRateControllerGetFrameTime());

	do {

		AESysFrameStart();
		AEInputUpdate();

		auto cappedDt = min(actualDt, dtCap);

		if (actualDt > dtCap)
			++m_o_SystemManager->m_i_DroppedFrames;

		m_o_GameStateManager->Update(cappedDt);
		m_o_EventManager->Update();
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
		m_o_EntityComponentManager->ResolveDeletes();

		actualDt -= dtCap;
		
		AESysFrameEnd();

	} while (actualDt > 0.0f && m_b_EngineStatus);
}

float getDt()
{
	return actualDt;
}