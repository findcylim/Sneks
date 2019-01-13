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
	state_Exit = -1,
	state_Transition = -2,
	state_ErrorState = -99999
}State;

class GameStateManager
{
private:
	State	current;	/* Index of the current game state. */
	State	previous;	/* Index of the previous game state. */
	State	next;		/* Index of the next game state. */
public:
	State	ReturnCurrentState();
	State	ReturnNextState();
	State	ReturnPreviousState();
	bool	IsValid(State state);
	bool	IsChanging();
	void	SetNextState(State state);
	GameStateManager(State InitialState);
	~GameStateManager();
};
#endif