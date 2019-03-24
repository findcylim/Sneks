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
	void Update(float dt)override;
	void SwapWinScreen();

	int m_c_Winner = 1;
};

#endif // !WIN_SCREEN_SYSTEM_H