#pragma once
#include "../../ECS/System.h"

class GraphicsSystem;

class SplashScreenSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	float timer = 1.5f;
	size_t splashScreenCounter = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
public:
	void Initialize();
	void Update(float dt) override;
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	SplashScreenSystem(EntityManager* entityManagerPtr, EventManager* eventManager,GraphicsSystem* graphicsSystem);
	virtual ~SplashScreenSystem();
};

