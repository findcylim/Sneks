#ifndef PAUSE_MENU_SYSTEM_H
#define PAUSE_MENU_SYSTEM_H


#include "../../ECS/System.h"
class PauseMenuSystem :
	public BaseSystem
{
public:
	PauseMenuSystem();
	void Initialize();
	~PauseMenuSystem();
	void Update(float dt)override;
	void OnEnable()override;
	void OnDisable()override;
};

#endif // !PAUSE_MENU_SYSTEM_H
