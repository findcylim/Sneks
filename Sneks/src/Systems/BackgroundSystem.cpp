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