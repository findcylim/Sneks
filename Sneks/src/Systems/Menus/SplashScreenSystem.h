/* Start Header****************************************************************/
/*!
\file SplashScreenSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code for the intro splash screen images

\par Contribution : Adam - 100.00%

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
	float timer = 2.0f;
	size_t splashScreenCounter = 0;
	GraphicsSystem* m_po_GraphicsSystem = nullptr;
public:
	void Initialize();
	void Update(float dt) override;
	void Receive(const Events::EV_MOUSE_ONCLICK& eventData) override;
	SplashScreenSystem(GraphicsSystem* graphicsSystem);
	virtual ~SplashScreenSystem();
};

