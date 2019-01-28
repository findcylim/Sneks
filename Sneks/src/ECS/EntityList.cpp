
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
