#ifndef GSMANAGER_H
#define GSMANAGER_H
#pragma once

#include "../ECS/EntityManager.h"
#include "../ECS/SystemManager.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../ECS/EntityManager.h"
#include "../Systems/GraphicsSystem.h"

class WinScreenSystem;
typedef void(*FP)(void);

typedef enum State
{
	kStateSplashScreen,
	kStateMainMenu,
	kStateCharacterSelection,
	kStateOptions,
	kStateCountdown,
	kStateGame,
	kStateHelpMenu,
	kStatePauseMenu,
	kStateWinScreen,
	kStatePause,
	kStateCreditsScreen,
	kStateRestart,
	kStateExit = -1,
	kStateTransition = -2,
	kStateErrorState = -99999
}State;

class GameStateManager
{
private:
	static State	m_x_Current;	/* Index of the m_x_Current game state. */
	static State	m_x_Previous;	/* Index of the m_x_Previous game state. */
	static State	m_x_Next;		/* Index of the m_x_Next game state. */
	time_t timeStamp;
	WinScreenSystem* WinScreen;
	EntityManager* m_o_EntityManager;
	SystemManager* m_o_SystemManager;
	EventManager* m_o_EventManager;

	GraphicsSystem* m_po_GraphicsSystem;
	bool m_b_PutTransition = false;
	bool m_b_RemoveTransition = false;
	float m_f_ScreenMoveSpeed = 5000.0f;
	float m_f_ScreenWidth = 2500.0f;
	ScreenOverlayEntity* m_x_TransitionEntity = nullptr;
public:
	time_t ReturnTimeStamp();
	static	State	ReturnCurrentState();
	static	State	ReturnNextState();
	static	State	ReturnPreviousState();
	bool	IsValid(State state);
	bool	IsChanging();
	bool *	EngineStatus;
	static 	void	SetState(State state);
	GameStateManager(State InitialState,  SystemManager* systemManagerPtr, EntityManager* entityManager, EventManager* eventManagerPtr, bool* engineStatus);
	~GameStateManager();

	void AddGraphics(GraphicsSystem* graphics);
	void Update(float dt);
	void ExitGame();
	void Load();
	void Unload();
	void LoadMainMenu();
	void UnloadMainMenu();
	void LoadSnekSelect();
	void UnloadSnekSelect();
	void LoadBattle();
	void UnloadBattle();
	void LoadWinScreen();
	void UnloadWinScreen();
	void LoadHelpMenu();
	void UnloadHelpMenu();
	void ResetBattle();
	void LoadPauseMenu();
	void UnloadPauseMenu();
	void LoadCountdown();
	void UnloadCountdown();
	void LoadSplashScreen();
	void UnloadSplashScreen();
	void LoadCreditsScreen();
	void UnloadCreditsScreen();
	void LoadOptions();
	void UnloadOptions();
};
#endif