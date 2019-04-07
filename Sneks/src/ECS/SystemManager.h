/* Start Header****************************************************************/
/*!
\file SystemManager.cpp
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains manager for management of systems

\par Contribution : CY     - 26.32% (GetSystem & MakeSystem)
\par Contribution : Adam   - 73.68% (Everything else)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#pragma once

#include <vector>
#include "EventManager.h"
#include "System.h"
#include "../Utility/Logger.h"
#include "Component.h"

/*
	System Manager
	
	In charge of creation, deletion, updating and other function relating to systems
*/
class SystemManager
{
private:
	std::vector<BaseSystem*> m_v_SystemList;
	EventManager* m_o_EventManager;
	EntityManager* m_o_EntityManager;
	Logger* m_o_Logger;

public:
	size_t GetSystemCount() const;

	/*
		Creates a system. Default parameters
	*/
	template <typename SystemType>
	SystemType* MakeSystem(const char* systemName = "")
	{
		auto newSystem = new SystemType();
		AddSystem(newSystem, true);
		newSystem->SetName(systemName);
		return newSystem;
	}

	/*
		Adds a created system to the system list
	*/
	void AddSystem(BaseSystem* newSystem, bool init = true);
	/*
		Removes the system from the system list
	*/
	void RemoveSystem(BaseSystem* toRemove);
	void Update(float dt);
	//BaseSystem* GetSystem(int ID); 
	//BaseSystem* GetSystem(const char * systemName);
	void Initialize(EventManager* eventManager, EntityManager* entManager);
	SystemManager(Logger* logger);
	~SystemManager();

	/*
		Gets a system by name
	*/
	template <typename SystemType>
	SystemType* GetSystem(const char* Name = "")
	{
		for (BaseSystem* currSystem : m_v_SystemList)
		{
			if (auto system = dynamic_cast<SystemType*>(currSystem))
			{
				if (strlen(Name))
					if (currSystem->GetName() != Name)
						continue;
				return system;
				//return static_cast<SystemType*>(currSystem);
			}
		}
		return nullptr;
	}

	/*
		Disables a system given by template parameter
	*/
	template <typename SystemType>
	void DisableSystem()
	{
		for (auto system : m_v_SystemList)
		{
			if (auto correctSystem = dynamic_cast<SystemType*>(system))
			{
				correctSystem->m_b_isActive = false;
				correctSystem->OnDisable();
				//DisableAllSystemComponents<ComponentType>(type);
				return;
			}
		}
	}
	/*
		Enables a system given by template parameter
	*/
	template <typename SystemType>
	void EnableSystem()
	{
		for (auto system : m_v_SystemList)
		{
			if (auto correctSystem = dynamic_cast<SystemType*>(system))
			{
				correctSystem->m_b_isActive = true;
				correctSystem->OnEnable();
				//EnableAllComponents<ComponentType>(type);
				return;
			}
		}
	}


	template<typename T>
	void DisableAllComponents(Component type)
	{
		auto components = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<T>(type);
		while (components)
		{
			components->m_b_IsActive = false;
			components = static_cast<T*>(components->m_po_NextComponent);
		}
	}
	template<typename T>
	void EnableAllComponents(Component type)
	{
		auto components = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<T>(type);
		while (components)
		{
			components->m_b_IsActive = true;
			components = static_cast<T*>(components->m_po_NextComponent);
		}
	}

	//TODO
	int m_i_DroppedFrames;
};



#endif
