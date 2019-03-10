#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#pragma once

#include <vector>
#include "EventManager.h"
#include "System.h"
#include "../Utility/Logger.h"
#include "Component.h"

class SystemManager
{
private:
	std::vector<BaseSystem*> m_v_SystemList;
	EventManager* m_o_EventManager;
	EntityManager* m_o_EntityManager;
	Logger* m_o_Logger;


	template<typename T>
	void DisableAllSystemComponents(Component type)
	{
		auto components = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<T>(type);
		while (components)
		{
			components->m_b_IsActive = false;
			components = static_cast<T*>(components->m_po_NextComponent);
		}
	}
	template<typename T>
	void EnableAllSystemComponents(Component type)
	{
		auto components = m_o_EntityManager->GetComponentManager()->GetFirstComponentInstance<T>(type);
		while (components)
		{
			components->m_b_IsActive = true;
			components = static_cast<T*>(components->m_po_NextComponent);
		}
	}

public:
	size_t GetSystemCount() const;
	void AddSystem(BaseSystem* NewSystem);
	
	void RemoveSystem(BaseSystem* RemSystem);
	void Update(float dt);
	BaseSystem* GetSystem(int ID); 
	BaseSystem* GetSystem(const char * systemName);
	void Initialize(EventManager* eventManager, EntityManager* entManager);
	SystemManager(Logger* logger);
	~SystemManager();

	template <typename SystemType,typename ComponentType,Component type>
	void DisableSystem()
	{
		for (auto system : m_v_SystemList)
		{
			if (auto correctSystem = dynamic_cast<SystemType*>(system))
			{
				correctSystem->m_b_isActive = false;
				DisableAllSystemComponents<ComponentType>(type);
				return;
			}
		}
	}

	template <typename SystemType, typename ComponentType, Component type>
	void EnableSystem()
	{
		for (auto system : m_v_SystemList)
		{
			if (auto correctSystem = dynamic_cast<SystemType*>(system))
			{
				correctSystem->m_b_isActive = true;
				EnableAllSystemComponents<ComponentType>(type);
				return;
			}
		}
	}


	//TODO
	int m_i_DroppedFrames;
};

#endif
