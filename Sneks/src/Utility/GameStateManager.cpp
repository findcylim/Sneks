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

WinScreenSystem* WinScreen;

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
	m_x_Current = state;
}

GameStateManager::GameStateManager(State InitialState, EntityManager* entityManagerPtr, SystemManager* systemManagerPtr, EventManager* eventManagerPtr)
{
	m_x_Current		= InitialState;
	m_x_Next		= InitialState;
	m_x_Previous	= InitialState;
	m_o_EntityManager = entityManagerPtr;
	m_o_SystemManager = systemManagerPtr;
	m_o_EventManager = eventManagerPtr;
}

GameStateManager::~GameStateManager()
{

}

void GameStateManager::LoadMainMenu()
{
	UnloadBattle();
	UnloadWinScreen();
	m_o_SystemManager->EnableSystem<MainMenuSystem, DrawComponent, kComponentDraw>();
}

void GameStateManager::UnloadMainMenu()
{
	//m_o_SystemManager->DisableSystem<MainMenuSystem, DrawComponent, kComponentDraw>();
}

void GameStateManager::LoadBattle()
{
	UnloadMainMenu();
	UnloadWinScreen();
	m_o_SystemManager->EnableSystem<PhysicsSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<HUDSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<PowerUpSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<BackgroundSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<BuildingsSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<LevelLoaderSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<SnekSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<ProjectileSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->EnableSystem<ParticleSystem, DrawComponent, kComponentDraw>();
}

void GameStateManager::UnloadBattle()
{
	m_o_SystemManager->DisableSystem<PhysicsSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<HUDSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<PowerUpSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<BackgroundSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<BuildingsSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<LevelLoaderSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<SnekSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<ProjectileSystem, DrawComponent, kComponentDraw>();
	m_o_SystemManager->DisableSystem<ParticleSystem, DrawComponent, kComponentDraw>();
}

void GameStateManager::UnloadWinScreen()
{
	//m_o_SystemManager->DisableSystem<WinScreenSystem, DrawComponent, kComponentDraw>();
}

void GameStateManager::LoadWinScreen()
{
	m_o_SystemManager->DisableSystem<PhysicsSystem, DrawComponent, kComponentDraw>();

	if (GetP1Lives() <= 0)
	{
		WinScreen = new WinScreenSystem(m_o_EntityManager, m_o_EventManager, static_cast<char>(2));
		WinScreen->SetName("WinScreen");
		m_o_SystemManager->AddSystem(WinScreen);
	}

	else if (GetP2Lives() <= 0)
	{
		WinScreen = new WinScreenSystem(m_o_EntityManager, m_o_EventManager, static_cast<char>(1));
		WinScreen->SetName("WinScreen");
		m_o_SystemManager->AddSystem(WinScreen);
	}

	m_o_EntityManager->DisableComponentsFromEntityType<SnekBodyEntity, kEntitySnekBody, CollisionComponent>();
	m_o_EntityManager->DisableComponentsFromEntityType<SnekHeadEntity, kEntitySnekHead, CollisionComponent>();
}

void GameStateManager::Update()
{
	switch (m_x_Current) {
	//case kStateMainMenu:    UnloadBattle();
	//						UnloadWinScreen();
	//						LoadMainMenu();
	//						break;

	case kStateGame:		UnloadMainMenu();
							UnloadWinScreen();
							LoadBattle();
							break;

	case kStateWinScreen:	LoadWinScreen();
							break;
	}
}



