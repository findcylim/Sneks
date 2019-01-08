#pragma once

#include <list>
#include <vector>
#include "../Utility/Logger.h"
#include "SystemManager.h"

using namespace std;

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
	FunctionP function;
	short EventId;
	void* CalleePtr;
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
	static EventManager * EventInstance;
	static std::list<Event*> EventQueue;
public:
	static void Update();
	static void ProcessEvents();
	bool AddCallback(short EventID, FunctionP fp, void* callee);
	bool RemoveCallbackFromEvent(short EventID, FunctionP FPRef, void* callee);
	bool RemoveCallback(FunctionP FPRef, void* callee);
	static bool EmitEvent(short EventID, void* data = 0);
	static EventManager* Instance();
	static void ResetInstance();
	void Initialize();
	EventManager();
	virtual ~EventManager();
private:
	bool hasEvent(short EventId);
	static std::vector<vector<CallbackP>> Event_CallBackList;
};

