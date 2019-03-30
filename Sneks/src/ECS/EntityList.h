
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
#include "../Utility/RTTIHelper.h"
class EntityManager;

class BaseEntity
{
		Entity m_x_EntityID = Entity::kEntityBase;

	public:
		char m_pc_EntityName[100];
		bool m_b_IsActive = true;
		std::vector<BaseComponent*> m_v_AttachedComponentsList;
		EntityManager* m_po_EntityManager;
		BaseEntity* m_po_PrevEntiy = nullptr, *m_po_NextEntity = nullptr;

		BaseEntity(const char* entityName);

		template<typename T>
		auto GetComponent()
		{
			for (auto& component : m_v_AttachedComponentsList)
			{
				if (T* p = dynamic_cast<T*>(component))
				{
					return static_cast<T*>(component);
				}
			}
			return static_cast<T*>(nullptr);
		}

		///Checks if entity of is entityType
		/// if it is not, return nullptr
		template<typename T>
		auto Is(T entityType)
		{
			return dynamic_cast<T*>(this);
		}

		//template<typename T>
		//T* Has(T componentType)
		//{
		//	for (auto& component : m_v_AttachedComponentsList)
		//	{
		//		if (T* p = dynamic_cast<T*>(component))
		//		{
		//			return static_cast<T*>(component);
		//		}
		//	}
		//	return nullptr;
		//}

		/*int GetCount()
		{
			BaseEntity* current = this;
			int counter = 0;
			while(current)
			{
				++counter;
				current = current->m_po_NextEntity;
			}
			return counter;
		}*/

		Entity GetEntityID();
		void SetEntityID(Entity type);
};

class SnekHeadEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[10] ={ Component::kComponentTransform, Component::kComponentDraw, Component::kComponentAnimation, Component::kComponentPhysics , Component::kComponentSnekHead,
		Component::KComponentInvulnerable, Component::kComponentCollision, Component::kComponentPowerUp, Component::kComponentBloom, Component::kComponentEnd };

	SnekHeadEntity(const char* entityName) : BaseEntity(entityName) {};
};

class SnekBodyEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[8] ={ Component::kComponentTransform, Component::kComponentDraw, Component::kComponentPhysics , Component::KComponentInvulnerable,
		Component::kComponentCollision, Component::kComponentFollow, Component::kComponentBloom, Component::kComponentEnd };

	SnekBodyEntity(const char* entityName) : BaseEntity(entityName) {};
};

class SnekTailEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[8] = { Component::kComponentTransform, Component::kComponentDraw, Component::kComponentPhysics , Component::KComponentInvulnerable,
		Component::kComponentCollision, Component::kComponentFollow, Component::kComponentBloom, Component::kComponentEnd };

	SnekTailEntity(const char* entityName) : BaseEntity(entityName) {};
};

class MoonEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { Component::kComponentTransform, Component::kComponentDraw,
		Component::kComponentPhysics , Component::KComponentInvulnerable, Component::kComponentCollision, Component::kComponentEnd };

	MoonEntity(const char* entityName) : BaseEntity(entityName) {};
};

class StaticObjectEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ Component::kComponentTransform, Component::kComponentDraw, Component::kComponentCollision,
		Component::kComponentEnd };

	StaticObjectEntity(const char* entityName) : BaseEntity(entityName) {};
};

class BackgroundEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[3] ={ Component::kComponentTransform, Component::kComponentDraw  , Component::kComponentEnd };

	BackgroundEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CameraEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[3] ={ Component::kComponentTransform , Component::kComponentCamera , Component::kComponentEnd };

	CameraEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ProjectileEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { Component::kComponentTransform , Component::kComponentDraw, 
											Component::kComponentPhysics,	 Component::kComponentCollision,
											Component::kComponentEnd };

	ProjectileEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ParticleEffectEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { Component::kComponentParticleEffect, Component::kComponentEnd };

	ParticleEffectEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ParticleEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { Component::kComponentTransform , Component::kComponentDraw, Component::kComponentPhysics, 
											Component::kComponentParticle, Component::kComponentAnimation, Component::kComponentEnd };

	ParticleEntity(const char* entityName) : BaseEntity(entityName) {};
};

class PowerUpHolderEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[8] = { Component::kComponentTransform , Component::kComponentDraw, Component::kComponentPhysics, 
											Component::kComponentCollision, Component::KComponentInvulnerable, Component::kComponentBloom, kComponentPowerUpHolder, Component::kComponentEnd };

	PowerUpHolderEntity(const char* entityName) : BaseEntity(entityName) {};
};


class CanvasEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { Component::kComponentTransform , Component::kComponentCanvas, Component::kComponentEnd };

	CanvasEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasButtonEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { Component::kComponentTransform , Component::kComponentDraw , Component::kComponentCollision,Component::kComponentCanvasElement,Component::kComponentTextRenderer, Component::kComponentEnd };

	CanvasButtonEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasBasicSpriteEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { Component::kComponentTransform , Component::kComponentDraw, Component::kComponentCanvasElement,
											Component::kComponentTextRenderer, Component::kComponentEnd };

	CanvasBasicSpriteEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasTextLabelEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { Component::kComponentTransform , Component::kComponentDraw, Component::kComponentCanvasElement,
											Component::kComponentTextRenderer, Component::kComponentEnd };

	CanvasTextLabelEntity(const char* entityName) : BaseEntity(entityName) {};
};

class MouseEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] = { Component::kComponentTransform ,Component::kComponentDraw ,Component::kComponentCollision, Component::kComponentEnd };

	MouseEntity(const char* entityName) : BaseEntity(entityName) {};
};

class GrowthPickupEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ Component::kComponentTransform ,Component::kComponentDraw, Component::kComponentEnd };

	GrowthPickupEntity(const char* entityName) : BaseEntity(entityName) {};
};

#endif
