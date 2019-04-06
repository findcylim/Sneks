/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


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
	std::vector<BaseEntity*> m_v_ToFree;


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
		return static_cast<T*>(NewEntityReroute(entityType, entityName));
	}
	template <class T>
	T* GetFirstEntityInstance(Entity entityType)
	{
		return static_cast<T*>(GetFirstEntityInstanceReroute(entityType));
	}
	template <class T>
	T* GetSpecificEntityInstance(Entity entityType, const char* entityName)
	{
		return static_cast<T*>(GetSpecificEntityInstanceReroute(entityType, entityName));
	}
	template <class T>
	T* GetSpecificEntityInstance(BaseComponent* componentPointer)
	{
		return static_cast<T*>(GetSpecificEntityInstanceReroute(componentPointer));
	}

	template <typename EntityType, Entity kEntity>
	void DisableAllEntityType()
	{
		auto firstEnt = GetFirstEntityInstance<EntityType>(kEntity);
		while (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = false;
			}
			firstEnt->m_b_IsActive = false;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}

	template <typename EntityType, Entity kEntity>
	void EnableAllEntityType()
	{
		auto firstEnt = GetFirstEntityInstance<EntityType>(kEntity);
		while (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = true;
			}
			firstEnt->m_b_IsActive = true;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}

	template <typename EntityType, Entity kEntity>
	void DisableSpecificEntityType(const char * name)
	{
		auto firstEnt = GetSpecificEntityInstance<EntityType>(kEntity,name);
		while (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = false;
			}
			firstEnt->m_b_IsActive = false;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}

	template <typename EntityType, Entity kEntity>
	void DisableSpecificEntity(const char * name)
	{
		auto firstEnt = GetSpecificEntityInstance<EntityType>(kEntity, name);
		if (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = false;
			}
			firstEnt->m_b_IsActive = false;
		}
	}

	template <typename EntityType, Entity kEntity>
	void EnableSpecificEntity(const char * name)
	{
		auto firstEnt = GetSpecificEntityInstance<EntityType>(kEntity, name);
		if (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = true;
			}
			firstEnt->m_b_IsActive = true;
		}
	}

	template <typename EntityType, Entity kEntity>
	void EnableSpecificEntityType(const char * name)
	{
		auto firstEnt = GetSpecificEntityInstance<EntityType>(kEntity,name);
		while (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = true;
			}
			firstEnt->m_b_IsActive = true;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}

	void DisableSpecificEntityType(BaseComponent * comp)
	{
		auto firstEnt = comp->m_po_OwnerEntity;
		if (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = false;
			}
			firstEnt->m_b_IsActive = false;
		}
		firstEnt->m_b_IsActive = false;
	}


	void EnableSpecificEntityType(BaseComponent * comp)
	{
		auto firstEnt = comp->m_po_OwnerEntity;
		if (firstEnt)
		{
			for (auto componentIterator : firstEnt->m_v_AttachedComponentsList)
			{
				componentIterator->m_b_IsActive = true;
			}
			firstEnt->m_b_IsActive = true;
		}
	}


	template <typename EntityType,Entity kEntity, typename ComponentType>
	void DisableComponentsFromEntityType()
	{
		auto firstEnt = GetFirstEntityInstance<EntityType>(kEntity);
		while (firstEnt)
		{
			firstEnt->GetComponent<ComponentType>()->m_b_IsActive = false;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}

	template <typename EntityType, Entity kEntity, typename ComponentType>
	void EnableComponentsFromEntityType()
	{
		auto firstEnt = GetFirstEntityInstance<EntityType>(kEntity);
		while (firstEnt)
		{
			firstEnt->GetComponent<ComponentType>()->m_b_IsActive = true;
			firstEnt = static_cast<EntityType*>(firstEnt->m_po_NextEntity);
		}
	}
	
	ComponentManager* GetComponentManager() const;
	void AddToDeleteQueue(BaseEntity* entityPointer);
	void ResolveDeletes();
};

#endif