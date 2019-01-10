#pragma once



typedef enum State
{
	state_splashScreen,
	state_mainMenu,
	state_Options,
	state_Credits,
	state_Game,
	state_PauseMenu,
	state_Exit = -1,
	state_Transition = -2,

}State;



class GameStateManager
{
private:
	State	current;	/* Index of the current game state. */
	State	previous;	/* Index of the previous game state. */
	State	next;		/* Index of the next game state. */
public:
	void	GameStateInit();
	State	ReturnCurrentState();
	State	ReturnNextState();
	State	ReturnPreviousState();
	bool	IsValid(State state);
	bool	IsChanging();
	void	SetNextState(State state);
};
