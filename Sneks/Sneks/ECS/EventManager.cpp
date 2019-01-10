#include "EventManager.h"

//Constructor
EventManager::EventManager(Utility* UtilityRef)
{
	m_UtilityPtr = UtilityRef;

	//TODO EDIT THIS TO THE REAL NUMBER OF MAX EVENTS WE HAVE IN ENUM
	for (char i = 0; i < 100; i++)
	{
		std::vector<CallbackP> vcbp;
		m_EventCallBackList.push_back(vcbp);
	}
}


//Destructor
EventManager::~EventManager()
{
	
}


void EventManager::Update()
{


}

void EventManager::ProcessEvents()
{
	for (std::list<Event*>::iterator it = EventQueue.begin(); it != EventQueue.end(); it++)
	{
		for (std::vector<CallbackP>::iterator vit = m_EventCallBackList[(*it)->EventId].begin();
			vit != m_EventCallBackList[(*it)->EventId].end(); vit++)
		{
			(*vit)->m_function((*it)->Data,(*vit)->m_CalleePtr);
		}
		//TODO change this to our own memory allocator later
		free((*it)->Data);
	}
}

bool EventManager::hasEvent(short EventId)
{
	if (m_EventCallBackList.size() > EventId)
	{
		return true;
	}
	return false;
}

bool EventManager::AddCallback(short EventID, FunctionP fp,void* callee)
{
	if (hasEvent(EventID))
	{
		CallbackP cbp = new CallbackT;
		cbp->m_EventId = EventID;
		cbp->m_function = fp;
		cbp->m_CalleePtr = callee;
		EventManager::m_EventCallBackList[EventID].push_back(cbp);
		return true;
	}
	else
	{
		m_UtilityPtr->m_Logger->LogMessage("Error 1000 : Cannot add callback %s as event does not exist(EventManger.cpp)",typeid(fp).name());
		return false;
	}
}

bool EventManager::EmitEvent(short EventID,void* data)
{
	//Add check if eventid is legit
	if (EventID <= m_EventCallBackList.size())
	{
		Event* newEvent = new Event;
		newEvent->EventId = EventID;
		newEvent->Data = data;

		EventQueue.push_back(newEvent);
		return true;
	}
	return false;
}
/*
	TODO
	REMOVE CALLBACKS FROM DEAD MEMBERS
*/
bool EventManager::RemoveCallbackFromEvent(short EventID, FunctionP FPRef, void* callee)
{
	for (std::vector<CallbackP>::iterator vit = m_EventCallBackList[EventID].begin();
		vit != m_EventCallBackList[EventID].end(); vit++)
	{
		try
		{
			if ((**vit).m_function == FPRef)
			{
				if ((**vit).m_CalleePtr == callee)
				{
					delete(*vit);
					m_EventCallBackList[EventID].erase(vit);
				}
			}
		}
		catch(...)
		{
			m_UtilityPtr->m_Logger->LogMessage("Error 1004 : Exception caught at %s EventManager::RemoveCallback","RemoveCallbackFromEvent");
			return false;
		}
	}
	return true;
}

bool EventManager::RemoveCallback(FunctionP FPRef, void* callee)
{
	for (std::vector<std::vector<CallbackP>>::iterator it = m_EventCallBackList.begin();
		it != m_EventCallBackList.end(); it++)
	{
		for (std::vector<CallbackP>::iterator vit = (*it).begin();
			vit != (*it).end(); vit++)
		{
			try
			{
				if ((**vit).m_function == FPRef)
				{
					if ((**vit).m_CalleePtr == callee)
					{
						delete(*vit);
						(*it).erase(vit);
					}
				}
			}
			catch (...)
			{
				m_UtilityPtr->m_Logger->LogMessage("Error 1004 : Exception caught at %s EventManager::RemoveCallback", "RemoveCallback");
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
