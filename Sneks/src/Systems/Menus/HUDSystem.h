#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"
#include "../SnekSystem.h"

class HUDSystem : public BaseSystem
{
public:
	void Initialize(CanvasComponent* canvasComponent);
	void Update(float dt) override;
	HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager);
	~HUDSystem();
};
