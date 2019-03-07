
#include "EntityList.h"

BaseEntity::BaseEntity(const char* entityName)
{
	char* charPointer = m_pc_EntityName = (char*)malloc(strlen(entityName) + 1);

	while (*entityName)
	{
		*charPointer = *entityName;
		charPointer++, entityName++;
	}

	*charPointer = 0;
}

Entity BaseEntity::GetEntityID()
{
	return m_x_EntityID;
}

void BaseEntity::SetEntityID(Entity type)
{
	m_x_EntityID = type;
}


