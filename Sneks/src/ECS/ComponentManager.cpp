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

#include "ComponentManager.h"
#include "../Components/CollisionComponent.h"
#include "../Components/InvulnerableComponent.h"
#include "../Components/SnekHeadComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/DrawComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/FollowComponent.h"
#include "../Components/ParticleSpawnerComponent.h"
#include "../Components/ParticleComponent.h"
#include "../Components/CanvasComponent.h"
#include "../Components/TextRendererComponent.h"
#include "../Components/PowerUpComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BloomComponent.h"
#include "../Components/PowerUpHolderComponent.h"
#include "../Components/TintComponent.h"

ComponentManager::ComponentManager()
{
	for (unsigned char i_iter = 0; i_iter < Component::kComponentEnd; i_iter++)
		m_v_ComponentPool.push_back(nullptr);
}

BaseComponent* ComponentManager::NewComponentReroute(BaseEntity* entityPointer, Component componentType)
{
	BaseComponent* componentPointer = nullptr;

	if (entityPointer)
	{
		switch (componentType)
		{
		case Component::kComponentBase:
			componentPointer = new BaseComponent;
			break;
		case Component::kComponentTransform:
			componentPointer = static_cast<BaseComponent*>(new TransformComponent);
			break;
		case Component::kComponentDraw:
			{
			auto transformComponent = GetSpecificComponentInstance<TransformComponent>(
				entityPointer, kComponentTransform
			);
			auto drawComponent = new DrawComponent();
			drawComponent->m_po_TransformComponent = transformComponent;
			componentPointer = static_cast<BaseComponent*>(drawComponent);
			}
			break;
		case Component::kComponentPhysics:
		{
			auto transformComponent = GetSpecificComponentInstance<TransformComponent>(
				entityPointer, kComponentTransform
			);
			auto physicsComponent = new PhysicsComponent();
			physicsComponent->m_po_TransformComponent = transformComponent;
			componentPointer = static_cast<BaseComponent*>(physicsComponent);
		}
			break;
		case kComponentCamera: 
			componentPointer = static_cast<BaseComponent*>(new CameraComponent);
			break;
		case kComponentCollision: 
			{
			auto transformComponent = GetSpecificComponentInstance<TransformComponent>(
				entityPointer, kComponentTransform
			);
			auto drawComponent = GetSpecificComponentInstance<DrawComponent>(
				entityPointer, kComponentDraw
			);
			auto collisionComponent = new CollisionComponent();
			collisionComponent->m_po_DrawComponent = drawComponent;
			collisionComponent->m_po_DrawComponent->m_po_TransformComponent = transformComponent;
			componentPointer = static_cast<BaseComponent*>(collisionComponent);
			break;
			}
		case KComponentInvulnerable:
			componentPointer = static_cast<BaseComponent*>(new InvulnerableComponent);
			break;
		case kComponentSnekHead: 
			componentPointer = static_cast<BaseComponent*>(new SnekHeadComponent);
			break;
		case kComponentFollow:
			componentPointer = static_cast<BaseComponent*>(new FollowComponent);
			break;
		case kComponentParticleEffect:
			componentPointer = static_cast<BaseComponent*>(new ParticleSpawnerComponent);
			break;
		case kComponentParticle:
			componentPointer = static_cast<BaseComponent*>(new ParticleComponent);
			break;
		case kComponentCanvas:
			componentPointer = static_cast<BaseComponent*>(new CanvasComponent);
			static_cast<CanvasComponent*>(componentPointer)->m_po_ComponentManager = this;
			break;
		case kComponentCanvasElement:
			componentPointer = static_cast<BaseComponent*>(new CanvasElementComponent);
			break;
		case kComponentTextRenderer:
		{
			auto transformComponent = GetSpecificComponentInstance<TransformComponent>(
				entityPointer, kComponentTransform);
			componentPointer = static_cast<BaseComponent*>(new TextRendererComponent(transformComponent));
			break;
		}
		case kComponentPowerUp:
			componentPointer = static_cast<BaseComponent*>(new PowerUpComponent);
			break;
		case kComponentAnimation: 
			componentPointer = static_cast<BaseComponent*>(new AnimationComponent);
			break;
		case kComponentBloom: 
			componentPointer = static_cast<BaseComponent*>(new BloomComponent);
			break;
		case kComponentPowerUpHolder:
			componentPointer = static_cast<BaseComponent*>(new PowerUpHolderComponent);
			break;
		
		case kComponentTint:
			componentPointer = static_cast<BaseComponent*>(new TintComponent);
			break;

		}

		if (componentPointer)
		{
			componentPointer->m_x_ComponentID = componentType;
			componentPointer->m_po_OwnerEntity = entityPointer;
			entityPointer->m_v_AttachedComponentsList.push_back(componentPointer);
			AddComponent(componentPointer, componentType);
		}
	}

	return componentPointer;
}

