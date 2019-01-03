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

Event::Event()
{

}
Event::~Event()
{
	
}
Callback::Callback(void(*f)(void *))
{
	Callback::function = f;
}

//Constructor
EventManager::EventManager()
{
	
}

EventManager* EventManager::EventInstance = 0;

//Destructor
EventManager::~EventManager()
{
	
}


void EventManager::Update()
{


}


bool EventManager::hasEvent(short EventId)
{
	for (Event* event : EventList)
	{
		if (event->EventId == EventId)
		{
			return true;
		}
	}
	return false;
}

bool EventManager::AddEvent(short EventID, char *Name, Callback* cb)
{
	if (!hasEvent(EventID))
	{
		Event* newEvent = new Event;
		newEvent->EventId = EventID;
		newEvent->EventName = Name;
		newEvent->EventCallback = cb;
		EventManager::EventList.push_back(newEvent);
		return true;
	}
	else
	{
		if(EventManager::LogObj)
			EventManager::LogObj->LogMessage("Error 1000 : Cannot add new event as it already exists");
		return false;
	}
}

template<class CalleeType>
bool EventManager::CallEvent(short EventID,void* param1, void* param2, void* param3, void* param4, void* param5)
{
	Event* eventObj = EventManager::EventList.at(EventID);

	if (eventObj)
	{
		//Needs to be typecasted into the correct objType

		if (param5)
			(void*(*)(void*, void*, void*, void*, void*))((CalleeType*)eventObj->EventCallback.CalleeObj)
			->eventObj->EventCallback.function(param1, param2, param3, param4, param5);
		else if (param4)
			(void*(*)(void*, void*, void*, void*))((CalleeType*)eventObj->EventCallback.CalleeObj)
			->eventObj->EventCallback.function(param1, param2, param3, param4);
		else if (param3)
			(void*(*)(void*, void*, void*))((CalleeType*)eventObj->EventCallback.CalleeObj)
			->eventObj->EventCallback.function(param1, param2, param3);
		else if (param2)
			(void*(*)(void*, void*))((CalleeType*)eventObj->EventCallback.CalleeObj)
			->eventObj->EventCallback.function(param1, param2);
		else if (param1)
			(void*(*)(void*))((CalleeType*)eventObj->EventCallback.CalleeObj)
			->eventObj->EventCallback.function(param1);
		else
			return false;

		return true;
	}
	return false;
}

void EventManager::ResetInstance()
{
	
}


EventManager* EventManager::Instance()
{
	if (!EventManager::EventInstance)
	{
		EventManager::EventInstance = new EventManager;
	}
	return EventManager::EventInstance;
}



void EventManager::Initialize(Logger* logger)
{
	LogObj = logger;
}
