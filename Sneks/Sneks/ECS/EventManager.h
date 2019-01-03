#pragma once



#include <vector>
#include <map>
#include "Logger.h"

using namespace std;
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
typedef std::map<void*(*)(...), std::vector<void*>> EventFunctionMap;

typedef struct Event
{
	short EventId;
	char * EventDescription;

	/*****************************************************************************
	A map of function pointers(returning a void pointer and taking in a variable 
	vector of void* of which the function has to typecast into its proper type), 
	which is mapped to the vector of void* parameter     

	Using std::make_pair to add to the functionlist
	******************************************************************************/

	EventFunctionMap FunctionList;
	
}Event;


class EventManager
{
	
protected : 
	Logger * LogObj;
	static EventManager * EventInstance;
public:
	void Update();
	bool AddEvent(short EventID, char *Description);
	bool AddFunctionToEvent(const char *, void*(*)(...), std::vector<void*>);
	bool EmitEvent(short EventID);
	bool EventOccured(short EventID);
	static EventManager* Instance();
	static void ResetInstance();
private:
	EventManager();
	virtual ~EventManager();
	bool hasEvent(short EventId);
	std::vector<Event*> EventList;
};

