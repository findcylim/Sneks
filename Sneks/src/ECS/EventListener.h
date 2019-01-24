#pragma once

#include <typeindex>

template<typename T>
std::type_index getTypeIndex()
{
	return std::type_index(typeid(T));
}

//This base class is so that we can manage EventListeners without knowing their template types
class BaseEventListener
{
public:
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


	//	Derived Systems that inherit this listener will overload their own receieve functions for whatever 
	//	event that they are listening for.

	virtual void receive(const T& eventData) = 0;

};
