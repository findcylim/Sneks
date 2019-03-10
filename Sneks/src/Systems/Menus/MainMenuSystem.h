#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"
class MainMenuSystem: public BaseSystem
{
public:
	void Initialize(CanvasComponent* canvasComponent);
	void Update(float dt) override;
	MainMenuSystem(EntityManager* entityManagerPtr, EventManager* eventManager);
	~MainMenuSystem();
};

