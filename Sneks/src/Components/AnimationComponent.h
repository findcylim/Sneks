#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#pragma once


#include "AEEngine.h"
#include "../Math/HTVector2.h"
#include "../ECS/ComponentList.h"
#include "../Math/HTColor.h"
#include "TransformComponent.h"

struct SpriteSheet
{
	AEGfxTexture* m_px_Texture;
	int m_i_Width;
	int m_i_Height;
};

struct Animation
{
	SpriteSheet m_px_SpriteSheet;

	int m_i_StartFrame = 0;
	int m_i_NumFrames;

	int m_i_CurrentFrameIndex = 0;
	float m_f_CurrentTimeElapsed = 0;

	float m_f_SecondsPerFrame = 0.1f; //Animation speed, 1.0f means 1 fps
	Animation(SpriteSheet spriteSheet,int startFrame = 0, int numFrames = 1);
};

class AnimationComponent : public BaseComponent
{
public:
	std::vector<const char*> m_vx_Names;
	std::vector<Animation> m_vx_AnimationsList;
	int m_i_CurrentAnimationId;
	bool m_b_IsAnimating;
};

#endif