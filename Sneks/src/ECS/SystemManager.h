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




public:
	size_t GetSystemCount() const;
	void AddSystem(BaseSystem* NewSystem);
	void RemoveSystem(BaseSystem* RemSystem);
	void Update(float dt);
	//BaseSystem* GetSystem(int ID); 
	//BaseSystem* GetSystem(const char * systemName);
	void Initialize(EventManager* eventManager, EntityManager* entManager);
	SystemManager(Logger* logger);
	~SystemManager();

	template <typename SystemType>
	SystemType* GetSystem(const char* Name)
	{
		for (BaseSystem* currSystem : m_v_SystemList)
		{
			if (currSystem->GetName() == Name)
			{
				return static_cast<SystemType*>(currSystem);
			}
		}
		return nullptr;
	}

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
