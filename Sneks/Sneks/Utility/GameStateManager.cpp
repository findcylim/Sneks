

#include "GameStateManager.h"
#include "Logger.h"


GameStateManager* GameStateManager::m_GSMInstance = 0;

State	GameStateManager::current;	/* Index of the current game state. */
State	GameStateManager::previous;	/* Index of the previous game state. */
State	GameStateManager::next;		/* Index of the next game state. */
bool	GameStateManager::m_IsInit;

const char * g_Error5005 = "Error 5005: GameStateManager not yet initialized.";

void GameStateManager::GameStateInit(State InitialState)
{
	if (!m_GSMInstance)
	{
		m_GSMInstance = new GameStateManager(InitialState);
	}
	else
	{
		if (!m_IsInit)
		{
			delete(m_GSMInstance);
			m_GSMInstance = new GameStateManager(InitialState);
		}
	}
}

void GameStateManager::Reset(State NewInitialState)
{
	m_IsInit = false;
	GameStateInit(NewInitialState);
}

State GameStateManager::ReturnCurrentState()
{
	if (m_IsInit)
		return current;
	else
	{
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
		return state_ErrorState;
	}
}

State GameStateManager::ReturnNextState()
{
	if (m_IsInit)
		return next;
	else
	{
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
		return state_ErrorState;
	}
}

State GameStateManager::ReturnPreviousState()
{
	if (m_IsInit)
		return previous;
	else
	{
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
		return state_ErrorState;
	}
}

bool GameStateManager::IsChanging()
{
	if (m_IsInit)
		return (current != next);
	else
	{
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
		return false;
	}
}

bool GameStateManager::IsValid(State state)
{
	if (m_IsInit)
		return (state >= 0 && state <= 6);
	else
	{
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
		return false;
	}
}

void GameStateManager::SetNextState(State state)
{
	if (m_IsInit)
		next = state;
	else
		Logger::LogMessage(LOGGER_SYSTEM, g_Error5005);
}

GameStateManager::GameStateManager(State InitialState)
{
	current		= InitialState;
	next		= InitialState;
	previous	= InitialState;
	m_IsInit	= true;
}
GameStateManager::~GameStateManager()
{

}




