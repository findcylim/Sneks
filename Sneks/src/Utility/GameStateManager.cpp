/* Start Header****************************************************************/
/*!
\file GameStateManager.cpp
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 84.81%  (Everything else)
\par Contribution : Edwin  - 15.19%  (Transition Code)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "GameStateManager.h"
#include "Logger.h"
#include "../Systems/InputSystem.h"
#include "../Systems/PhysicsSystem.h"
#include "../Systems/LevelLoaderSystem.h"
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
#include "../Systems/Menus/CreditsScreenSystem.h"
#include "../Systems/Menus/SplashScreenSystem.h"
#include "../Systems/Menus/SnekSelectMenuSystem.h"
#include "../Systems/Menus/OptionsMenuSystem.h"
#include "../Systems/Menus/ConfirmationScreenSystem.h"
#include "../Systems/InputSystem.h"
#include "../ECS/ECSystem.h"

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

GameStateManager::GameStateManager(State InitialState,  SystemManager* systemManagerPtr, EntityManager* entityManager, EventManager* eventManagerPtr, bool * engineStatus)
{
	m_x_Current		= InitialState;
	m_x_Next		= InitialState;
	m_x_Previous	= InitialState;
	m_o_EntityManager = entityManager;
	m_o_SystemManager = systemManagerPtr;
	m_o_EventManager = eventManagerPtr;
	EngineStatus = engineStatus;
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::InitializeTransitionEntity(GraphicsSystem* graphics)
{
	m_po_GraphicsSystem = graphics;
	m_x_TransitionEntity = m_o_EntityManager->NewEntity<ScreenOverlayEntity>(kEntityScreenOverlay, "Transition Screen");
	
	auto transformComponent = m_x_TransitionEntity->GetComponent<TransformComponent>();
	auto drawComponent = m_x_TransitionEntity->GetComponent<DrawComponent>();

	transformComponent->m_x_Position = { 1920, 0 };
	drawComponent->m_f_DrawPriority = 1;
	m_po_GraphicsSystem->InitializeDrawComponent(drawComponent, "Background01", { 0, 0, 0, 0 }, 1, 1);
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
}

void GameStateManager::LoadSnekSelect()
{
	m_o_SystemManager->EnableSystem<SnekSelectMenuSystem>();
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Snek Select UI");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("PlayButton");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("CreditsButton");
	m_o_EntityManager->EnableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("QuitButton");
}

void GameStateManager::UnloadSnekSelect()
{
	m_o_SystemManager->DisableSystem<SnekSelectMenuSystem>();
	m_o_EntityManager->DisableSpecificEntityType<CanvasEntity, kEntityCanvas>("Snek Select UI");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("PlayButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("CreditsButton");
	m_o_EntityManager->DisableSpecificEntity<CanvasButtonEntity, kEntityCanvasButton>("QuitButton");
}

void GameStateManager::ResetBattle()
{
	// auto particleEntity = m_o_EntityManager->GetFirstEntityInstance<ParticleEntity>(kEntityParticle);

	// while (particleEntity)
	// {
	// 	m_o_EntityManager->AddToDeleteQueue(particleEntity);
	// 	particleEntity = static_cast<ParticleEntity*>(particleEntity->m_po_NextEntity);
	// }

	// auto particleSpawnerEntity = m_o_EntityManager->GetFirstEntityInstance<ParticleEffectEntity>(kEntityParticleEffect);

	// while (particleSpawnerEntity)
	// {
	// 	m_o_EntityManager->AddToDeleteQueue(particleSpawnerEntity);
	// 	particleSpawnerEntity = static_cast<ParticleEffectEntity*>(particleSpawnerEntity->m_po_NextEntity);
	// }

	// auto powerupEntity = m_o_EntityManager->GetFirstEntityInstance<PowerUpHolderEntity>(kEntityPowerUpHolder);

	// while (powerupEntity)
	// {
	// 	m_o_EntityManager->AddToDeleteQueue(powerupEntity);
	// 	powerupEntity = static_cast<PowerUpHolderEntity*>(powerupEntity->m_po_NextEntity);
	// }

	m_o_EntityManager->ResolveDeletes();
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>("Snek");
	snek->ResetStage();
	
	// snek->ResetDamageAll();
	// snek->ResetLivesAll();
}

void GameStateManager::LoadBattle()
{
	m_o_SystemManager->DisableSystem<InputSystem>();
	m_o_SystemManager->EnableSystem<PhysicsSystem>();
	m_o_SystemManager->EnableSystem<HUDSystem>();
	m_o_SystemManager->EnableSystem<PowerUpSystem>();
	m_o_SystemManager->EnableSystem<BackgroundSystem>();
	m_o_SystemManager->EnableSystem<BuildingsSystem>();
	m_o_SystemManager->EnableSystem<LevelLoaderSystem>();
	m_o_SystemManager->EnableSystem<SnekSystem>();
	m_o_SystemManager->EnableSystem<ProjectileSystem>();
	m_o_SystemManager->EnableSystem<ParticleSystem>();
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Heads Up Display");
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
	AEInputShowCursor(false);
	auto snekHead = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<SnekHeadComponent>(kComponentSnekHead);
	while (snekHead)
	{
		snekHead->m_i_LivesLeft = 3;
		snekHead = static_cast<SnekHeadComponent*>(snekHead->m_po_NextComponent);
	}
	auto cameraComponent = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
	cameraComponent->m_x_CameraAttributes.speedDecay = 0.9f;
	cameraComponent->m_b_TrackObjects = true;
}

void GameStateManager::UnloadBattle()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("Tutorial UI");
	m_o_SystemManager->DisableSystem<PhysicsSystem>();
	m_o_SystemManager->DisableSystem<HUDSystem>();
	m_o_SystemManager->DisableSystem<PowerUpSystem>();
	m_o_SystemManager->DisableSystem<BackgroundSystem>();
	m_o_SystemManager->DisableSystem<BuildingsSystem>();
	m_o_SystemManager->DisableSystem<LevelLoaderSystem>();
	m_o_SystemManager->DisableSystem<SnekSystem>();
	m_o_SystemManager->DisableSystem<ProjectileSystem>();
	m_o_SystemManager->DisableSystem<ParticleSystem>();
	AEInputShowCursor(true);
	m_o_SystemManager->EnableSystem<InputSystem>();
}

void GameStateManager::LoadHelpMenu()
{
	m_o_SystemManager->EnableSystem<HelpMenuSystem>();
	if (m_x_Previous == kStateCharacterSelection)
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
	//TODO:: Set win screen to player 2 texture if p2 wins
	auto snek = m_o_SystemManager->GetSystem<SnekSystem>("Snek");

	auto winscreen = m_o_SystemManager->GetSystem<WinScreenSystem>("WinScreen");

	if (winscreen->m_c_Winner != snek->GetWinner())
	{
		winscreen->SwapWinScreen();
		winscreen->m_c_Winner = snek->GetWinner();
	}

	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("WinScreenEntity");
	m_o_EntityManager->DisableSpecificEntityType<SnekHeadEntity, kEntitySnekHead>("Head");
}

void GameStateManager::LoadPauseMenu()
{
	m_o_SystemManager->EnableSystem<PauseMenuSystem>();
	SetTimeScale(0);
	//m_o_SystemManager->DisableSystem<PhysicsSystem>();
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
}

void GameStateManager::UnloadPauseMenu()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("PauseMenuEntity");
	SetTimeScale(1.0f);
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

void GameStateManager::LoadSplashScreen()
{
}

void GameStateManager::UnloadSplashScreen()
{
	auto splashScreen = m_o_SystemManager->GetSystem<SplashScreenSystem>("SplashScreen");
	if(splashScreen)
		m_o_SystemManager->RemoveSystem(splashScreen);
}

void GameStateManager::LoadCreditsScreen()
{
	m_o_SystemManager->EnableSystem<CreditsScreenSystem>();
	if (m_x_Previous == kStateMainMenu)
		m_o_SystemManager->GetSystem<CreditsScreenSystem>("CreditsScreen")->SetNextState(kStateMainMenu);
}

void GameStateManager::UnloadCreditsScreen()
{
	m_o_SystemManager->DisableSystem<CreditsScreenSystem>();
}

void GameStateManager::LoadOptions()
{
	m_o_EntityManager->EnableSpecificEntity<CanvasEntity, kEntityCanvas>("OptionsMenuEntity");
	m_o_SystemManager->EnableSystem<OptionsMenuSystem>();
}

void GameStateManager::UnloadOptions()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("OptionsMenuEntity");
	m_o_SystemManager->DisableSystem<OptionsMenuSystem>();
}

void GameStateManager::LoadConfirm()
{
	m_o_SystemManager->EnableSystem<ConfirmationScreenSystem>();
}

void GameStateManager::UnloadConfirm()
{
	m_o_EntityManager->DisableSpecificEntity<CanvasEntity, kEntityCanvas>("ConfirmationScreen");
	m_o_SystemManager->DisableSystem<ConfirmationScreenSystem>();
}


void GameStateManager::ExitGame()
{
	*EngineStatus = false;
}

void GameStateManager::Load()
{
//	if (m_x_Previous == kStateWinScreen && m_x_Current == kStateMainMenu)
	//if (m_x_Previous == kStateWinScreen)
	//{
	//	ResetBattle();
	//}

	switch (m_x_Current) 
	{
	case kStateMainMenu:   
		LoadMainMenu();
		if(m_x_Previous != kStateCreditsScreen && m_x_Previous != kStateSplashScreen)
			ResetBattle();
		break;
	case kStateSplashScreen:
		LoadSplashScreen();
		break;
	case kStateOptions:
		LoadOptions();
		break;
	case kStateGame:
		LoadBattle();
		break;
	case kStateCreditsScreen:
		LoadCreditsScreen();
		break;
	case kStateWinScreen:	
		LoadWinScreen();
		break;
	case kStateCharacterSelection:
		LoadSnekSelect();
		break;
	case kStateHelpMenu:	
		LoadHelpMenu();
		break;
	case kStatePause:		
		LoadPauseMenu();
		break;
	case kStateConfirmationScreen:
		LoadConfirm();
		break;
	case kStateCountdown:
		LoadCountdown();
		break;
	case kStateExit:		
		ExitGame();
		break;
	}
	m_x_Current = m_x_Next;
}

void GameStateManager::Unload()
{
	if (m_x_Next == kStateMainMenu)
	{
		ResetBattle();
	}
	switch (m_x_Previous)
	{
	case kStateSplashScreen:
		UnloadSplashScreen();
		break;
	case kStateMainMenu:    
		UnloadMainMenu();		
		break;
	case kStateOptions:
		UnloadOptions();
		break;
	case kStateGame:		
		UnloadBattle();		
		break;
	case kStateWinScreen:	
		UnloadWinScreen();	
		break;
	case kStateCreditsScreen:
		UnloadCreditsScreen();
		break;
	case kStateCharacterSelection:
		UnloadSnekSelect();
		break;
	case kStateHelpMenu:	
		UnloadHelpMenu();		
		break;
	case kStateConfirmationScreen:
		UnloadConfirm();
		break;
	case kStateCountdown:	
		UnloadCountdown();		
		break;
	case kStatePause:		
		UnloadPauseMenu();			
		break;
	}
}

void GameStateManager::Update(float dt)
{
	if (m_x_Current != m_x_Next && !m_b_PutTransition && !m_b_RemoveTransition)
	{
		m_o_EventManager->EmitEvent<Events::EV_GAME_STATE_CHANGED>(Events::EV_GAME_STATE_CHANGED{ m_x_Next ,m_x_Current });

		if (m_x_Current == kStateMainMenu || m_x_Next == kStateMainMenu || m_x_Next == kStateRestart)
		{
			m_o_SystemManager->DisableSystem<InputSystem>();
			m_o_SystemManager->DisableSystem<CameraSystem>();
			auto transformComponent = m_x_TransitionEntity->GetComponent<TransformComponent>();
			auto cameraComponent = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
			transformComponent->m_x_Position.x = (m_f_ScreenWidth / cameraComponent->m_f_VirtualScale - cameraComponent->m_f_VirtualOffset.x);
			transformComponent->m_x_Position.y = (-cameraComponent->m_f_VirtualOffset.y);
			transformComponent->m_f_Scale = m_f_ScreenWidth / cameraComponent->m_f_VirtualScale;
			m_b_PutTransition = true;
		}
		else
		{
			m_x_Previous = m_x_Current;
			m_x_Current = m_x_Next;
			Unload();
			Load();
		}
	}
	if (m_b_PutTransition)
	{
		auto transformComponent = m_x_TransitionEntity->GetComponent<TransformComponent>();
		auto cameraComponent = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
		transformComponent->m_x_Position.x = (transformComponent->m_x_Position.x - m_f_ScreenMoveSpeed / cameraComponent->m_f_VirtualScale * dt);

		if (transformComponent->m_x_Position.x <= -cameraComponent->m_f_VirtualOffset.x)
		{
			cameraComponent->m_f_VirtualOffset = { 0,0 };
			cameraComponent->m_f_VirtualScale = 1;
			transformComponent->m_x_Position = { 0 ,0 };
			m_b_PutTransition = false;
			m_b_RemoveTransition = true;
			m_x_Previous = m_x_Current;
			m_x_Current = m_x_Next;
			Unload();
			Load();

			if (m_x_Current == kStateRestart)
			{
				ResetBattle();
				m_x_Next = kStateGame;
			}
		}

		m_x_TransitionEntity->GetComponent<DrawComponent>()->SetAlpha(
			((m_f_ScreenWidth / cameraComponent->m_f_VirtualScale) -
				transformComponent->m_x_Position.x - cameraComponent->m_f_VirtualOffset.x) /
				(m_f_ScreenWidth / cameraComponent->m_f_VirtualScale)
		);
	}
	else if (m_b_RemoveTransition)
	{
		auto transformComponent = m_x_TransitionEntity->GetComponent<TransformComponent>();
		auto cameraComponent = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<CameraComponent>(kComponentCamera);
		transformComponent->m_x_Position.x = (transformComponent->m_x_Position.x - m_f_ScreenMoveSpeed / cameraComponent->m_f_VirtualScale * dt);
		transformComponent->m_x_Position.y = (-cameraComponent->m_f_VirtualOffset.y);
		transformComponent->m_f_Scale = m_f_ScreenWidth / cameraComponent->m_f_VirtualScale;

		if (transformComponent->m_x_Position.x <= (-m_f_ScreenWidth / cameraComponent->m_f_VirtualScale) - cameraComponent->m_f_VirtualOffset.x)
		{
			m_b_RemoveTransition = false;
			m_o_SystemManager->EnableSystem<InputSystem>();
			m_o_SystemManager->EnableSystem<CameraSystem>();
		}

		m_x_TransitionEntity->GetComponent<DrawComponent>()->SetAlpha(
			((-m_f_ScreenWidth / cameraComponent->m_f_VirtualScale) -
				transformComponent->m_x_Position.x - cameraComponent->m_f_VirtualOffset.x) /
				(-m_f_ScreenWidth / cameraComponent->m_f_VirtualScale)
		);
	}

	if (GetAsyncKeyState(VK_ESCAPE) && m_x_Current == kStateGame)
		SetState(kStatePause);

	if (m_x_Current == kStateCountdown)
		if ((std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - timeStamp) > 3.5) // check if countdown is over
			m_x_Next = kStateGame;
}