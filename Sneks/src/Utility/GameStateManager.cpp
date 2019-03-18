#include "GameStateManager.h"
#include "Logger.h"
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
#include "../Systems/PowerUpSystem.h"
#include "../Systems/Menus/HelpMenuSystem.h"
#include "../Systems/Menus/PauseMenuSystem.h"

State GameStateManager::m_x_Next = kStateErrorState;
State GameStateManager::m_x_Current = kStateErrorState;
State GameStateManager::m_x_Previous = kStateErrorState;

State GameStateManager::ReturnCurrentState()
{
	return m_x_Current;
}

State GameStateManager::ReturnNextState()
{
	return m_x_Next;
}

State GameStateManager::ReturnPreviousState()
{
	return m_x_Previous;
}

bool GameStateManager::IsChanging()
{
	return (m_x_Current != m_x_Next);
}

bool GameStateManager::IsValid(State state)
{
	return (state >= 0 && state <= 6);
}

void GameStateManager::SetState(State state)
{
	m_x_Next = state;
}

GameStateManager::GameStateManager(State InitialState, EntityManager* entityManagerPtr, SystemManager* systemManagerPtr, EventManager* eventManagerPtr, bool * engineStatus)
{
	m_x_Current		= InitialState;
	m_x_Next		= InitialState;
	m_x_Previous	= InitialState;
	m_o_EntityManager = entityManagerPtr;
	m_o_SystemManager = systemManagerPtr;
	m_o_EventManager = eventManagerPtr;
	EngineStatus = engineStatus;
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::LoadMainMenu()
{
	m_o_SystemManager->EnableSystem<MainMenuSystem>();
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Main Menu UI");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("PlayButton");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("CreditsButton");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("QuitButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
}

void GameStateManager::UnloadMainMenu()
{
	m_o_SystemManager->DisableSystem<MainMenuSystem>();
	m_o_EntityManager->DisableSpecificEntityType<CanvasEntity, kEntityCanvas>("Main Menu UI");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("PlayButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("CreditsButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("QuitButton");
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
}

void GameStateManager::ResetBattle()
{
	//auto graphics = m_o_SystemManager->GetSystem<GraphicsSystem>("Graphics");

	auto snekHead = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);

	while (snekHead)
	{
		m_o_SystemManager->GetSystem<SnekSystem>("Snek")->DeleteSnek(static_cast<SnekHeadEntity*>(snekHead->m_po_OwnerEntity));
		snekHead = static_cast<SnekHeadComponent*>(snekHead->m_po_NextComponent);
	}
	m_o_EntityManager->ResolveDeletes();

	auto camera = m_o_SystemManager->GetSystem<CameraSystem>("Camera");
	camera->RemoveCameraTrackObjects();

	//m_o_SystemManager->RemoveSystem(m_o_SystemManager->GetSystem<BaseSystem>("Snek"));
	//auto snek = new SnekSystem(m_o_EntityManager, graphics, this);
	//m_o_SystemManager->AddSystem(snek);
	//snek->SetName("Snek");
	//snek->Initialize();
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>("Snek");
	snek->CreateSnek(-200, 0, PI, 20, "HeadAnim", 0);
	snek->CreateSnek(200, 0, 0, 20, "SnekHead02", 1);

	auto buildings = m_o_SystemManager->GetSystem<BuildingsSystem>("Buildings");
	buildings->RemoveBuildings();
	buildings->GenerateNewBuildings(500);

	
	//auto buildings = new BuildingsSystem(m_o_EntityManager, graphics);
	//m_o_SystemManager->AddSystem(buildings);
	//buildings->SetName("Buildings");
	//buildings->Initialize();
	snek->ResetDamage();
	snek->ResetLives();
}

void GameStateManager::LoadBattle()
{
	m_o_SystemManager->EnableSystem<PhysicsSystem>();
	m_o_SystemManager->EnableSystem<HUDSystem>();
	m_o_SystemManager->EnableSystem<PowerUpSystem>();
	m_o_SystemManager->EnableSystem<BackgroundSystem>();
	m_o_SystemManager->EnableSystem<BuildingsSystem>();
	m_o_SystemManager->EnableSystem<LevelLoaderSystem>();
	m_o_SystemManager->EnableSystem<SnekSystem>();
	m_o_SystemManager->EnableSystem<ProjectileSystem>();
	m_o_SystemManager->EnableSystem<ParticleSystem>();

	auto cameraComponent = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_x_CameraAttributes.speedDecay = 0.9f;
	cameraComponent->m_b_TrackObjects = true;
}

void GameStateManager::UnloadBattle()
{
	m_o_SystemManager->DisableSystem<PhysicsSystem>();
	m_o_SystemManager->DisableSystem<HUDSystem>();
	m_o_SystemManager->DisableSystem<PowerUpSystem>();
	m_o_SystemManager->DisableSystem<BackgroundSystem>();
	m_o_SystemManager->DisableSystem<BuildingsSystem>();
	m_o_SystemManager->DisableSystem<LevelLoaderSystem>();
	m_o_SystemManager->DisableSystem<SnekSystem>();
	m_o_SystemManager->DisableSystem<ProjectileSystem>();
	m_o_SystemManager->DisableSystem<ParticleSystem>();
}

void GameStateManager::LoadHelpMenu()
{
	m_o_SystemManager->EnableSystem<HelpMenuSystem>();
	if (m_x_Previous == kStateMainMenu)
		m_o_SystemManager->GetSystem<HelpMenuSystem>("HelpMenu")->SetNextState(kStateGame);
}

void GameStateManager::UnloadHelpMenu()
{
	m_o_SystemManager->DisableSystem<HelpMenuSystem>();
}

void GameStateManager::UnloadWinScreen()
{
	//m_o_EntityManager->AddToDeleteQueue(m_o_EntityManager->GetSpecificEntityInstance<CanvasEntity>(kEntityCanvas, "WinScreenEntity"));
	//m_o_SystemManager->RemoveSystem(WinScreen);
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("RestartButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("ReturnToMainButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Background");
}

void GameStateManager::LoadWinScreen()
{
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>("Snek");

	if (snek->GetP1Lives() <= 0)
	{
		m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	}

	else if (snek->GetP2Lives() <= 0)
	{
		m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	}

	m_o_EntityManager->DisableSpecificEntityType<SnekHeadEntity, kEntitySnekHead>("Head");
}

void GameStateManager::LoadPauseMenu()
{
	m_o_SystemManager->EnableSystem<PauseMenuSystem>();
	m_o_SystemManager->DisableSystem<PhysicsSystem>();
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
}

void GameStateManager::UnloadPauseMenu()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
	m_o_SystemManager->DisableSystem<PauseMenuSystem>();
}

void GameStateManager::LoadCountdown()
{
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("CountdownEntity");
	timeStamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

void GameStateManager::UnloadCountdown()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("CountdownEntity");
}

void GameStateManager::ExitGame()
{
	*EngineStatus = false;
}

void GameStateManager::Load()
{
//	if (m_x_Previous == kStateWinScreen && m_x_Current == kStateMainMenu)
	if (m_x_Previous == kStateWinScreen)
	{
		ResetBattle();
	}
	switch (m_x_Current) {
	case kStateMainMenu:    LoadMainMenu();
		break;

	case kStateGame:		LoadBattle();
		break;

	case kStateWinScreen:	LoadWinScreen();
							break;
	case kStateHelpMenu:	LoadHelpMenu();
							break;
	case kStatePause:		LoadPauseMenu();
							break;	
	case kStateExit:		ExitGame();
		break;
	}
	m_x_Current = m_x_Next;
}

void GameStateManager::Unload()
{
	switch (m_x_Previous) {
	case kStateMainMenu:    UnloadMainMenu();
							break;

	case kStateGame:		UnloadBattle();
							break;

	case kStateWinScreen:	UnloadWinScreen();
							break;
	case kStateHelpMenu:	UnloadHelpMenu();
							break;
	case kStateCountdown:	UnloadCountdown();
							break;
	case kStatePause:		UnloadPauseMenu();
							break;
	}
}

void GameStateManager::Update()
{
	if (m_x_Current != m_x_Next)
	{
		m_x_Previous = m_x_Current;
		m_x_Current = m_x_Next;
		Unload();
		Load();
	}

	if (GetAsyncKeyState(AEVK_P) && m_x_Current == kStateGame)
		SetState(kStatePause);

	if (m_x_Current == kStateCountdown)
		if ((std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - timeStamp) > 3.5) // check if countdown is over
			m_x_Next = kStateGame;

	if (m_x_Current == kStateRestart)
		m_x_Next = kStateGame;
}