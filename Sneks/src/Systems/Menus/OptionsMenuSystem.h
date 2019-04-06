/* Start Header****************************************************************/
/*!
\file OptionsMenuSystem.h
\author Primary Author : Javier Foo, javier.foo
\par email: javier.foo\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains

\par Contribution : Javier - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef OPTIONS_MENU_SYSTEM_H
#define OPTIONS_MENU_SYSTEM_H

#include "../../ECS/System.h"
#include "../../Utility/GameStateManager.h"

class OptionsMenuSystem : public BaseSystem
{
	GraphicsSystem* m_o_GraphicsSystem;
	bool fullscreen = true;
public:
	void Initialize();
	void Update(float dt) override;
	OptionsMenuSystem();
	~OptionsMenuSystem();
	bool ReturnFullscreen();
	void ToggleFullscreen();
};

#endif // !OPTIONS_MENU_SYSTEM_H