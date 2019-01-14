#include "EventManager.h"

//Constructor
EventManager::EventManager(Logger* logger)
{
	m_o_Logger = logger;
	//TODO EDIT THIS TO THE REAL NUMBER OF MAX EVENTS WE HAVE IN ENUM
	for (char i = 0; i < 100; i++)
	{
		std::vector<CallbackP> v_CallbackP;
		m_EventCallBackList.push_back(v_CallbackP);
	}
}


//Destructor
EventManager::~EventManager()
{
	
}


void EventManager::Update()
{
	ProcessEvents();
}

void EventManager::ProcessEvents()
{
	for (std::list<Event*>::iterator i_l_EventIterator = EventQueue.begin(); i_l_EventIterator != EventQueue.end(); i_l_EventIterator++)
	{
		for (std::vector<CallbackP>::iterator i_v_EventCallbackIterator = m_EventCallBackList[(*i_l_EventIterator)->EventId].begin();
											  i_v_EventCallbackIterator != m_EventCallBackList[(*i_l_EventIterator)->EventId].end();
											  i_v_EventCallbackIterator++)
		{
			(*i_v_EventCallbackIterator)->m_function((*i_l_EventIterator)->Data,(*i_v_EventCallbackIterator)->m_CalleePtr);
		}
		//TODO change this to our own memory allocator later
		free((*i_l_EventIterator)->Data);
	}
	EventQueue.clear();
}

bool EventManager::hasEvent(short EventId)
{
	if (static_cast<short>(m_EventCallBackList.size()) > EventId)
	{
		return true;
	}
	return false;
}

bool EventManager::AddCallback(short eventID, FunctionP functionPointer,void* callee)
{
	if (hasEvent(eventID))
	{
		CallbackP cbp		= new CallbackT;
		cbp->m_EventId		= eventID;
		cbp->m_function		= functionPointer;
		cbp->m_CalleePtr	= callee;
		EventManager::m_EventCallBackList[eventID].push_back(cbp);
		return true;
	}
	else
	{
		m_o_Logger->LogMessage(LOGGER_SYSTEM,"Error 1000 : Cannot add callback %s as event does not exist(EventManger.cpp)",typeid(functionPointer).name());
		return false;
	}
}

bool EventManager::EmitEvent(short eventID,void* data)
{
	//Add check if eventid is legit
	if (eventID <= static_cast<short>(m_EventCallBackList.size()))
	{
		Event* newEvent		 = new Event;
		newEvent->EventId	 = eventID;
		newEvent->Data		 = data;
		EventQueue.push_back(newEvent);
		return true;
	}
	return false;
}
/*
	TODO
	REMOVE CALLBACKS FROM DEAD MEMBERS
	Call this in destructors of objects that add callbacks
*/
bool EventManager::RemoveCallbackFromEvent(short eventID, FunctionP functionPointer, void* callee)
{
	for (std::vector<CallbackP>::iterator i_v_EventCallbackIterator = m_EventCallBackList[eventID].begin();
										  i_v_EventCallbackIterator != m_EventCallBackList[eventID].end();
										  i_v_EventCallbackIterator++)
	{
		try
		{
			if ((*i_v_EventCallbackIterator)->m_function == functionPointer)
			{
				if ((*i_v_EventCallbackIterator)->m_CalleePtr == callee)
				{
					delete(*i_v_EventCallbackIterator);
					m_EventCallBackList[eventID].erase(i_v_EventCallbackIterator);
				}
			}
		}
		catch(...)
		{
			m_o_Logger->LogMessage(LOGGER_SYSTEM,"Error 1004 : Exception caught at %s EventManager::RemoveCallback","RemoveCallbackFromEvent");
			return false;
		}
	}
	return true;
}

bool EventManager::RemoveCallback(FunctionP FPRef, void* callee)
{
	for (std::vector<std::vector<CallbackP>>::iterator i_l_EventIterator = m_EventCallBackList.begin();
													   i_l_EventIterator != m_EventCallBackList.end(); 
													   i_l_EventIterator++)
	{
		for (std::vector<CallbackP>::iterator i_v_EventCallbackIterator = (*i_l_EventIterator).begin();
											  i_v_EventCallbackIterator != (*i_l_EventIterator).end(); 
											  i_v_EventCallbackIterator++)
		{
			try
			{
				if ((**i_v_EventCallbackIterator).m_function == FPRef)
				{
					if ((**i_v_EventCallbackIterator).m_CalleePtr == callee)
					{
						delete(*i_v_EventCallbackIterator);
						(*i_l_EventIterator).erase(i_v_EventCallbackIterator);
					}
				}
			}
			catch (...)
			{
				m_o_Logger->LogMessage(LOGGER_SYSTEM, "Error 1004 : Exception caught at %s EventManager::RemoveCallback", "RemoveCallback");
				return false;
			}
		}
	}
	return true;
}

void EventManager::ResetInstance()
{
	
}

void EventManager::Initialize()
{

}
