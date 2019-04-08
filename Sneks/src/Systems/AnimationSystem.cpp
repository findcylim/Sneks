/* Start Header****************************************************************/
/*!
\file AnimationSystem.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains code that manipulates the texture offsets u and v 
for animation components, which allow spritesheets to work based on dt.

\par Contribution : CY     - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "AnimationSystem.h"
#include "../Components/CameraComponent.h"
#include "../Components/AnimationComponent.h"


void AnimationSystem::Receive(const Events::EV_PLAYER_COLLISION& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
}

AnimationSystem::AnimationSystem( GraphicsSystem* graphicsSystem):
m_o_GraphicsSystem(graphicsSystem)
{
}

AnimationSystem::~AnimationSystem()
{
	m_po_EventManagerPtr->RemoveListener<Events::EV_PLAYER_COLLISION>(this);
}

void AnimationSystem::Initialize()
{
	m_po_EventManagerPtr->AddListener<Events::EV_PLAYER_COLLISION>(this, this);

	/*TODO:: figure out a better initialization for sprite sheets
	 *auto texture = m_o_GraphicsSystem->LoadTextureToMap("../Resources/spritesheet2.png", "TestAnim");

	m_x_SpriteSheetMap.insert(std::pair<const char*, SpriteSheet>(
		"TestAnim", SpriteSheet{ texture,4,7 }
	));*/

	m_po_ComponentManager->Each<AnimationComponent>([&](AnimationComponent* animationComponent)->void
	{
		auto& currentAnim = animationComponent->m_vx_AnimationsList[animationComponent->m_i_CurrentAnimationId];
		currentAnim.m_i_CurrentFrameIndex = currentAnim.m_i_StartFrame;
		UpdateDrawCompTexture(animationComponent);
		animationComponent = static_cast<AnimationComponent*>(animationComponent->m_po_NextComponent);
	}, kComponentAnimation);

}

void AnimationSystem::Update(float dt)
{
	//iterate all animation comps
	m_po_ComponentManager->Each<AnimationComponent>([&](AnimationComponent* animationComponent)
	{
		if (animationComponent->m_vx_AnimationsList.empty())
		{
			return;
		}
		//TODO initialize animation comps properly
		UpdateDrawCompTexture(animationComponent);
		if (animationComponent->m_b_IsAnimating)
		{
			auto& currentAnim = animationComponent->m_vx_AnimationsList[animationComponent->m_i_CurrentAnimationId];
			currentAnim.m_f_CurrentTimeElapsed += dt;
			if (currentAnim.m_f_CurrentTimeElapsed >= currentAnim.m_f_SecondsPerFrame)
			{
				//	currentAnim.m_f_CurrentTimeElapsed -= currentAnim.m_f_MsPerFrame;
				currentAnim.m_f_CurrentTimeElapsed = 0;

				//Increment frame index, wrap around if last frame
				if (++currentAnim.m_i_CurrentFrameIndex >= currentAnim.m_i_StartFrame + currentAnim.m_i_NumFrames)
					currentAnim.m_i_CurrentFrameIndex = currentAnim.m_i_StartFrame;

				UpdateDrawCompTexture(animationComponent);
			}
		}

	}, kComponentAnimation, true);
}

void AnimationSystem::UpdateDrawCompTexture(AnimationComponent* animComp)
{
	auto drawComp = animComp->GetComponent<DrawComponent>();
	if (drawComp)
	{

		auto& currentAnim = animComp->m_vx_AnimationsList[animComp->m_i_CurrentAnimationId];

		int currentX = currentAnim.m_i_CurrentFrameIndex % currentAnim.m_px_SpriteSheet.m_i_Width;
		int currentY = currentAnim.m_i_CurrentFrameIndex / currentAnim.m_px_SpriteSheet.m_i_Width;

		drawComp->m_x_TextureOffset.x = currentX * 1.0f / currentAnim.m_px_SpriteSheet.m_i_Width;
			//+(float)currentAnim.m_px_SpriteSheet.m_i_GapLeftRight / 108.0f;// 1.8f / drawComp->m_po_TransformComponent->GetScale().x;
		drawComp->m_x_TextureOffset.y = currentY * 1.0f / currentAnim.m_px_SpriteSheet.m_i_Height;
			//+(float)currentAnim.m_px_SpriteSheet.m_i_GapTopBottom / 78.0f;//1.8f / drawComp->m_po_TransformComponent->GetScale().y;
	}
}