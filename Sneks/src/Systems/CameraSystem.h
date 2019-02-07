#pragma once
#include "../ECS/System.h"

class CameraSystem :
	public BaseSystem
{
public:
	CameraSystem(EntityManager* entityManagerPtr);
	void UpdateCamera(float dt) const;
	void Update(float dt) override;

};

