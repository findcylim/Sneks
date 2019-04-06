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