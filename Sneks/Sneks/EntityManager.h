
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, each entity has it's own pool of components sorted with vector, 

#include "Entity.h"
#include "Component.h"
#include "EntityList.h"
#include "ComponentList.h"
#include "ComponentManager.h"

class EntityManager
{
	std::vector<BaseEntity*> entitypool;
	void addEntity(BaseEntity* eptr, Entities ent);
	void attachAllComponents(BaseEntity* eptr, Entities ent);

	public:
		EntityManager();
		ComponentManager *comMan = new ComponentManager;
		BaseEntity* newEntity(Entities ent);
		void deleteEntity(BaseComponent* com);
		void deleteEntity(BaseEntity* ent);
};

#endif