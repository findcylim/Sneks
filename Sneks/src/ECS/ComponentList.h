
#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include <cstdlib>
#include "EntityList.h"
#include "Component.h"
#include "Entity.h"
class BaseComponent
{
	virtual BaseComponent* _G() { return this; };
	public:
		virtual ~BaseComponent() = default;
		Component m_x_ComponentID = Component::kComponentBase;
		bool m_b_IsActive = true;
		BaseEntity* m_po_OwnerEntity = nullptr;
		BaseComponent* m_po_PrevComponent = nullptr, *m_po_NextComponent = nullptr;

		template<typename T>
		T* GetComponent()
		{
			return m_po_OwnerEntity->GetComponent<T>();
		}
};

class SampleComponent : BaseComponent
{
	public:
};

#endif