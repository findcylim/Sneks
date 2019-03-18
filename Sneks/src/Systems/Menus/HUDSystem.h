#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"
#include "../SnekSystem.h"

class HUDSystem : public BaseSystem
{
	GraphicsSystem* m_o_GraphicsSystem;
public:
	void Initialize(CanvasComponent* canvasComponent);
	void Update(float dt) override;
	HUDSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GraphicsSystem* graphics);
	~HUDSystem();
	void CreateBar1();
	void CreateBar2();
};
