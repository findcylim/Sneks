#ifndef SNEK_SELECT_MENU_SYSTEM_H
#define SNEK_SELECT_MENU_SYSTEM_H


#include "../../ECS/System.h"
class SnekSelectMenuSystem : public BaseSystem
{
public:
	SnekSelectMenuSystem();
	~SnekSelectMenuSystem();

	void Initialize();
	void Update(float dt)override;
};

#endif // !SNEK_SELECT_MENU_SYSTEM_H
