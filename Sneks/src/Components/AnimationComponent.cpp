/* Start Header****************************************************************/
/*!
\file AnimationComponent.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Secondary Authors:
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 18/03/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 3

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "AnimationComponent.h"

Animation::Animation(SpriteSheet spriteSheet, int startFrame, int numFrames):
m_px_SpriteSheet(spriteSheet),
m_i_StartFrame(startFrame),
m_i_NumFrames(numFrames),
m_i_CurrentFrameIndex(startFrame)
{
}
