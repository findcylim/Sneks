#include "EventManager.h"
using namespace std;

#include <functional>
#include <map>


/*
	Ok
	EventHandler is a function object that gets put into the the EventHandler Vector
	An event can contain multiple function objects as an event such as 
	"Damage player" can have multiple function calls and do many different things
	So thats why you can add multiple listeners to a certain event
	Events are distinguished by its EventId

*/
/*
template<typename T>
EventManager<T>::EventManager() 
{
	
}
template<typename T>
EventManager<T>::~EventManager()
{
	
}
template<typename T>
void EventManager<T>::addListener(int eventId, EventHandler listener)
{
	if (hasEvent(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.push_back(listener);
	}
	else
	{
		std::vector<EventHandler> list;
		list.push_back(listener);
		m_handlers.insert(std::make_pair(eventId, list));
	}
}

template<typename T>
void EventManager<T>::emitEvent(int eventId, T value) 
{
	if (hasEvent(eventId)) 
	{
		EventHandlerList& list = m_handlers.at(eventId);
		for (auto & func : list)
		{
			func(value);
		}
	}
	else 
	{
		std::cout << "cannot find eventId" << eventId << std::endl;
	}
}

template<typename T>
bool EventManager<T>::hasEvent(int eventId)
{
	return m_handlers.find(eventId) != m_handlers.end();
}



template<typename T,typename E>
EventManager_2P<T,E>::EventManager_2P()
{

}

template<typename T, typename E>
EventManager_2P<T,E>::~EventManager_2P()
{

}

template<typename T, typename E>
void EventManager_2P<T,E>::addListener(int eventId, EventHandler listener)
{
	if (hasEvent(eventId)) 
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.push_back(listener);
	}
	else 
	{
		std::vector<EventHandler> list;
		list.push_back(listener);
		m_handlers.insert(std::make_pair(eventId, list));
	}
}

template<typename T, typename E>
void EventManager_2P<T, E>::emitEvent(int eventId, T param1, E param2)
{
	if (hasEvent(eventId)) 
	{
		EventHandlerList& list = m_handlers.at(eventId);
		for (auto & func : list)
		{
			func(param1, param2);
		}
	}
	else 
	{
		std::cout << "cannot find eventId" << eventId << std::endl;
	}
}

template<typename T, typename E>
bool EventManager_2P<T,E>::hasEvent(int eventId) 
{
	return m_handlers.find(eventId) != m_handlers.end();
}




template<typename T, typename E,typename F>
EventManager_3P<T, E, F>::EventManager_3P()
{

}

template<typename T, typename E, typename F>
EventManager_3P<T, E, F>::~EventManager_3P()
{

}

template<typename T, typename E, typename F>
void EventManager_3P<T,E,F>::addListener(int eventId, EventHandler listener) 
{
	if (hasEvent(eventId)) 
	{
		EventHandlerList& list = m_handlers.at(eventId);
		list.push_back(listener);
	}
	else 
	{
		std::vector<EventHandler> list;
		list.push_back(listener);
		m_handlers.insert(std::make_pair(eventId, list));
	}
}

template<typename T, typename E, typename F>
void EventManager_3P<T, E, F>::emitEvent(int eventId, T param1, E param2, F param3) {
	if (hasEvent(eventId))
	{
		EventHandlerList& list = m_handlers.at(eventId);
		for (auto & func : list) {
			func(param1, param2, param3);
		}
	}
	else 
	{
		std::cout << "cannot find eventId" << eventId << std::endl;
	}
}

template<typename T, typename E, typename F>
bool EventManager_3P<T, E, F>::hasEvent(int eventId) 
{
	return m_handlers.find(eventId) != m_handlers.end();
}*/

//Constructor
EventManager::EventManager()
{
	
}


//Destructor
EventManager::~EventManager()
{

}


void EventManager::Update()
{


}

bool EventManager::hasEvent(short EventId)
{
	for (auto & event : EventList)
	{
		if (event->EventId == EventId)
		{
			return true;
		}
	}
	return false;
}

bool EventManager::AddEvent(short EventID, char * Desc)
{
	if (!hasEvent(EventID))
	{
		Event* newEvent = new Event;
		newEvent->EventId = EventID;
		newEvent->EventDescription = Desc;
	}
	else
	{
		EventManager::LogObj->LogMessage("Error 1000 : Cannot add new event at ");
		return false;
	}
}
void EventManager::EmitEvent(short EventID) 
{


}
bool EventManager::EventOccured(short EventID) 
{


}

