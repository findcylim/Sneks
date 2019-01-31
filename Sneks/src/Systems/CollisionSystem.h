#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/System.h"
#

class CollisionSystem : public BaseSystem // Add event listeners here
{


public:
	CollisionSystem();
	~CollisionSystem();
	void Update() override;
	void Update(float dt);
	void AddObjectToCollisionGroup(DrawObject*, unsigned int collisionGroup);

};




#endif