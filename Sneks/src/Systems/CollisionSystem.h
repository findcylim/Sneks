#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "../ECS/System.h"
#include "../Math/CollisionGroup.h"
#include "../ECS/EntityManager.h"
#include "../Components/CollisionComponent.h"
#include <vector>

class CollisionSystem final : public BaseSystem // Add event listeners here
{
private:
	EntityManager*		                        m_po_EntityManager;
	ComponentManager*                         m_po_ComponentManager;
	std::vector<CollisionGroup>					m_xo_ComponentsPerGroup;
	std::vector<CollisionGroupPairing>			m_vx_CollisionsPairings;


public:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	void Update(float dt) override;
	void Initialize();
	void AddComponentToCollisionGroup(CollisionComponent* collisionComponent, unsigned collisionGroup);
	void UpdateComponentsPerGroup();
	//void AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup);

	static HTVector2 GetMin(DrawComponent* drawComponent);
	static HTVector2 GetMax(DrawComponent* drawComponent);
};
#endif