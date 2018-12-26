#include "EventHandler.h"
#include <functional>
#include <vector>
#include <map>
using namespace std;

//Constructor
EventHandler::EventHandler()
{

}

int EventHandler::GetTest()
{
	return test;
}


//Destructor
EventHandler::~EventHandler()
{

}

/*
	Ok
	EventHandler is a function object that gets put into the the EventHandler Vector
	An event can contain multiple function objects as an event such as 
	"Damage player" can have multiple function calls and do many different things
	So thats why you can add multiple listeners to a certain event
	Events are distinguished by its EventId

*/
template <typename T>
class EventManager {
private:
	typedef std::function<void(T)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	void addListener(int eventId, EventHandler listener) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			list.push_back(listener);
		}
		else {
			std::vector<EventHandler> list;
			list.push_back(listener);
			m_handlers.insert(std::make_pair(eventId, list));
		}
	}

	void emitEvent(int eventId, T value) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			for (auto & func : list) {
				func(value);
			}
		}
		else {
			std::cout << "cannot find eventId" << eventId << std::endl;
		}
	}

	bool hasEvent(int eventId) {
		return m_handlers.find(eventId) != m_handlers.end();
	}
};

template <typename T, typename E>
class EventManager_2P {
private:
	typedef std::function<void(T, E)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	void addListener(int eventId, EventHandler listener) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			list.push_back(listener);
		}
		else {
			std::vector<EventHandler> list;
			list.push_back(listener);
			m_handlers.insert(std::make_pair(eventId, list));
		}
	}

	void emitEvent(int eventId, T param1, E param2) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			for (auto & func : list) {
				func(param1, param2);
			}
		}
		else {
			std::cout << "cannot find eventId" << eventId << std::endl;
		}
	}

	bool hasEvent(int eventId) {
		return m_handlers.find(eventId) != m_handlers.end();
	}
};

template <typename T, typename E, typename F>
class EventManager_3P {
private:
	typedef std::function<void(T, E, F)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	void addListener(int eventId, EventHandler listener) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			list.push_back(listener);
		}
		else {
			std::vector<EventHandler> list;
			list.push_back(listener);
			m_handlers.insert(std::make_pair(eventId, list));
		}
	}

	void emitEvent(int eventId, T param1, E param2, F param3) {
		if (hasEvent(eventId)) {
			EventHandlerList& list = m_handlers.at(eventId);
			for (auto & func : list) {
				func(param1, param2, param3);
			}
		}
		else {
			std::cout << "cannot find eventId" << eventId << std::endl;
		}
	}

	bool hasEvent(int eventId) {
		return m_handlers.find(eventId) != m_handlers.end();
	}
};