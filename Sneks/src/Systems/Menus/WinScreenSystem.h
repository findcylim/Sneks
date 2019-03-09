#ifndef WIN_SCREEN_SYSTEM_H
#define WIN_SCREEN_SYSTEM_H


#include "../../ECS/System.h"
class WinScreenSystem :
	public BaseSystem
{
	EntityManager * m_po_EntityManager;
public:
	WinScreenSystem(EntityManager* entityManager, EventManager* eventManager);
	~WinScreenSystem();
};

#endif // !WIN_SCREEN_SYSTEM_H