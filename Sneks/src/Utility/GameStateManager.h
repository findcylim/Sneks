/* Start Header****************************************************************/
/*!
\file GameStateManager.h
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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

/* Game State List */
typedef enum State
{
	kStateSplashScreen,
	kStateMainMenu,
	kStateCharacterSelection,
	kStateOptions,
	kStateCountdown,
	kStateGame,
	kStateHelpMenu,
	kStateWinScreen,
	kStatePause,
	kStateCreditsScreen,
	kStateConfirmationScreen,
	kStateEndOfRound,
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

	/* Systems needed */
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
	/* Game State Manager Checks */
	bool	IsValid(State state);
	bool	IsChanging();
	bool *	EngineStatus;

	/* Constructor/Destructor */
	GameStateManager(State InitialState,  SystemManager* systemManagerPtr, EntityManager* entityManager, EventManager* eventManagerPtr, bool* engineStatus);
	~GameStateManager();

	/* General Game State Manager Functions */
	void InitializeTransitionEntity(GraphicsSystem* graphics);
	void Update(float dt);
	void ExitGame();
	void Load();
	void Unload();

	/* Load/Unload helper functions for each state */

	//Main Menu
	void LoadMainMenu();
	void UnloadMainMenu();

	// Snek Select Menu
	void LoadSnekSelect();
	void UnloadSnekSelect();

	// Battle
	void LoadBattle();
	void UnloadBattle();

	// Win Screen
	void LoadWinScreen();
	void UnloadWinScreen();

	// Instructions
	void LoadHelpMenu();
	void UnloadHelpMenu();

	// Pause Menu
	void LoadPauseMenu();
	void UnloadPauseMenu();

	// End of Round Event
	void LoadEndRound();
	void UnloadEndRound();

	// Countdown Event
	void LoadCountdown();
	void UnloadCountdown();

	// Required Logo Screens
	void LoadSplashScreen();
	void UnloadSplashScreen();

	// Credits state
	void LoadCreditsScreen();
	void UnloadCreditsScreen();

	// Options state
	void LoadOptions();
	void UnloadOptions();

	// Confirmation of Destructive Actions
	void LoadConfirm();
	void UnloadConfirm();

	// Reset Battlefield
	void ResetBattle();

	/* Getter/Setter */
	static 	void	SetState(State state);
	static	State	ReturnCurrentState();
	static	State	ReturnNextState();
	static	State	ReturnPreviousState();
};
#endif