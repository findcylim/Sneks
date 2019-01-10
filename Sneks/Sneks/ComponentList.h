
#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

#include <cstdlib>

#include "Component.h"
#include "Entity.h"

class BaseComponent
{
	public:
		Components componentID = Components::BaseC;
		bool active = true;
		BaseEntity* ownerEntity = NULL;
		BaseComponent* prev = NULL, *next = NULL;
};

class sampleComponent : BaseComponent
{
	public:
};

#endif
