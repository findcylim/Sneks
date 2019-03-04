
#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include <cstdlib>

#include "Component.h"
#include "Entity.h"
class BaseComponent
{
	virtual BaseComponent* _G() { return this; };
	public:
		Component m_x_ComponentID = Component::kComponentBase;
		bool m_b_IsActive = true;
		BaseEntity* m_po_OwnerEntity = nullptr;
		BaseComponent* m_po_PrevComponent = nullptr, *m_po_NextComponent = nullptr;
};

#endif