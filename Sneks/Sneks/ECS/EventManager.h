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

class Callback
{
	Callback(void*(*f)(void *));
	virtual ~Callback() {}

	void(*function)(void *);
};

class Event
{
public:
	short EventId;
	char * EventName;

	/*****************************************************************************
	A map of function pointers(returning a void pointer and taking in a variable 
	vector of void* of which the function has to typecast into its proper type), 
	which is mapped to the vector of void* parameter     

	Using std::make_pair to add to the functionlist
	******************************************************************************/

	Callback* EventCallback;

	Event();
	virtual ~Event();
};


class EventManager
{
	
protected : 
	Logger * LogObj;
	static EventManager * EventInstance;
public:
	void Update();
	bool AddEvent(short EventID, char *Description,Callback* cb);
	template<typename T>
	bool CallEvent(short EventID, void* d1 = 0, void* d2 = 0, void* d3 = 0, void* d4 = 0, void* d5 = 0);
	static EventManager* Instance();
	static void ResetInstance();
	void Initialize(Logger* logger);
private:
	EventManager();
	virtual ~EventManager();
	bool hasEvent(short EventId);
	std::vector<Event*> EventList;
};

