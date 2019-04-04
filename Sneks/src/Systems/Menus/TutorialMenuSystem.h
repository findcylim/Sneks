#ifndef TUTORIAL_MENU_SYSTEM_H
#define TUTORIAL_MENU_SYSTEM_H


#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"

class TutorialMenuSystem :
	public BaseSystem,
	public EventListener<Events::EV_PLAYER_MOVEMENT_KEY>
{
	EntityManager * m_po_EntityManager;
	GameStateManager* m_po_GameStateManager;
	float timer;
public:
	TutorialMenuSystem(EntityManager* entityManager, EventManager* eventManager, GameStateManager* gameStateManager);
	~TutorialMenuSystem();

	void Initialize(CanvasComponent* canvasComponent);
	void Receive(const Events::EV_PLAYER_MOVEMENT_KEY& eventData) override;
	void Update(float dt)override;
};

#endif // !TUTORIAL_MENU_SYSTEM_H
