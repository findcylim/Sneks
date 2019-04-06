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

#pragma once
#ifndef PROJECTILE_SYSTEM_H
#define PROJECTILE_SYSTEM_H
#pragma once
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
//#include "../ECS/ECSystem.h"
#include "GraphicsSystem.h"
//#include "SnekSystem.h"

class ProjectileSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>,
	public EventListener<Events::EV_CREATE_PROJECTILE>
{
public:
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