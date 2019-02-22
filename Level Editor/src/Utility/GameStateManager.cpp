

#include "GameStateManager.h"
#include "Logger.h"






State GameStateManager::ReturnCurrentState()
{
	return m_x_Current;
}

State GameStateManager::ReturnNextState()
{
	return m_x_Next;
}

State GameStateManager::ReturnPreviousState()
{
	return m_x_Previous;
}

bool GameStateManager::IsChanging()
{
	return (m_x_Current != m_x_Next);
}

bool GameStateManager::IsValid(State state)
{
	return (state >= 0 && state <= 6);
}

void GameStateManager::SetNextState(State state)
{
	m_x_Next = state;
}

GameStateManager::GameStateManager(State InitialState)
{
	m_x_Current		= InitialState;
	m_x_Next		= InitialState;
	m_x_Previous	= InitialState;
}
GameStateManager::~GameStateManager()
{

}




