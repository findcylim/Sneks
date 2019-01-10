
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

//iterator by list, pool handled by vector, get and add by id and name, each entity has it's own pool of components sorted with vector, 

#include "Entity.h"
#include "Component.h"
#include "EntityList.h"
#include "ComponentList.h"

void emInit();
BaseEntity* newEntity(Entities ent);
void deleteEntity(BaseComponent* com);
void deleteEntity(BaseEntity* ent);

#endif