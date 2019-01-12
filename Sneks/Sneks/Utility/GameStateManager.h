#ifndef GSMANAGER_H
#define GSMANAGER_H
#pragma once


typedef enum State
{
	state_SplashScreen,
	state_MainMenu,
	state_CharacterSelection,
	state_Options,
	state_Credits,
	state_Game,
	state_PauseMenu,
	state_EditorNormal = 100,
	state_EditorCompiling = 101,
	state_EditorGameplay = 102,
	state_Exit = -1,
	state_Transition = -2,
	state_ErrorState = -99999
}State;

class GameStateManager
{
private:
	static	State	current;	/* Index of the current game state. */
	static	State	previous;	/* Index of the previous game state. */
	static	State	next;		/* Index of the next game state. */
	static	GameStateManager* m_GSMInstance;
	static	bool	m_IsInit;
	GameStateManager(State InitialState);
	~GameStateManager();
public:
	static	void	GameStateInit(State InitialState);
	static	State	ReturnCurrentState();
	static	State	ReturnNextState();
	static	State	ReturnPreviousState();
	static	bool	IsValid(State state);
	static	bool	IsChanging();
	static	void	SetNextState(State state);
	static	void	Reset(State NewInitialState);
};

#endif