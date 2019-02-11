
#include <vector>
#include <string>
#include <iostream>

#include "EntityManager.h"

void checkName(BaseEntity* entityPointerSource, BaseEntity* entityPointerUntouched)
{
	char *charPointerUntouched = entityPointerUntouched->m_pc_EntityName, *charPointerSource = entityPointerSource->m_pc_EntityName;
	while (*charPointerUntouched == *charPointerSource)
	{
		if (!(*charPointerUntouched))
		{
			free(entityPointerSource->m_pc_EntityName);
			entityPointerSource->m_pc_EntityName = (char*)malloc(strlen(entityPointerUntouched->m_pc_EntityName) + 4);

			charPointerUntouched = entityPointerUntouched->m_pc_EntityName, charPointerSource = entityPointerSource->m_pc_EntityName;
			while (*charPointerUntouched)
			{
				*charPointerSource = *charPointerUntouched;
				charPointerSource++, charPointerUntouched++;
			}
			*charPointerSource = '(';
			charPointerSource++;
			*charPointerSource = '1';
			charPointerSource++;
			*charPointerSource = ')';
			charPointerSource++;
			*charPointerSource = 0;
		}

		charPointerSource++, charPointerUntouched++;
	}
}

EntityManager::EntityManager()
{
	for (int i_iter = 0; i_iter < Entity::kEntityEnd; i_iter++)
		m_v_EntityPool.push_back(nullptr);
}

ComponentManager* EntityManager::GetComponentManager() const
{
	return m_po_ComponentManagerInstance;
}

void EntityManager::AddEntity(BaseEntity* entityPointer, Entity entityType)
{
	if (entityPointer)
	{
		BaseEntity* prevEntity = m_v_EntityPool[entityType];

		if (prevEntity)
		{
			while (prevEntity->m_po_NextEntity)
			{
				//checkName(entityPointer, prevEntity); //NEEDS TO BE FIXED TODO
				prevEntity = prevEntity->m_po_NextEntity;
			}

			//checkName(entityPointer, prevEntity);

			prevEntity->m_po_NextEntity = entityPointer;
			entityPointer->m_po_PrevEntiy = prevEntity;
		}
		else
			m_v_EntityPool[entityType] = entityPointer;

		AttachAllComponents(entityPointer, entityType);
	}
}

void EntityManager::AttachAllComponents(BaseEntity* entityPointer, Entity entityType)
{
	if (entityPointer)
	{
		Component* componentPointer = nullptr;
		switch (entityType)
		{
		case Entity::kEntitySample:
			componentPointer = ((SampleEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityStaticObject:
			componentPointer = ((StaticObjectEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityBackground:
			componentPointer = ((BackgroundEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityCamera:
			componentPointer = ((CameraEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntitySnekHead:
			componentPointer = ((SnekHeadEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntitySnekBody:
			componentPointer = ((SnekBodyEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntitySnekTail:
			componentPointer = ((SnekTailEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityMoon:
			componentPointer = ((MoonEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityProjectile:
			componentPointer = ((ProjectileEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		}

		if (componentPointer)
		{
			while (*componentPointer != Component::kComponentEnd)
			{
				m_po_ComponentManagerInstance->NewComponent(entityPointer, *componentPointer);
				componentPointer++;
			}
		}
	}
}

BaseEntity* EntityManager::NewEntity(Entity entityType, const char* entityName)
{
	BaseEntity* entityPointer = nullptr;
	switch (entityType)
	{
		case Entity::kEntityBase:
			entityPointer = new BaseEntity(entityName);
			break;

		case Entity::kEntitySample:
			entityPointer = (BaseEntity*)new SampleEntity(entityName);
			break;

		case Entity::kEntityStaticObject:
			entityPointer = (BaseEntity*)new StaticObjectEntity(entityName);
			break;

		case Entity::kEntityBackground:
			entityPointer = (BaseEntity*)new BackgroundEntity(entityName);
			break;

		case Entity::kEntityCamera:
			entityPointer = (BaseEntity*)new CameraEntity(entityName);
			break;
		case kEntitySnekHead: 
			entityPointer = (BaseEntity*)new SnekHeadEntity(entityName);
			break;
		case kEntitySnekBody: 
			entityPointer = (BaseEntity*)new SnekBodyEntity(entityName);
			break;
		case kEntitySnekTail:
			entityPointer = (BaseEntity*)new SnekTailEntity(entityName);
			break;
		case kEntityMoon:
			entityPointer = (BaseEntity*)new MoonEntity(entityName);
			break;
		case kEntityProjectile:
			entityPointer = (BaseEntity*)new ProjectileEntity(entityName);
			break;
	}

	if (entityPointer)
	{
		entityPointer->m_x_EntityID = entityType;
		AddEntity(entityPointer, entityType);
	}

	return entityPointer;
}

void EntityManager::DeleteEntity(BaseComponent* componentPointer)
{
	if(componentPointer)
		DeleteEntity(componentPointer->m_po_OwnerEntity);
}

void EntityManager::AddToDeleteQueue(BaseEntity* entityPointer)
{
	m_v_ToDelete.push_back(entityPointer);
}

void EntityManager::ResolveDeletes()
{
	for (auto entity : m_v_ToDelete)
	{
		DeleteEntity(entity);
	}
	m_v_ToDelete.clear();
}

void EntityManager::DeleteEntity(BaseEntity* entityPointer)
{
	if (entityPointer)
	{
		BaseEntity *prevEntity = entityPointer->m_po_PrevEntiy, *nextEntity = entityPointer->m_po_NextEntity;

		while (entityPointer->m_v_AttachedComponentsList.size() > 0)
			m_po_ComponentManagerInstance->DeleteComponent(entityPointer->m_v_AttachedComponentsList[0]);

		if (prevEntity && nextEntity)
		{
			prevEntity->m_po_NextEntity = nextEntity;
			nextEntity->m_po_PrevEntiy = prevEntity;
		}
		else if (nextEntity && !prevEntity)
		{
			m_v_EntityPool[entityPointer->m_x_EntityID] = nextEntity;
			nextEntity->m_po_PrevEntiy = nullptr;
		}
		else if (prevEntity && !nextEntity)
			prevEntity->m_po_NextEntity = nullptr;
		else if (!prevEntity && !nextEntity)
			m_v_EntityPool[entityPointer->m_x_EntityID] = nullptr;

		if (entityPointer->m_pc_EntityName)
			free(entityPointer->m_pc_EntityName);

		delete entityPointer;
	}
}

BaseEntity* EntityManager::GetFirstEntityInstance(Entity entityType)
{
	return m_v_EntityPool[entityType];
}

BaseEntity* EntityManager::GetSpecificEntityInstance(Entity entityType, const char* entityName)
{
	BaseEntity* entityPointer = m_v_EntityPool[entityType];

	while (entityPointer)
	{
		if (entityPointer->m_pc_EntityName == entityName)
			break;

		entityPointer = entityPointer->m_po_NextEntity;
	}

	return entityPointer;
}

BaseEntity* EntityManager::GetSpecificEntityInstance(BaseComponent* componentPointer)
{
	return componentPointer->m_po_OwnerEntity;
}
