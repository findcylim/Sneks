#include "BackgroundSystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraComponent.h"
#include "../Components/CollisionComponent.h"

BackgroundSystem::BackgroundSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics) :
	BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
}

void BackgroundSystem::Update(float dt)
{
	
}

void BackgroundSystem::Initialize()
{
	float screenSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	float screenSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	//CreateInstancedBackgrounds(1, 1, "Background01");
}

void BackgroundSystem::CreateBackground(float posX, float posY, float sizeX, float sizeY, const char* textureName) const
{
	auto newBackgroundEntity = static_cast<BackgroundEntity*>(
		m_po_EntityManager->NewEntity(kEntityBackground, "BG"));

	for (auto i_Component : newBackgroundEntity->m_v_AttachedComponentsList)
	{
		if (i_Component->m_x_ComponentID == kComponentTransform)
		{
			static_cast<TransformComponent*>(i_Component)->SetPosition(posX, posY);
			static_cast<TransformComponent*>(i_Component)->SetRotation(0);
		}
		else if (i_Component->m_x_ComponentID == kComponentDraw)
		{
			static_cast<DrawComponent*>(i_Component)->Initialize(
				m_o_GraphicsSystem->FetchTexture(textureName),
				sizeX, sizeY, HTColor{ 1,1,1,1 }
			);
			static_cast<DrawComponent*>(i_Component)->m_f_DrawPriority = 10;
		}
	}
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