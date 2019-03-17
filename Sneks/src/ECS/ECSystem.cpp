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
#include "../Systems/Menus/PauseMenuSystem.h"
#include "../Systems/Menus/WinScreenSystem.h"
#include "../Systems/Menus/HelpMenuSystem.h"
#include "../Systems/PowerUpSystem.h"
#include <queue>
#include "../Systems/AnimationSystem.h"


ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_o_EventManager			   = new EventManager();
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_o_EntityComponentManager	   = new EntityManager();
	m_o_GameStateManager           = new GameStateManager(kStateMainMenu, m_o_EntityComponentManager, m_o_SystemManager, m_o_EventManager,&m_b_EngineStatus);
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
	auto camera = new CameraSystem(m_o_EntityComponentManager);
	auto levelLoader = new LevelLoaderSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager,graphics);
	auto snek = new SnekSystem(m_o_EntityComponentManager, graphics, m_o_GameStateManager);
	auto background = new BackgroundSystem(m_o_EntityComponentManager, graphics);
	auto buildings = new BuildingsSystem(m_o_EntityComponentManager, graphics);
	auto collisions = new CollisionSystem(m_o_EntityComponentManager);
	auto projectile = new ProjectileSystem(m_o_EntityComponentManager, graphics);
	auto particle = new ParticleSystem(m_o_EntityComponentManager, graphics);
	auto audio = new AudioSystem(m_o_EntityComponentManager);
	auto powerup = new PowerUpSystem(m_o_EntityComponentManager, graphics, snek);
	auto helpmenu = new HelpMenuSystem(m_o_EntityComponentManager,m_o_EventManager);
	auto canvas = new CanvasUISystem(m_o_EntityComponentManager, graphics, m_o_EventManager);
	auto anim = new AnimationSystem(m_o_EntityComponentManager, graphics);
	

	

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

	snek->CreateSnek(-200, 0, PI, 20, "HeadAnim", 0);
	snek->CreateSnek(200, 0, 0, 20, "SnekHead02", 1);


	m_o_SystemManager->AddSystem(background);
	background->SetName("Background");
	background->CreateInstancedBackgrounds(3, 3, "Background01");

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

	CanvasEntity* HUDCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Heads Up Display");

	auto hud = new HUDSystem(m_o_EntityComponentManager, m_o_EventManager);
	hud->SetName("HUD");
	hud->Initialize(HUDCanvas->GetComponent<CanvasComponent>());
	m_o_SystemManager->AddSystem(hud);

	helpmenu->Initialize();
	helpmenu->SetName("HelpMenu");
	m_o_SystemManager->AddSystem(helpmenu);

	m_o_SystemManager->AddSystem(powerup);
	powerup->SetName("Power Up");
	powerup->Initialize();

	auto PauseMenu = new PauseMenuSystem(m_o_EntityComponentManager, m_o_EventManager);
	PauseMenu->SetName("PauseMenu");
	m_o_SystemManager->AddSystem(PauseMenu);

	auto WinScreen = new WinScreenSystem(m_o_EntityComponentManager, m_o_EventManager, static_cast<char>(2));
	WinScreen->SetName("WinScreen");
	m_o_SystemManager->AddSystem(WinScreen);

	/*************************************************************************/
	//\\\\\\\\\\END UI & MENUS
	/*************************************************************************/


	/*************************************************************************/
	// Other Entity Creation
	/*************************************************************************/

	MouseEntity* mouseEntity = m_o_EntityComponentManager->NewEntity<MouseEntity>(kEntityMouse, "MouseEntity");
	mouseEntity->GetComponent<CollisionComponent>()->m_i_CollisionGroupVec.push_back(kCollGroupMouse);
	graphics->InitializeDrawComponent(mouseEntity->GetComponent<DrawComponent>(), "MouseCollider");




	/*************************************************************************/
	//\\\\\\\\\\END Other Entity Creation
	/*************************************************************************/



	//m_o_EntityComponentManager->DisableSpecificEntityType<CanvasTextLabelEntity , kEntityCanvasTextLabel>("Main Menu UI");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	m_o_EntityComponentManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");

	m_o_SystemManager->DisableSystem<HUDSystem>();
	m_o_SystemManager->DisableSystem<WinScreenSystem>();
	m_o_SystemManager->DisableSystem<HelpMenuSystem>();
	m_o_SystemManager->DisableSystem<PauseMenuSystem>();
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

		m_o_GameStateManager->Update();
		m_o_EventManager->Update();
		m_o_SystemManager->Update(cappedDt);

		if (GetAsyncKeyState(AEVK_ESCAPE))
		{
			m_b_EngineStatus = false;
		}

		m_o_EntityComponentManager->ResolveDeletes();


		actualDt -= dtCap;
		
		AESysFrameEnd();

	} while (actualDt > 0.0f && m_b_EngineStatus);
}

float getDt()
{
	return actualDt;
}