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
	const char* m_x_TextureName;
	//AEGfxTexture* m_px_Texture;
	int m_i_Width;
	int m_i_Height;
//	int m_i_GapTopBottom = 3;
// int m_i_GapLeftRight = 3;
};

struct Animation
{
	SpriteSheet m_px_SpriteSheet;

	int m_i_StartFrame = 0; // the first frame of the animation within the sprite sheet
	int m_i_NumFrames; //number of frames the animation has

	int m_i_CurrentFrameIndex = 0;    //dont touch, used for internal calculations
	float m_f_CurrentTimeElapsed = 0; // dont touch, used for internal calculations

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