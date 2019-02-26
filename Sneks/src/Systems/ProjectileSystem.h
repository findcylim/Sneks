#pragma once
#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "SnekSystem.h"

class ProjectileSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>,
	public EventListener<Events::EV_CREATE_PROJECTILE>
{
public:
	ProjectileSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	~ProjectileSystem();
	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
	void Receive(const Events::EV_CREATE_PROJECTILE& eventData);
	void Update(float dt) override;
	void Initialize();
	//void CreateMoon(SnekHeadEntity* owner, const char* textureName) const;

private:
	GraphicsSystem* m_o_GraphicsSystem;
};

#endif //BACKGROUND_SYSTEM_H