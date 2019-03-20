
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, 

#include "Component.h"
#include "Entity.h"
#include "ComponentList.h"
#include "EntityList.h"
#include <functional>


class ComponentManager
{
	std::vector<BaseComponent*> m_v_ComponentPool = {};
	void AddComponent(BaseComponent* componentPointer, Component componentType);
	BaseComponent* NewComponentReroute(BaseEntity* entityPointer, Component componentType);
	BaseComponent* GetFirstComponentInstanceReroute(Component componentType);
	BaseComponent* GetSpecificComponentInstanceReroute(BaseEntity* entityPointer, Component componentType);
	BaseComponent* GetSpecificComponentInstanceReroute(BaseComponent* componentPointer, Component componentType);

public:
	ComponentManager();
	template <typename ComponentType>
	void Each(std::function<void(ComponentType*)> function, Component componentType, bool skipActive = false)
	{
		for (ComponentType* component = GetFirstComponentInstance<ComponentType>(componentType);
			  component != nullptr;
			  component = static_cast<ComponentType*>(component->m_po_NextComponent))
		{
			if (skipActive) {
				if (!component->m_b_IsActive || !component->m_po_OwnerEntity->m_b_IsActive)
				{
					continue;
				}
			}
			function(component);
		}
	}

	template <class T>
	T* NewComponent(BaseEntity* entityPointer, Component componentType)
	{
		return static_cast<T*>(NewComponentReroute(entityPointer, componentType));
	}
	template <class T>
	T* GetFirstComponentInstance(Component componentType)
	{
		return static_cast<T*>(GetFirstComponentInstanceReroute(componentType));
	}
	template <class T>
	T* GetSpecificComponentInstance(BaseEntity* entityPointer, Component componentType)
	{
		return static_cast<T*>(GetSpecificComponentInstanceReroute(entityPointer, componentType));
	}
	template <class T>
	T* GetSpecificComponentInstance(BaseComponent* componentPointer, Component componentType)
	{
		return static_cast<T*>(GetSpecificComponentInstanceReroute(componentPointer, componentType));
	}

	void DeleteComponent(BaseEntity* entityPointer, Component componentType);
	void DeleteComponent(BaseComponent* componentPointer);
};

#endif
