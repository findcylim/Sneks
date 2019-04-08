/* Start Header****************************************************************/
/*!
\file HUDSystem.h
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

#pragma once
#include "../CanvasUISystem.h"
#include "../../ECS/System.h"
#include "../SnekSystem.h"

class HUDSystem : public BaseSystem
{
	GraphicsSystem* m_o_GraphicsSystem;
public:
	/* Constructor/ Destructor */
	HUDSystem(GraphicsSystem* graphics);
	~HUDSystem();

	/* General System Functions */
	void Initialize();
	void Update(float dt) override;
};
