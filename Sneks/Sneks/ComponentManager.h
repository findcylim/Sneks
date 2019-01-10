
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, 

#include "Component.h"
#include "Entity.h"
#include "ComponentList.h"
#include "EntityList.h"

class ComponentManager
{
	std::vector<BaseComponent*> componentpool;
	void addComponent(BaseComponent* cptr, Components com);

	public:
		ComponentManager();
		BaseComponent* newComponent(BaseEntity* ent, Components com);
		void deleteComponent(BaseEntity* ent, Components com);
		void deleteComponent(BaseComponent* com);
};

#endif