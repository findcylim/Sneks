/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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