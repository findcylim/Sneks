#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"
class MainMenuSystem: public BaseSystem
{
public:
	void Initialize();
	void Update(float dt) override;
	~MainMenuSystem();
};

