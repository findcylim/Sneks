#ifndef PAUSE_MENU_SYSTEM_H
#define PAUSE_MENU_SYSTEM_H


#include "../../ECS/System.h"
class PauseMenuSystem :
	public BaseSystem
{
	EntityManager * m_po_EntityManager;
public:
	PauseMenuSystem(EntityManager* entityManager, EventManager* eventManager);
	~PauseMenuSystem();
	void Update(float dt)override;
};

#endif // !PAUSE_MENU_SYSTEM_H
