
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

void EntityManager::AddEntity(BaseEntity* entityPointer, Entity entityType)
{
	if (entityPointer)
	{
		BaseEntity* prevEntity = m_v_EntityPool[entityType];

		if (prevEntity)
		{
			while (prevEntity->m_po_NextEntity)
			{
				checkName(entityPointer, prevEntity);
				prevEntity = prevEntity->m_po_NextEntity;
			}

			checkName(entityPointer, prevEntity);

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