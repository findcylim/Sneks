#ifndef GSMANAGER_H
#define GSMANAGER_H
#pragma once


typedef enum State
{
	kStateSplashScreen,
	kStateMainMenu,
	kStateCharacterSelection,
	kStateOptions,
	kStateCredits,
	kStateGame,
	kStatePauseMenu,
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