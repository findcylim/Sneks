#ifndef GSMANAGER_H
#define GSMANAGER_H
#pragma once

#include "../ECS/EntityManager.h"
#include "../ECS/SystemManager.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../ECS/EntityManager.h"

typedef enum State
{
	kStateSplashScreen,
	kStateMainMenu,
	kStateCharacterSelection,
	kStateOptions,
	kStateCredits,
	kStateGame,
	kStatePauseMenu,
	kStateWinScreen,
	kStateExit = -1,
	kStateTransition = -2,
	kStateErrorState = -99999
}State;

class GameStateManager
{
private:
	State	m_x_Current;	/* Index of the m_x_Current game state. */
	State	m_x_Previous;	/* Index of the m_x_Previous game state. */
	State	m_x_Next;		/* Index of the m_x_Next game state. */
	EntityManager* m_o_EntityManager;
	SystemManager* m_o_SystemManager;
	EventManager* m_o_EventManager;
public:
	State	ReturnCurrentState();
	State	ReturnNextState();
	State	ReturnPreviousState();
	bool	IsValid(State state);
	bool	IsChanging();
	void	SetState(State state);
	GameStateManager(State InitialState, EntityManager* entityManagerPtr, SystemManager* systemManagerPtr, EventManager* eventManagerPtr);
	~GameStateManager();

	void Update();
	void LoadMainMenu();
	void UnloadMainMenu();
	void LoadBattle();
	void UnloadBattle();
	void LoadWinScreen();
	void UnloadWinScreen();
};
#endif