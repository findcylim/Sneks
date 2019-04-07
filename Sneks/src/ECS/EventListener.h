/* Start Header****************************************************************/
/*!
\file EventListener.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for BaseEventListeners

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#ifndef EVENT_LISTENER
#define EVENT_LISTENER

#include "../Utility/RTTIHelper.h"

// BaseSystem forward declaration
class BaseSystem;

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
	BaseSystem* systemListener = nullptr;

	//	Derived Systems that inherit this listener will overload their own receieve functions for whatever 
	//	event that they are listening for.

	virtual void Receive(const T& eventData) = 0;
};

#endif