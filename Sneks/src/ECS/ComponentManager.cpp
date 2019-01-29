//make functions to check and create new, as well as delete
//please use vector to store ptr, name can be gotten from the ptr itself

#include <vector>

#include "ComponentManager.h"


ComponentManager::ComponentManager()
{
	for (unsigned char i_iter = 0; i_iter < Component::kComponentEnd; i_iter++)
		m_v_ComponentPool.push_back(nullptr);
}

BaseComponent* ComponentManager::NewComponent(BaseEntity* entityPointer, Component componentType)
{
	BaseComponent* componentPointer = nullptr;

	if (entityPointer)
	{
		switch (componentType)
		{
		case Component::kComponentBase:
			componentPointer = new BaseComponent;
			break;
		case Component::kComponentSample:
			componentPointer = (BaseComponent*)new SampleComponent;
			break;
		case Component::kComponentTransform:
			componentPointer = (BaseComponent*)new TransformComponent;
			break;
		case Component::kComponentDraw:
			componentPointer = (BaseComponent*)new DrawComponent;
			break;
		case Component::kComponentPhysics:
			componentPointer = (BaseComponent*)new PhysicsComponent;
			break;
		}

		if (componentPointer)
		{
			componentPointer->m_x_ComponentID = componentType;
			componentPointer->m_po_OwnerEntity = entityPointer;
			entityPointer->m_v_AttachedComponentsList.push_back(componentPointer);
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

BaseComponent* ComponentManager::GetFirstComponentInstance(Component componentType)
{
	return m_v_ComponentPool[componentType];
}

BaseComponent* ComponentManager::GetSpecificComponentInstance(BaseEntity* entityPointer, Component componentType)
{
	for (unsigned i = 0; i < entityPointer->m_v_AttachedComponentsList.size(); i++)
		if (entityPointer->m_v_AttachedComponentsList[i]->m_x_ComponentID == componentType)
			return entityPointer->m_v_AttachedComponentsList[i];

	return nullptr;
}

BaseComponent* ComponentManager::GetSpecificComponentInstance(BaseComponent* componentPointer, Component componentType)
{
	return GetSpecificComponentInstance(componentPointer->m_po_OwnerEntity, componentType);
}
