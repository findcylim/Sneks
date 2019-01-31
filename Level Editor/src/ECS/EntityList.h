
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
		Entity m_x_EntityID = Entity::kEntityBase;
		char* m_pc_EntityName = nullptr;
		bool m_b_IsActive = true;
		std::vector<BaseComponent*> m_v_AttachedComponentsList;
		BaseEntity* m_po_PrevEntiy = nullptr, *m_po_NextEntity = nullptr;

		BaseEntity(const char* entityName);
};

class SampleEntity : BaseEntity
{
	public:
		Component m_ax_InitialComponents[2] = { Component::kComponentSample , Component::kComponentEnd };

		SampleEntity(const char* entityName) : BaseEntity(entityName) {};
};

#endif
