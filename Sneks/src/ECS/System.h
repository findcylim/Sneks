#ifndef SYSTEM_H
#define SYSTEM_H

#pragma once
#include "../Utility/MemoryAllocator.h" //new overload

#include "EventManager.h"
#include "EntityManager.h"

//Virtual System Class
//To be overriden by actual systems
//Needs to be used to manage different systems in the SystemManager
class BaseSystem 
{
protected:
	const char*                m_c_SystemName = "Unknown";
	short		                  m_s_SystemID;
	bool		                  m_b_isActive;
public:
	EventManager*              m_o_EventManagerPtr;
	EntityManager*		         m_po_EntityManager;
	ComponentManager*          m_po_ComponentManager;
	BaseSystem() = default;
	BaseSystem(EntityManager*);
	virtual void Update(float dt) = 0;
	void SetID(short id);
	void SetName(const char* name);
	virtual ~BaseSystem() {}
	short GetID();
	const char * GetName();
};

#endif