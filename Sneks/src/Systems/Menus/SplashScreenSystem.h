/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 05/04/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../../ECS/System.h"

class GraphicsSystem;

class SplashScreenSystem :
	public BaseSystem,
	public EventListener<Events::EV_MOUSE_ONCLICK>
{
	float timer = 1.5f;
	size_t splashScreenCounter = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
public:
	void Initialize();
	void Update(float dt) override;
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	SplashScreenSystem(GraphicsSystem* graphicsSystem);
	virtual ~SplashScreenSystem();
};

