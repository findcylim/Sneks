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
	SystemManager* m_o_SystemManager;
	EventManager*              m_po_EventManagerPtr;
	EntityManager*		         m_po_EntityManager;
	ComponentManager*          m_po_ComponentManager;
	BaseSystem() = default;
	BaseSystem(EntityManager*);
	virtual void Update(float dt) = 0;
	virtual void OnEnable() {}
	virtual void OnDisable() {}
	void SetID(short id);
	void SetName(const char* name);
	virtual ~BaseSystem() {}
	short GetID();
	const char * GetName();
	bool operator<(const BaseSystem& rhs) const;
};

#endif