
#include"Utility.h"



Utility::Utility()
{
	m_Logger	= new Logger();
	m_GSManager = new GameStateManager();

	m_GSManager->GameStateInit();
}

Utility::~Utility()
{
	delete(m_Logger);
	delete(m_GSManager);
}









