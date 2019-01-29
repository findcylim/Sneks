
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, 

#include "Component.h"
#include "Entity.h"
#include "ComponentList.h"
#include "EntityList.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/PhysicsComponent.h"

class ComponentManager
{
	std::vector<BaseComponent*> m_v_ComponentPool;
	void AddComponent(BaseComponent* componentPointer, Component componentType);

	public:
		ComponentManager();
		BaseComponent* NewComponent(BaseEntity* entityPointer, Component componentType);
		void DeleteComponent(BaseEntity* entityPointer, Component componentType);
		void DeleteComponent(BaseComponent* componentPointer);
		BaseComponent* GetFirstComponentInstance(Component componentType);
		BaseComponent* GetSpecificComponentInstance(BaseEntity* entityPointer, Component componentType);
		BaseComponent* GetSpecificComponentInstance(BaseComponent* componentPointer, Component componentType);
};

#endif