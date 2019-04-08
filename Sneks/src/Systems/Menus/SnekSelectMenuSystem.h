/* Start Header****************************************************************/
/*!
\file SnekSelectMenuSystem.h
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

#ifndef SNEK_SELECT_MENU_SYSTEM_H
#define SNEK_SELECT_MENU_SYSTEM_H


#include "../../ECS/System.h"
class SnekSelectMenuSystem : public BaseSystem
{
public:
	/* Constrcutor/Destructor */
	SnekSelectMenuSystem();
	~SnekSelectMenuSystem();

	/* General System Functions */
	void Initialize();
	void Update(float dt)override;
};

#endif // !SNEK_SELECT_MENU_SYSTEM_H
