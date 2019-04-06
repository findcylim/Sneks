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

#ifndef SYSTEM_H
#define SYSTEM_H

#pragma once
#include "../Utility/MemoryAllocator.h" //new overload

#include "EventManager.h"
#include "EntityManager.h"
class SystemManager;
//Virtual System Class
//To be overriden by actual systems
//Needs to be used to manage different systems in the SystemManager
class BaseSystem 
{
protected:
	const char*                m_c_SystemName = "Unknown";
	short		                  m_s_SystemID;
public:
	bool		                  m_b_isActive = true;
	SystemManager*					m_po_SystemManager;
	EventManager*              m_po_EventManagerPtr;
	EntityManager*		         m_po_EntityManager;
	ComponentManager*          m_po_ComponentManager;
	BaseSystem() = default;
	//BaseSystem(EntityManager*);
	virtual void Initialize();
	virtual void Update(float dt) = 0;
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	void SetID(short id);
	void SetName(const char* name);
	virtual ~BaseSystem() = default;
	short GetID();
	const char * GetName();
	bool operator<(const BaseSystem& rhs) const;
};

#endif