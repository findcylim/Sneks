
#ifndef ENTITY_LIST_H
#define ENTITY_LIST_H

//place to store all the entities(?)
//holds all the pools and memory for entities, making it the only "list" containing info on entities
//holds the base class
// all entities will be attached 


//possibly the system will not be touching the entities at all
//if entities are to be touched, the entity might need an object tag id
//otherwise the node system for it might not be needed

#include <cstdlib>
#include <vector>

#include "Entity.h"
#include "Component.h"

class BaseEntity
{
	public:
		Entities entityID = Entities::BaseE;
		bool active = true;
		std::vector<BaseComponent*> coe;
		BaseEntity* prev = NULL, *next = NULL;
};

class sampleEntity : BaseEntity
{
	public:
		Components basicComponents[2] = { Components::SampleC , Components::EndC };
};

#endif
