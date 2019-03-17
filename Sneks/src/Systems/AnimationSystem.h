#pragma once
#include "../ECS/System.h"
#include "../Components/AnimationComponent.h"
#include "GraphicsSystem.h"

class AnimationSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	GraphicsSystem*								  m_o_GraphicsSystem;
	std::multimap<const char*, SpriteSheet>  m_x_SpriteSheetMap;
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
public:

	AnimationSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphicsSystem);
	~AnimationSystem();
	void Initialize();
	void Update(float dt) override;
	void UpdateDrawCompTexture(AnimationComponent* animComp);
};

