#include "EventManager.h"
using namespace std;

std::list<Event*> EventManager::EventQueue;
std::vector<vector<CallbackP>> EventManager::Event_CallBackList;
//Constructor
EventManager::EventManager()
{
	//TODO EDIT THIS TO THE REAL NUMBER OF MAX EVENTS WE HAVE IN ENUM
	for (char i = 0; i < 100; i++)
	{
		std::vector<CallbackP> vcbp;
		Event_CallBackList.push_back(vcbp);
	}
}

EventManager* EventManager::EventInstance = 0;


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
		for (std::vector<CallbackP>::iterator vit = Event_CallBackList[(*it)->EventId].begin();
			vit != Event_CallBackList[(*it)->EventId].end(); vit++)
		{
			(*vit)->function((*it)->Data,(*vit)->CalleePtr);
		}
		//TODO change this to our own memory allocator later
		free((*it)->Data);
	}
}

bool EventManager::hasEvent(short EventId)
{
	if (Event_CallBackList.size() > EventId)
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
		cbp->EventId = EventID;
		cbp->function = fp;
		cbp->CalleePtr = callee;
		EventManager::Event_CallBackList[EventID].push_back(cbp);
		return true;
	}
	else
	{
		Logger::LogMessage("Error 1000 : Cannot add callback as event does not exist");
		return false;
	}
}

bool EventManager::EmitEvent(short EventID,void* data)
{
	//Add check if eventid is legit
	Event* newEvent = new Event;
	newEvent->EventId = EventID;
	newEvent->Data = data;

	EventQueue.push_back(newEvent);
	return true;
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



void EventManager::Initialize()
{

}
