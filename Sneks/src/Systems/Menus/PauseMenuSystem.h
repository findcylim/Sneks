#ifndef PAUSE_MENU_SYSTEM_H
#define PAUSE_MENU_SYSTEM_H


#include "../../ECS/System.h"
class PauseMenuSystem :
	public BaseSystem
{
public:
	PauseMenuSystem(EntityManager* entityManager, EventManager* eventManager);
	void Initialize();
	~PauseMenuSystem();
	void Update(float dt)override;
};

#endif // !PAUSE_MENU_SYSTEM_H