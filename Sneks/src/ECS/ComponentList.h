/* Start Header****************************************************************/
/*!
\file ComponentList.h
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


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

#endif