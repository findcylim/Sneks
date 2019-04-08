/* Start Header****************************************************************/
/*!
\file AnimationSystem.h
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains header file that manipulates the texture offsets u and
v for animation components, which allow spritesheets to work based on dt.

\par Contribution : CY     - 100.00%

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

	AnimationSystem( GraphicsSystem* graphicsSystem);
	~AnimationSystem();
	void Initialize();
	void Update(float dt) override;
	void UpdateDrawCompTexture(AnimationComponent* animComp);
};

