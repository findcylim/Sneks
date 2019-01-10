

#include "GameStateManager.h"




void GameStateManager::GameStateInit()
{
	current	 = state_mainMenu;
	next	 = state_mainMenu;
	previous = state_mainMenu;

}

State GameStateManager::ReturnCurrentState()
{
	return current;
}

State GameStateManager::ReturnNextState()
{
	return next;
}

State GameStateManager::ReturnPreviousState()
{
	return current;
}

bool GameStateManager::IsChanging()
{
	return (current != next);
}

bool GameStateManager::IsValid(State state)
{
	return (state >= 0 && state <= 5);
}

void GameStateManager::SetNextState(State state)
{
	next = state;
}






