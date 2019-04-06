/* Start Header****************************************************************/
/*!
\file PauseMenuSystem.h
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
