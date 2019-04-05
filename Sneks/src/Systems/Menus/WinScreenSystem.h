#ifndef WIN_SCREEN_SYSTEM_H
#define WIN_SCREEN_SYSTEM_H


#include "../../ECS/System.h"
class WinScreenSystem :
	public BaseSystem
{
public:
	WinScreenSystem();
	~WinScreenSystem();
	void Initialize() override;
	void Update(float dt) override;
	void SwapWinScreen();

	int m_c_Winner = 1;
};

#endif // !WIN_SCREEN_SYSTEM_H