void ComponentManager::AddComponent(BaseComponent* componentPointer, Component componentType)
{
	if (componentPointer)
	{
		BaseComponent* prevComponent = m_v_ComponentPool[componentType];

		if (prevComponent)
		{
			while (prevComponent->m_po_NextComponent)
				prevComponent = prevComponent->m_po_NextComponent;

			prevComponent->m_po_NextComponent = componentPointer;
			componentPointer->m_po_PrevComponent = prevComponent;
		}
		else
			m_v_ComponentPool[componentType] = componentPointer;
	}
}

void ComponentManager::DeleteComponent(BaseEntity* entityPointer, Component componentType)
{
	if (entityPointer)
	{
		for (unsigned i_vectorIndex = 0; i_vectorIndex < entityPointer->m_v_AttachedComponentsList.size(); i_vectorIndex++)
		{
			if (entityPointer->m_v_AttachedComponentsList[i_vectorIndex]->m_x_ComponentID == componentType)
				DeleteComponent(entityPointer->m_v_AttachedComponentsList[i_vectorIndex]);
		}
	}
}

void ComponentManager::DeleteComponent(BaseComponent* componentPointer)
{
	if (componentPointer)
	{
		BaseComponent *prevComponent = componentPointer->m_po_PrevComponent, *nextComponent = componentPointer->m_po_NextComponent;

		if (prevComponent && nextComponent)
		{
			prevComponent->m_po_NextComponent = nextComponent;
			nextComponent->m_po_PrevComponent = prevComponent;
		}
		else if (nextComponent && !prevComponent)
		{
			m_v_ComponentPool[componentPointer->m_x_ComponentID] = nextComponent;
			nextComponent->m_po_PrevComponent = nullptr;
		}
		else if (prevComponent && !nextComponent)
			prevComponent->m_po_NextComponent = nullptr;
		else if (!prevComponent && !nextComponent)
			m_v_ComponentPool[componentPointer->m_x_ComponentID] = nullptr;

		unsigned i_vectorIndex;
		for (i_vectorIndex = 0; componentPointer->m_po_OwnerEntity->m_v_AttachedComponentsList[i_vectorIndex] != componentPointer; i_vectorIndex++);

		componentPointer->m_po_OwnerEntity->m_v_AttachedComponentsList.erase(componentPointer->m_po_OwnerEntity->m_v_AttachedComponentsList.begin() + i_vectorIndex);
		
		delete componentPointer;
	}
}

BaseComponent* ComponentManager::GetFirstComponentInstanceReroute(Component componentType)
{
	return m_v_ComponentPool[componentType];
}

BaseComponent* ComponentManager::GetSpecificComponentInstanceReroute(BaseEntity* entityPointer, Component componentType)
{
	for (unsigned i = 0; i < entityPointer->m_v_AttachedComponentsList.size(); i++)
	{
		if (entityPointer->m_v_AttachedComponentsList[i]->m_x_ComponentID == componentType)
		{
			return entityPointer->m_v_AttachedComponentsList[i];
		}
	}

	return nullptr;
}

BaseComponent* ComponentManager::GetSpecificComponentInstanceReroute(BaseComponent* componentPointer, Component componentType)
{
	return GetSpecificComponentInstanceReroute(componentPointer->m_po_OwnerEntity, componentType);
}
