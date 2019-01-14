
#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <list>
#include <vector>
#include "../Utility/Logger.h"

enum kEventList
{
	Ev_PLAYER1GAME_LEFTKEY,
	Ev_PLAYER1GAME_RIGHTKEY,
	Ev_PLAYER1GAME_LEFTSHIFTKEY,
	Ev_PLAYER2GAME_LEFTKEY,
	Ev_PLAYER2GAME_RIGHTKEY,
	Ev_PLAYER2GAME_LEFTSHIFTKEY,
};

/*
	<Standard function pointer format for callbacks>
	-Returns void.
	-Takes in void* to data structure, of which you will have to 
	typecast in your callback function.
	-Takes in a void* to member object of which the callback 
	came from.
*/
typedef void(*FunctionP)(void * data,void* callee);

typedef struct CallbackT
{
	FunctionP m_function;
	short m_EventId;
	void* m_CalleePtr;
}CallbackT;

typedef CallbackT* CallbackP;

typedef struct Event
{	
	short EventId;
	void* Data;
}Event;

class EventManager
{
protected : 
	std::list<Event*> EventQueue;
public:
	void Update();
	void ProcessEvents();
	bool AddCallback(short EventID, FunctionP fp, void* callee);
	bool RemoveCallbackFromEvent(short EventID, FunctionP FPRef, void* callee);
	bool RemoveCallback(FunctionP FPRef, void* callee);
	bool EmitEvent(short EventID, void* data = 0);
	void ResetInstance();
	void Initialize();
	EventManager(Logger* logger);
	virtual ~EventManager();
private:
	bool hasEvent(short EventId);
	Logger* m_o_Logger;
	std::vector<std::vector<CallbackP>> m_EventCallBackList;
};

#endif