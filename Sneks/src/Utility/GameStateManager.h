#ifndef GSMANAGER_H
#define GSMANAGER_H
#pragma once

#include "../ECS/EntityManager.h"
#include "../ECS/SystemManager.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../ECS/EntityManager.h"

class WinScreenSystem;
typedef void(*FP)(void);

typedef enum State
{
	kStateSplashScreen,
	kStateMainMenu,
	kStateCharacterSelection,
	kStateOptions,
	kStateCredits,
	kStateCountdown,
	kStateGame,
	kStateHelpMenu,
	kStatePauseMenu,
	kStateWinScreen,
	kStatePause,
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
public:
	time_t ReturnTimeStamp();
	static	State	ReturnCurrentState();
	static	State	ReturnNextState();
	static	State	ReturnPreviousState();
	bool	IsValid(State state);
	bool	IsChanging();
	bool *	EngineStatus;
	static 	void	SetState(State state);
	GameStateManager(State InitialState, EntityManager* entityManagerPtr, SystemManager* systemManagerPtr, EventManager* eventManagerPtr, bool* engineStatus);
	~GameStateManager();

	void Update();
	void ExitGame();
	void Load();
	void Unload();
	void LoadMainMenu();
	void UnloadMainMenu();
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
};
#endif