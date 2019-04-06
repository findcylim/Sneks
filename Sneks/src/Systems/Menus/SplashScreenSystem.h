#pragma once
#include "../../ECS/System.h"

class GraphicsSystem;

class SplashScreenSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	float timer = 2.0f;
	size_t splashScreenCounter = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
public:
	void Initialize();
	void Update(float dt) override;
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	SplashScreenSystem(GraphicsSystem* graphicsSystem);
	virtual ~SplashScreenSystem();
};

