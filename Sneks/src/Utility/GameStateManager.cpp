

#include "GameStateManager.h"
#include "Logger.h"






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
	return previous;
}

bool GameStateManager::IsChanging()
{
	return (current != next);
}

bool GameStateManager::IsValid(State state)
{
	return (state >= 0 && state <= 6);
}

void GameStateManager::SetNextState(State state)
{
	next = state;
}

GameStateManager::GameStateManager(State InitialState)
{
	current		= InitialState;
	next		= InitialState;
	previous	= InitialState;
}
GameStateManager::~GameStateManager()
{

}




