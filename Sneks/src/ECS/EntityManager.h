
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
	std::vector<BaseEntity*> m_v_EntityPool;
	void AddEntity(BaseEntity* entityPointer, Entity entityType);
	void AttachAllComponents(BaseEntity* entityPointer, Entity entityType);
	ComponentManager *m_po_ComponentManagerInstance = new ComponentManager;
	std::vector<BaseEntity*> m_v_ToDelete;
	void DeleteEntity(BaseEntity* entityPointer);
	void DeleteEntity(BaseComponent* componentPointer);


public:
	
	EntityManager();
	ComponentManager* GetComponentManager() const;
	BaseEntity* NewEntity(Entity entityType, const char* entityName);
	void AddToDeleteQueue(BaseEntity* entityPointer);
	void ResolveDeletes();
	BaseEntity* GetFirstEntityInstance(Entity entityType);
	BaseEntity* GetSpecificEntityInstance(Entity entityType, const char* entityName);
	BaseEntity* GetSpecificEntityInstance(BaseComponent* componentPointer);
};

#endif