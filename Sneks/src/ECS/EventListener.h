/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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