#pragma once
#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"
#include "SnekSystem.h"

class ProjectileSystem final : public BaseSystem,
	public EventListener<Events::Ev_PLAYER_COLLISION>
{
public:
	ProjectileSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	~ProjectileSystem();
	void receive(const Events::Ev_PLAYER_COLLISION& eventData);
	void Update(float dt) override;
	void Initialize();
	void CreateMoon(SnekHeadEntity* owner, const char* textureName) const;

private:
	GraphicsSystem* m_o_GraphicsSystem;
};

#endif //BACKGROUND_SYSTEM_H