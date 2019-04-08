/* Start Header****************************************************************/
/*!
\file EntityManager.cpp
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for management of entities.

\par Contribution : CY     - 21.58% (Entity List & Delete Queue)
\par Contribution : Javier - 7.19%  (Entity List)
\par Contribution : Edwin  - 64.03% (Structure and management of entities)
\par Contribution : Adam   - 7.19%  (Entity List)

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "EntityManager.h"

void checkName(BaseEntity* entityPointerSource, BaseEntity* entityPointerUntouched)
{
	char *charPointerUntouched = entityPointerUntouched->m_pc_EntityName, *charPointerSource = entityPointerSource->m_pc_EntityName;
	while (*charPointerUntouched == *charPointerSource)
	{
		if (!(*charPointerUntouched))
		{
			//delete(entityPointerSource->m_pc_EntityName);
			//entityPointerSource->m_pc_EntityName = new char [(strlen(entityPointerUntouched->m_pc_EntityName) + 4)];

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
	m_po_ComponentManagerInstance = new ComponentManager;
	for (int i_iter = 0; i_iter < Entity::kEntityEnd; i_iter++)
		m_v_EntityPool.push_back(nullptr);
}

EntityManager::~EntityManager()
{
	while (!m_v_EntityPool.empty())
	{
		auto toDelete = m_v_EntityPool.back();
		if (!toDelete)
		{
			m_v_EntityPool.pop_back();
			continue;
		}
		BaseEntity* i_EntityType = toDelete->m_po_NextEntity;
		while (i_EntityType)
		{
			auto nextNode = i_EntityType->m_po_NextEntity;
			DeleteEntity(i_EntityType);
			i_EntityType = nextNode;
		}
		DeleteEntity(toDelete);
		m_v_EntityPool.pop_back();
	}
	delete m_po_ComponentManagerInstance;
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
//TODO
// TEMPLATE THIS 
void EntityManager::AttachAllComponents(BaseEntity* entityPointer, Entity entityType)
{
	if (entityPointer)
	{
		Component* componentPointer = nullptr;
		switch (entityType)
		{
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

		case Entity::kEntityParticleEffect:
			componentPointer = ((ParticleSpawnerEntity*)entityPointer)->m_ax_InitialComponents;
			break;

		case Entity::kEntityParticle:
			componentPointer = ((ParticleEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityCanvas:
			componentPointer = ((CanvasEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityCanvasBasicSprite:
			componentPointer = ((CanvasBasicSpriteEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityCanvasButton:
			componentPointer = ((CanvasButtonEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityCanvasTextLabel:
			componentPointer = ((CanvasTextLabelEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		case Entity::kEntityMouse:
			componentPointer = ((MouseEntity*)entityPointer)->m_ax_InitialComponents;
			break;

		case Entity::kEntityPowerUpHolder:
			componentPointer = ((PowerUpHolderEntity*)entityPointer)->m_ax_InitialComponents;
			break;

		case Entity::kEntityScreenOverlay:
			componentPointer = ((ScreenOverlayEntity*)entityPointer)->m_ax_InitialComponents;
			break;
		}

		if (componentPointer)
		{
			while (*componentPointer != Component::kComponentEnd)
			{
				m_po_ComponentManagerInstance->NewComponent<BaseComponent>(entityPointer, *componentPointer);
				componentPointer++;
			}
		}
	}
}

BaseEntity* EntityManager::NewEntityReroute(Entity entityType, const char* entityName)
{
	BaseEntity* entityPointer = nullptr;
	switch (entityType)
	{
		case Entity::kEntityBase:
			entityPointer = new BaseEntity(entityName);
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
		
		case kEntityParticleEffect:
			entityPointer = (BaseEntity*)new ParticleSpawnerEntity(entityName);
			break;
		
		case kEntityParticle:
			entityPointer = (BaseEntity*)new ParticleEntity(entityName);
			break;
		case kEntityCanvas:
			entityPointer = (BaseEntity*)new CanvasEntity(entityName);
			break;
		case kEntityCanvasBasicSprite:
			entityPointer = (BaseEntity*)new CanvasBasicSpriteEntity(entityName);
			break;
		case kEntityCanvasButton:
			entityPointer = (BaseEntity*)new CanvasButtonEntity(entityName);
			break;
		case kEntityCanvasTextLabel:
			entityPointer = (BaseEntity*)new CanvasTextLabelEntity(entityName);
			break;
		case kEntityMouse:
			entityPointer = (BaseEntity*)new MouseEntity(entityName);
			break;
		
		case kEntityPowerUpHolder:
			entityPointer = (BaseEntity*)new PowerUpHolderEntity(entityName);
			break;
		
		case kEntityScreenOverlay:
			entityPointer = (BaseEntity*)new ScreenOverlayEntity(entityName);
			break;
	}

	if (entityPointer)
	{
		entityPointer->m_po_EntityManager = this;
		entityPointer->SetEntityID(entityType);
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
	entityPointer->m_b_IsActive = false;
	for (auto comp : entityPointer->m_v_AttachedComponentsList)
		comp->m_b_IsActive = false;

	if(entityPointer)
		m_v_ToDelete.push_back(entityPointer);
}

void EntityManager::ResolveDeletes()
{
	for (auto entity : m_v_ToDelete)
	{
		entity->m_b_IsActive = false;
		DeleteEntity(entity);
	}
	m_v_ToDelete.clear();
}

void EntityManager::  DeleteEntity(BaseEntity* entityPointer)
{
	if (entityPointer)
	{
		BaseEntity *prevEntity = entityPointer->m_po_PrevEntiy, *nextEntity = entityPointer->m_po_NextEntity;

		while (!entityPointer->m_v_AttachedComponentsList.empty())
			m_po_ComponentManagerInstance->DeleteComponent(entityPointer->m_v_AttachedComponentsList[0]);

		if (prevEntity && nextEntity)
		{
			//You probably added the same entity more than to delete
			prevEntity->m_po_NextEntity = nextEntity;
			nextEntity->m_po_PrevEntiy = prevEntity;
		}
		else if (nextEntity && !prevEntity)
		{
			m_v_EntityPool[entityPointer->GetEntityID()] = nextEntity;
			nextEntity->m_po_PrevEntiy = nullptr;
		}
		else if (prevEntity && !nextEntity)
			prevEntity->m_po_NextEntity = nullptr;
		else if (!prevEntity && !nextEntity)
			m_v_EntityPool[entityPointer->GetEntityID()] = nullptr;
		//m_v_ToFree.push_back(entityPointer);

		delete entityPointer;
		entityPointer = nullptr;
	}
}

BaseEntity* EntityManager::GetFirstEntityInstanceReroute(Entity entityType)
{
	return m_v_EntityPool[entityType];
}

BaseEntity* EntityManager::GetSpecificEntityInstanceReroute(Entity entityType, const char* entityName)
{
	BaseEntity* entityPointer = m_v_EntityPool[entityType];

	while (entityPointer)
	{
		if (strcmp(entityPointer->m_pc_EntityName , entityName)==0)
			break;

		entityPointer = entityPointer->m_po_NextEntity;
	}

	return entityPointer;
}

BaseEntity* EntityManager::GetSpecificEntityInstanceReroute(BaseComponent* componentPointer)
{
	return componentPointer->m_po_OwnerEntity;
}
