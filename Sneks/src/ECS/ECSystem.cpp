#include <windows.h>
#include "ECSystem.h"
#include "../Utility/FileIO.h"
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
#include <iostream>

ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_o_EventManager			   = new EventManager(m_o_Logger);
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_o_GameStateManager		   = new GameStateManager(kStateGame);
	m_o_EntityComponentManager	= new EntityManager();
	m_b_EngineStatus			   = false;
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
	
	
	m_o_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	m_o_Logger->LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_o_SystemManager->Initialize(m_o_EventManager, m_o_EntityComponentManager);
	/*
		Create and add Systems here
	*/

	auto cameraEntity = static_cast<CameraEntity*>(
		m_o_EntityComponentManager->NewEntity(kEntityCamera, "Camera"));

	auto graphics = new GraphicsSystem(m_o_EntityComponentManager);
	m_o_SystemManager->AddSystem(graphics);
	graphics->SetID(0);
	graphics->PreLoadTextures();

	auto physics = new PhysicsSystem(m_o_EntityComponentManager);
	m_o_SystemManager->AddSystem(physics);
	physics->Initialize(m_o_GameStateManager);
	physics->SetID(1);

	auto camera = new CameraSystem(m_o_EntityComponentManager);
	m_o_SystemManager->AddSystem(camera);
	camera->Initialize();
	camera->SetID(3);

	auto levelLoader = new LevelLoaderSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager,graphics);
	m_o_SystemManager->AddSystem(levelLoader);
	levelLoader->SetID(4);
	//levelLoader->LoadLevel(kLevel1);

	auto snek = new SnekSystem(m_o_EntityComponentManager, graphics);
	m_o_SystemManager->AddSystem(snek);
	snek->CreateSnek(-200, 0, PI, 20, "SnekHead01",0);
	snek->CreateSnek(200, 0, 0, 20, "SnekHead02",1);
	snek->Initialize();

	auto background = new BackgroundSystem(m_o_EntityComponentManager, graphics);
	m_o_SystemManager->AddSystem(background);
	background->CreateInstancedBackgrounds(2, 2, "Background01");

	auto buildings = new BuildingsSystem(m_o_EntityComponentManager, graphics);
	m_o_SystemManager->AddSystem(buildings);
	buildings->Initialize();

	auto collisions = new CollisionSystem(m_o_EntityComponentManager);
	m_o_SystemManager->AddSystem(collisions);
	collisions->Initialize();
	m_b_EngineStatus = true;

	auto projectile = new ProjectileSystem(m_o_EntityComponentManager, graphics);
	m_o_SystemManager->AddSystem(projectile);
	projectile->Initialize();
}

bool ECSystem::IsEngineOn() const
{
	return m_b_EngineStatus;
}

void ECSystem::Update()
{
	AESysFrameStart();


	auto dt = static_cast<float>(AEFrameRateControllerGetFrameTime());

	m_o_EventManager->Update();
	m_o_SystemManager->Update(dt);

	if (GetAsyncKeyState(AEVK_ESCAPE))
	{
		m_b_EngineStatus = false;
	}
	m_o_EntityComponentManager->ResolveDeletes();
	AESysFrameEnd();
}

