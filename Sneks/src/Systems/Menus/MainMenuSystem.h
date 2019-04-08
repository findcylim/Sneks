/* Start Header****************************************************************/
/*!
\file MainMenuSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains the declaration for main menu elements

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"

class MainMenuSystem: public BaseSystem
{
public:
	void Initialize();
	void Update(float dt) override;
	~MainMenuSystem();
};

