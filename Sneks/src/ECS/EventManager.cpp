#include "EventManager.h"

//Constructor
EventManager::EventManager(Logger* logger)
{
	m_o_Logger = logger;
	//TODO EDIT THIS TO THE REAL NUMBER OF MAX EVENTS WE HAVE IN ENUM
	/*for (char i = 0; i < 100; i++)
	{
		std::vector<CallbackP> v_CallbackP;
		m_EventCallBackList.push_back(v_CallbackP);
	}*/


}


//Destructor
EventManager::~EventManager()
= default;


void EventManager::Update()
{
	ProcessEvents();
}

void EventManager::ProcessEvents()
{
	
}
