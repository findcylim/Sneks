
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, each entity has it's own pool of components sorted with vector, 
#include "../Utility/MemoryAllocator.h"
#include "Entity.h"
#include "Component.h"
#include "EntityList.h"
#include "ComponentList.h"
#include "ComponentManager.h"

class EntityManager
{
	std::vector<BaseEntity*> m_v_EntityPool;
	ComponentManager *m_po_ComponentManagerInstance;
	std::vector<BaseEntity*> m_v_ToDelete;

	void AddEntity(BaseEntity* entityPointer, Entity entityType);
	void AttachAllComponents(BaseEntity* entityPointer, Entity entityType);
	void DeleteEntity(BaseEntity* entityPointer);
	void DeleteEntity(BaseComponent* componentPointer);

	BaseEntity* NewEntityReroute(Entity entityType, const char* entityName);
	BaseEntity* GetFirstEntityInstanceReroute(Entity entityType);
	BaseEntity* GetSpecificEntityInstanceReroute(Entity entityType, const char* entityName);
	BaseEntity* GetSpecificEntityInstanceReroute(BaseComponent* componentPointer);


public:

	EntityManager();
	~EntityManager();

	template <class T>
	T* NewEntity(Entity entityType, const char* entityName)
	{
		return static_cast<T*>(static_cast<void*>(NewEntityReroute(entityType, entityName)));
	}
	template <class T>
	T* GetFirstEntityInstance(Entity entityType)
	{
		return static_cast<T*>(static_cast<void*>(GetFirstEntityInstanceReroute(entityType)));
	}
	template <class T>
	T* GetSpecificEntityInstance(Entity entityType, const char* entityName)
	{
		return static_cast<T*>(static_cast<void*>(GetSpecificEntityInstanceReroute(entityType, entityName)));
	}
	template <class T>
	T* GetSpecificEntityInstance(BaseComponent* componentPointer)
	{
		return static_cast<T*>(static_cast<void*>(GetSpecificEntityInstanceReroute(componentPointer)));
	}
	
	ComponentManager* GetComponentManager() const;
	void AddToDeleteQueue(BaseEntity* entityPointer);
	void ResolveDeletes();
};

#endif