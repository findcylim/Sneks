#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#pragma once


#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"
#include "../Math/HTColor.h"
#include "TransformComponent.h"

struct Animation
{
	int m_i_NumFrames;
	AEGfxTexture* m_px_SpriteSheet;
	int m_i_Width;
	int m_i_Height;
};

class AnimationComponent : public BaseComponent
{
public:
	int m_i_CurrentFrameIndex;
	std::vector<Animation> m_vx_AnimationsList;
	int m_i_CurrentAnimationId;
	bool m_b_IsAnimating;
};

#endif