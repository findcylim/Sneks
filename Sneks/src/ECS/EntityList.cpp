/* Start Header****************************************************************/
/*!
\file EntityList.cpp
\author Primary Author : Edwin Khoo, b.khoo
\par email: b.khoo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the definition for the base entity.

\par Contribution : Edwin  - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/


#include "EntityList.h"

BaseEntity::BaseEntity(const char* entityName)
{
	if (entityName)
		strcpy_s(m_pc_EntityName, entityName);
	//char* charPointer = m_pc_EntityName = (char*)malloc(strlen(entityName) + 1);

	//while (*entityName)
	//{
	//	*charPointer = *entityName;
	//	charPointer++, entityName++;
	//}

	//*charPointer = 0;
}

Entity BaseEntity::GetEntityID()
{
	return m_x_EntityID;
}

void BaseEntity::SetEntityID(Entity type)
{
	m_x_EntityID = type;
}


