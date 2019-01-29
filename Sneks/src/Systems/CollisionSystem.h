#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../ECS/System.h"

class CollisionSystem : public BaseSystem // Add eventlisteners here
{


public:
	CollisionSystem();
	~CollisionSystem();
	void Update() override;
};




#endif