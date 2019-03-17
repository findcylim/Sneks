#ifndef COUNTDOWN_SYSTEM_H
#define COUNTDOWN_SYSTEM_H

#include "../../ECS/System.h"
class CountdownSystem :
	public BaseSystem
{
	EntityManager * m_po_EntityManager;
public:
	CountdownSystem(EntityManager* entityManager, EventManager* eventManager);
	~CountdownSystem();
	void Update(float dt)override;
};

#endif // !PAUSE_MENU_SYSTEM_H
