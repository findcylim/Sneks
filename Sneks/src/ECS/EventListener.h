#pragma once
#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include "../Utility/RTTIHelper.h"

class BaseSystem;
//This base class is so that we can manage EventListeners without knowing their template types
class BaseEventListener
{
public:
	virtual void IReceive(const Events::IEvent&) = 0;
	virtual ~BaseEventListener() {};
};

/**
	The template type takes in what kind of event the system which derives from this is listening for
*/
template<typename T>
class EventListener : public BaseEventListener
{
public:
	virtual ~EventListener() override {}
	BaseSystem* systemListener = nullptr;

	//	Derived Systems that inherit this listener will overload their own receieve functions for whatever 
	//	event that they are listening for.
	void IReceive(const Events::IEvent& event)
	{
		Receive(static_cast<const T&>(event));
	}
	virtual void Receive(const T& eventData) = 0;
};

#endif