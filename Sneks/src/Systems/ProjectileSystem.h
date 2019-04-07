/* Start Header****************************************************************/
/*!
\file ProjectileSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file containscode for Lunar pellet projectile

\par Contribution : Adam     - 50.00% 
\par Contribution : Javier   - 50.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "GraphicsSystem.h"

/*
	Projectile System 

	Handling all current and future projectiles
	Game specific
*/
class ProjectileSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>,
	public EventListener<Events::EV_CREATE_PROJECTILE>
{
public:
	// Constructor and Destructor
	ProjectileSystem( GraphicsSystem* graphics);
	~ProjectileSystem();

	void Receive(const Events::EV_PLAYER_COLLISION& eventData);
	void Receive(const Events::EV_CREATE_PROJECTILE& eventData);
	void Update(float dt) override;
	void Initialize();
	//void CreateMoon(SnekHeadEntity* owner, const char* textureName) const;

private:
	GraphicsSystem* m_o_GraphicsSystem;
};

//float GetProjectileChargeRate();

#endif //BACKGROUND_SYSTEM_H