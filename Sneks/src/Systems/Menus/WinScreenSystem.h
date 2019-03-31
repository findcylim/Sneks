#ifndef WIN_SCREEN_SYSTEM_H
#define WIN_SCREEN_SYSTEM_H


#include "../../ECS/System.h"
class WinScreenSystem :
	public BaseSystem
{
public:
	void Initialize();
	~WinScreenSystem();
	void Update(float dt)override;
};

#endif // !WIN_SCREEN_SYSTEM_H