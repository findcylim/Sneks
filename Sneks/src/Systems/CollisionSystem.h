/* Start Header ***************************************************************/
/*!
\file CollisionSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 10

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ECS/System.h"
#include "../Math/Aabb.h"
#include "../ECS/EntityManager.h"
#include "../Components/CollisionComponent.h"
#include <vector>


struct CollisionGroup final
{
public:
	std::vector<CollisionComponent*> objects = {};
	std::vector<Aabb>					objectsHitBoxes = {};
};

struct CollisionGroupPairing final
{
	unsigned int groupA;
	unsigned int groupB;
};


class CollisionSystem final : public BaseSystem // Add event listeners here
, public EventListener<Events::EV_ENTITY_POOL_CHANGED>
{
private:
	std::vector<CollisionGroup*>					m_xo_ComponentsPerGroup;
	std::vector<CollisionGroupPairing>			m_vx_CollisionsPairings = 
		{{kCollGroupSnek1Head,kCollGroupSnek2Head},
		 {kCollGroupSnek1Head,kCollGroupSnek2Body}, //Snek Head and Other Head
		 {kCollGroupSnek2Head,kCollGroupSnek1Body},
		 {kCollGroupSnek1Head,kCollGroupBuilding },
		 {kCollGroupSnek2Head,kCollGroupBuilding },
		 {kCollGroupSnek1Head,kCollGroupPowerUp },
		 {kCollGroupSnek2Head,kCollGroupPowerUp },
		 {kCollGroupMoon		 ,kCollGroupSnek1Head}, //Moon and Other Head
		 {kCollGroupMoon		 ,kCollGroupSnek1Body}, //Moon and Other Body
		 {kCollGroupMoon		 ,kCollGroupSnek2Head}, //Moon and Other Head
		 {kCollGroupMoon		 ,kCollGroupSnek2Body}, //Moon and Other Body
		 {kCollGroupMoon		 ,kCollGroupBuilding },  //Moon and Buildings
		 {kCollGroupMouse    ,kCollGroupUIButton}
		};


public:
	CollisionSystem(EntityManager* entityManagerPtr);
	~CollisionSystem();
	void Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData) override;
	void Update(float dt) override;
	void Initialize();
	void AddComponentToCollisionGroup(CollisionComponent* collisionComponent, unsigned int collisionGroup);
	void UpdateComponentsPerGroup();
	//void AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup);

	static HTVector2 GetMin(DrawComponent* drawComponent);
	static HTVector2 GetMax(DrawComponent* drawComponent);
	void UpdateAllHitBoxes();
	void UpdateHitBoxes(CollisionGroup* collisionGroup) const;
};
#endif