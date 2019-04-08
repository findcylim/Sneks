/* Start Header****************************************************************/
/*!
\file EndRoundScreenSystem.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef ENDROUND_MENU_SYSTEM_H
#define ENDROUND_MENU_SYSTEM_H


#include "../../ECS/System.h"
class EndRoundScreenSystem :
	public BaseSystem
{
public:
	EndRoundScreenSystem();
	void Initialize() override;
	~EndRoundScreenSystem();
	void Update(float dt)override;
};

#endif // !PAUSE_MENU_SYSTEM_H