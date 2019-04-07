/* Start Header****************************************************************/
/*!
\file WinScreenSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for Win screen events

\par Contribution : Adam - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef WIN_SCREEN_SYSTEM_H
#define WIN_SCREEN_SYSTEM_H

#include "../../ECS/System.h"


/*
	Win Screen System declaration	
*/
class WinScreenSystem :
	public BaseSystem
{
public:
	/*
		Constructor and destructor
	*/
	WinScreenSystem();
	~WinScreenSystem();

	// BaseSystem Overrides
	void Initialize() override;
	void Update(float dt) override;

	void SwapWinScreen();

	int m_c_Winner = 1;
};

#endif // !WIN_SCREEN_SYSTEM_H