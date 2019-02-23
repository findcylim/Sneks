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
#include "../Systems/ParticleSystem.h"
#include "../Systems/LevelEditorSystem.h"
#include "../Utility/Logger.h"
#include <iostream>

ECSystem::ECSystem()
{
	m_o_Logger					   = new Logger("log.txt");
	m_o_EventManager			   = new EventManager(m_o_Logger);
	m_o_SystemManager			   = new SystemManager(m_o_Logger);
	m_o_GameStateManager		   = new GameStateManager(kStateGame);
	m_o_EntityComponentManager	   = new EntityManager();
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
	
	
	m_o_EventManager->Initialize();
	/*
		Create and add Events here
	*/
	m_o_Logger->LogMessage(LOGGER_SYSTEM, "TEST %f", 232.034f);


	m_o_SystemManager->Initialize(m_o_EventManager, m_o_EntityComponentManager);
	/*
		Create and add Systems here
	*/
	m_o_EntityComponentManager->NewEntity(kEntityCamera, "Camera");

	auto camera = new CameraSystem(m_o_EntityComponentManager);
	camera->SetID(0);

	auto graphics = new GraphicsSystem(m_o_EntityComponentManager);
	m_o_SystemManager->AddSystem(graphics);
	graphics->SetID(1);
	graphics->PreLoadTextures();


	auto levelLoader = new LevelLoaderSystem(m_o_EntityComponentManager, m_o_EventManager, m_o_GameStateManager,graphics);
	m_o_SystemManager->AddSystem(levelLoader);
	levelLoader->SetID(2);
	//levelLoader->LoadLevel(kLevel1);


	auto background = new BackgroundSystem(m_o_EntityComponentManager, graphics);
	m_o_SystemManager->AddSystem(background);
	background->SetID(3);
	//background->CreateInstancedBackgrounds(2, 2, "Background01");
	background->CreateInstancedBackgrounds(5, 5, "EditorScale");
	
	auto levelEditor = new LevelEditorSystem(m_o_EntityComponentManager, m_o_Logger,graphics);
	m_o_SystemManager->AddSystem(levelEditor);
	levelEditor->SetID(4);

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

