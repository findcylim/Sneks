#pragma once



#include <vector>
#include "Logger.h"
/*
	Handles Error Codes : 1000 - 1999
*/


/*
#ifndef EVENTMANAGER1
#define EVENTMANAGER1

template <typename T>
class EventManager
{
private:
	typedef std::function<void(T)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	EventManager();
	virtual ~EventManager();
	void addListener(int eventId, EventHandler listener);
	void emitEvent(int eventId, T value);
	bool hasEvent(int eventId);
};
#endif

#ifndef EVENTMANAGER2
#define EVENTMANAGER2
template <typename T, typename E>
class EventManager_2P 
{
private:
	typedef std::function<void(T, E)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;

public:
	EventManager_2P();
	virtual ~EventManager_2P();
	void addListener(int eventId, EventHandler listener);
	void emitEvent(int eventId, T param1, E param2);
	bool hasEvent(int eventId);
};
#endif



#ifndef EVENTMANAGER3
#define EVENTMANAGER3
template <typename T, typename E, typename F>
class EventManager_3P
{
private:
	typedef std::function<void(T, E, F)> EventHandler;
	typedef std::vector<EventHandler> EventHandlerList;
	std::map<int, EventHandlerList> m_handlers;
public:
	EventManager_3P();
	virtual ~EventManager_3P();
	void addListener(int eventId, EventHandler listener);
	void emitEvent(int eventId, T param1, E param2, F param3);
	bool hasEvent(int eventId);
};
#endif
*/

typedef struct Event
{
	short EventId;
	char * EventDescription;
}Event;


class EventManager
{
	
protected : 
	Logger * LogObj;
public:
	EventManager();
	virtual ~EventManager();
	void Update();
	bool AddEvent(short EventID, char *Description);
	void EmitEvent(short EventID);
	bool EventOccured(short EventID);
private:
	bool hasEvent(short EventId);
	std::vector<Event*> EventList;
};


