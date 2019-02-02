
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
#include <cstring> //memcpy
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

class SnekHeadEntity : BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ Component::kComponentTransform, Component::kComponentDraw, Component::kComponentPhysics , Component::kComponentCollision };

	SnekHeadEntity(const char* entityName) : BaseEntity(entityName) {};
};

class SnekBodyEntity : BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ Component::kComponentTransform, Component::kComponentDraw, Component::kComponentPhysics , Component::kComponentCollision };

	SnekBodyEntity(const char* entityName) : BaseEntity(entityName) {};
};

class StaticObjectEntity : BaseEntity
{
public:
	Component m_ax_InitialComponents[3] ={ Component::kComponentTransform, Component::kComponentDraw , Component::kComponentCollision };

	StaticObjectEntity(const char* entityName) : BaseEntity(entityName) {};
};

class BackgroundEntity : BaseEntity
{
public:
	Component m_ax_InitialComponents[2] ={ Component::kComponentTransform, Component::kComponentDraw };

	BackgroundEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CameraEntity : BaseEntity
{
public:
	Component m_ax_InitialComponents[2] ={ Component::kComponentTransform , Component::kComponentCamera };

	CameraEntity(const char* entityName) : BaseEntity(entityName) {};
	
};


#endif
