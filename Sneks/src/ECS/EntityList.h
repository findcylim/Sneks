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
		virtual ~BaseEntity() = default;
};

class SnekHeadEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[11] ={ kComponentTransform, kComponentDraw, kComponentAnimation, kComponentPhysics , kComponentSnekHead,
		KComponentInvulnerable, kComponentCollision, kComponentPowerUp, kComponentBloom, kComponentTint, kComponentEnd };

	SnekHeadEntity(const char* entityName) : BaseEntity(entityName) {};
};

class SnekBodyEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[9] ={ kComponentTransform, kComponentDraw, kComponentPhysics , KComponentInvulnerable,
		kComponentCollision, kComponentFollow, kComponentBloom, kComponentTint, kComponentEnd };

	SnekBodyEntity(const char* entityName) : BaseEntity(entityName) {};
};

class SnekTailEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[8] = { kComponentTransform, kComponentDraw, kComponentPhysics , KComponentInvulnerable,
		kComponentCollision, kComponentFollow, kComponentBloom, kComponentEnd };

	SnekTailEntity(const char* entityName) : BaseEntity(entityName) {};
};

class MoonEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { kComponentTransform, kComponentDraw,
		kComponentPhysics , KComponentInvulnerable, kComponentCollision, kComponentEnd };

	MoonEntity(const char* entityName) : BaseEntity(entityName) {};
};

class StaticObjectEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ kComponentTransform, kComponentDraw, kComponentCollision,
		kComponentEnd };

	StaticObjectEntity(const char* entityName) : BaseEntity(entityName) {};
};

class BackgroundEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[3] ={ kComponentTransform, kComponentDraw  , kComponentEnd };

	BackgroundEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CameraEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[3] ={ kComponentTransform , kComponentCamera , kComponentEnd };

	CameraEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ProjectileEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { kComponentTransform , kComponentDraw, 
											kComponentPhysics,	 kComponentCollision,
											kComponentEnd };

	ProjectileEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ParticleSpawnerEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { kComponentParticleEffect, kComponentEnd };

	ParticleSpawnerEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ParticleEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { kComponentTransform , kComponentDraw, kComponentPhysics, 
											kComponentParticle, kComponentAnimation, kComponentEnd };

	ParticleEntity(const char* entityName) : BaseEntity(entityName) {};
};

class PowerUpHolderEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[8] = { kComponentTransform , kComponentDraw, kComponentPhysics, 
											kComponentCollision, KComponentInvulnerable, kComponentBloom, kComponentPowerUpHolder, kComponentEnd };

	PowerUpHolderEntity(const char* entityName) : BaseEntity(entityName) {};
};


class CanvasEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { kComponentTransform , kComponentCanvas, kComponentEnd };

	CanvasEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasButtonEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[6] = { kComponentTransform , kComponentDraw , kComponentCollision,kComponentCanvasElement,kComponentTextRenderer, kComponentEnd };

	CanvasButtonEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasBasicSpriteEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[5] = { kComponentTransform , kComponentDraw, kComponentCanvasElement,
											kComponentTextRenderer, kComponentEnd };

	CanvasBasicSpriteEntity(const char* entityName) : BaseEntity(entityName) {};
};

class CanvasTextLabelEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] = { Component::kComponentTransform , Component::kComponentCanvasElement,
											Component::kComponentTextRenderer, Component::kComponentEnd };

	CanvasTextLabelEntity(const char* entityName) : BaseEntity(entityName) {};
};

class MouseEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] = { kComponentTransform ,kComponentDraw ,kComponentCollision, kComponentEnd };

	MouseEntity(const char* entityName) : BaseEntity(entityName) {};
};

class GrowthPickupEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ kComponentTransform ,kComponentDraw, kComponentEnd };

	GrowthPickupEntity(const char* entityName) : BaseEntity(entityName) {};
};

class ScreenOverlayEntity : public BaseEntity
{
public:
	Component m_ax_InitialComponents[4] ={ Component::kComponentTransform ,Component::kComponentDraw, Component::kComponentEnd };

	ScreenOverlayEntity(const char* entityName) : BaseEntity(entityName) {};
};

#endif
