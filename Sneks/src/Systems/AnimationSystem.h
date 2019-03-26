/* Start Header ***************************************************************/
/*!
\file AnimationSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 19/03/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#pragma once
#include "../ECS/System.h"
#include "../Components/AnimationComponent.h"
#include "GraphicsSystem.h"

class AnimationSystem : public BaseSystem,
	public EventListener<Events::EV_PLAYER_COLLISION>
{
private:
	GraphicsSystem*								  m_o_GraphicsSystem;
	std::multimap<const char*, SpriteSheet>  m_x_SpriteSheetMap;
	void Receive(const Events::EV_PLAYER_COLLISION& eventData) override;
public:

	AnimationSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphicsSystem);
	~AnimationSystem();
	void Initialize();
	void Update(float dt) override;
	void UpdateDrawCompTexture(AnimationComponent* animComp);
};

