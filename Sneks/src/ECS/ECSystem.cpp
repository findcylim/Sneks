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


ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_o_EventManager			   = new EventManager();
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_o_EntityComponentManager	   = new EntityManager();
	m_o_GameStateManager           = new GameStateManager(kStateMainMenu, m_o_SystemManager, m_o_EntityComponentManager, m_o_EventManager,&m_b_EngineStatus);
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


void ECSystem::LoadLevel1()
{
	m_o_EntityComponentManager->NewEntity<CameraEntity>(kEntityCamera, "Camera");
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>();

	snek->CreateSnek(-200, 0, PI * 3 / 4, 20, "HeadAnim", 0);
	snek->CreateSnek(200, 0, PI * 7 / 4, 20, "SnekHead02", 1);
}

/*******************************************************
Function: InitializeEngine
***************************

	Initialises Engine Components
	Place all initialization functions here.

********************************************************/
void ECSystem::InitializeEngine()
{
	m_o_SystemManager->Initialize(m_o_EventManager, m_o_EntityComponentManager);

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
	auto audio = new AudioSystem();
	auto powerUp = new PowerUpSystem(graphics);
	auto helpMenu = new HelpMenuSystem();
	auto canvas = new CanvasUISystem(graphics);
	auto anim = new AnimationSystem(graphics);
	auto input = new InputSystem();
	auto mainMenu = new MainMenuSystem();
	auto hud = new HUDSystem(graphics);
	auto pauseMenu = new PauseMenuSystem();
	auto winScreen = new WinScreenSystem();


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

	m_o_SystemManager->AddSystem(graphics);
	graphics->SetName("Graphics");
	graphics->Initialize();

	m_o_SystemManager->AddSystem(anim);
	anim->SetName("Animations");
	anim->Initialize();

	m_o_SystemManager->AddSystem(camera);
	camera->SetName("Camera");
	camera->Initialize();

	audio->SetName("Audio");
	m_o_SystemManager->AddSystem(audio);
	audio->Initialize();

	m_o_SystemManager->AddSystem(background);
	background->SetName("Background");
	background->CreateInstancedBackgrounds(2, 2, "Background01");

	m_o_SystemManager->AddSystem(buildings);
	buildings->SetName("Buildings");
	buildings->Initialize();

	/*m_o_SystemManager->AddSystem(levelLoader);
	levelLoader->SetName("LevelLoader");*/
	//levelLoader->LoadLevel(kLevel1);

	m_o_SystemManager->AddSystem(input);
	buildings->SetName("Input");


	/*************************************************************************/
	// UI & MENUS
	/*************************************************************************/

	CanvasEntity* mainMenuCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Main Menu UI");

	m_o_SystemManager->AddSystem(mainMenu);
	mainMenu->Initialize(mainMenuCanvas->GetComponent<CanvasComponent>());
	mainMenu->SetName("Main Menu"); 

	CanvasEntity* HUDCanvas = m_o_EntityComponentManager->NewEntity<CanvasEntity>(kEntityCanvas, "Heads Up Display");

	m_o_SystemManager->AddSystem(hud);
	hud->SetName("HUD");
	hud->Initialize(HUDCanvas->GetComponent<CanvasComponent>());

	m_o_SystemManager->AddSystem(helpMenu);
	helpMenu->Initialize();
	helpMenu->SetName("HelpMenu");

	m_o_SystemManager->AddSystem(powerUp);
	powerUp->SetName("Power Up");
	powerUp->Initialize();

	m_o_SystemManager->AddSystem(pauseMenu);
	pauseMenu->SetName("PauseMenu");
	pauseMenu->Initialize();

	m_o_SystemManager->AddSystem(winScreen);
	winScreen->SetName("WinScreen");
	winScreen->Initialize();

	/*************************************************************************/
	//\\\\\\\\\\END UI & MENUS
	/*************************************************************************/


	/*************************************************************************/
	// Other Entity Creation
	/*************************************************************************/
	LoadLevel1();

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
		if (AEInputCheckTriggered(AEVK_COMMA))
		{
			AEToogleFullScreen(false);
		}
		if (AEInputCheckTriggered(AEVK_PERIOD))
		{
			AEToogleFullScreen(true);
		}
		m_o_EntityComponentManager->ResolveDeletes();

		actualDt -= dtCap;
		
		AESysFrameEnd();

	} while (actualDt > 0.0f && m_b_EngineStatus);
}

float GetDt()
{
	return actualDt;
}