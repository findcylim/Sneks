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
	std::vector<Aabb*>					objectsHitBoxes = {};
};

struct CollisionGroupPairing final
{
	int groupA;
	int groupB;
};

class CollisionSystem final : public BaseSystem // Add event listeners here
{
private:
	std::vector<CollisionGroup*>					m_xo_ComponentsPerGroup;
	std::vector<CollisionGroupPairing>			m_vx_CollisionsPairings = 
	{{0,2}, //Snek Head and Other Head
	 {0,3}, //Snek Head and Other Body
	 {2,1},
	 {0,10},
	 {2,10}
	};


public:
	CollisionSystem(EntityManager* entityManagerPtr);
	~CollisionSystem() = default;
	void Update(float dt) override;
	void Initialize();
	void AddComponentToCollisionGroup(CollisionComponent* collisionComponent, unsigned collisionGroup);
	void UpdateComponentsPerGroup();
	//void AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup);

	static HTVector2 GetMin(DrawComponent* drawComponent);
	static HTVector2 GetMax(DrawComponent* drawComponent);
	void UpdateHitBoxes(CollisionGroup* collisionGroup);
};
#endif