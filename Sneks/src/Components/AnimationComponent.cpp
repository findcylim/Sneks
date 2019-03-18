#include "AnimationComponent.h"

Animation::Animation(SpriteSheet spriteSheet, int startFrame, int numFrames):
m_px_SpriteSheet(spriteSheet),
m_i_StartFrame(startFrame),
m_i_NumFrames(numFrames),
m_i_CurrentFrameIndex(startFrame)
{
}
