/* Start Header ***************************************************************/
/*!
\file BackgroundSystem.cpp
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 5

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "BackgroundSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"

BackgroundSystem::BackgroundSystem( GraphicsSystem* graphics)
{
	m_o_GraphicsSystem = graphics;
}

void BackgroundSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

void BackgroundSystem::Initialize()
{

}

BackgroundEntity* BackgroundSystem::CreateBackground(float posX, float posY, float sizeX, float sizeY, const char* textureName) const
{
	auto newBackgroundEntity = m_po_EntityManager->NewEntity<BackgroundEntity>(kEntityBackground, "BG");

	for (auto i_Component : newBackgroundEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->m_x_Position.x = (posX);
			static_cast<TransformComponent*>(i_Component)->m_x_Position.y=(posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(i_Component),
				textureName, sizeX, sizeY);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 10;
		}
	}
	return newBackgroundEntity;
}

void BackgroundSystem::CreateInstancedBackgrounds(int instancesX, int instancesY, const char* textureName)
{
	float screenSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	float screenSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	for (int i_Background = -instancesX; i_Background <= instancesX; i_Background++) {
		for (int i_BackgroundY = -instancesY; i_BackgroundY <= instancesY; i_BackgroundY++) {
			CreateBackground(i_Background * screenSizeX, i_BackgroundY * screenSizeY,
				screenSizeX, screenSizeY, textureName);
			//m_BgInstances.push_back(bgInstance);
		}
	}
}