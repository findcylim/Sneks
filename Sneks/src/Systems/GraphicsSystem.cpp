#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{
	
};


GraphicsSystem::~GraphicsSystem()
{
	for (auto pairing : m_x_TextureMap)
	{
		AEGfxTextureUnload(pairing.second);
	}
}

void GraphicsSystem::Initialize(EntityManager* entityManager)
{
	m_po_EntityManager = entityManager;
	m_po_ComponentManager = entityManager->GetComponentManager();
}

AEGfxTexture* GraphicsSystem::FetchTexture(const char* textureName)
{
	AEGfxTexture* retTexture = m_x_TextureMap.find(textureName)->second;
	assert(retTexture != nullptr);
	return retTexture;
}

void GraphicsSystem::PreLoadTextures()
{
	//TODO: MAKE FILE PARSER RESOURCES NEEDED PER LEVEL?
	LoadTextureToMap("../Resources/snake-head.png"    , "Snek01_Head");
	LoadTextureToMap("../Resources/head2.png"			 , "Snek02_Head");
	LoadTextureToMap("../Resources/snake-body.png"		 , "Snek01_Body");
	LoadTextureToMap("../Resources/snake-body2.png"	 , "Snek02_Body");
	LoadTextureToMap("../Resources/rocket_booster.jpg", "Particles01_Rocket");
	LoadTextureToMap("../Resources/smoke.jpg"			 , "Particles02_Smoke");
	LoadTextureToMap("../Resources/map.png"				 , "Map01");
	LoadTextureToMap("../Resources/building.png"		 , "Building01");

}

void GraphicsSystem::LoadTextureToMap(const char* fileName, const char* textureName)
{
	auto texture = AEGfxTextureLoad(fileName);
	m_x_TextureMap.insert(std::pair<const char*, AEGfxTexture*>(textureName,texture));
}

void GraphicsSystem::Draw(float dt) const
{
	//Look for DrawComponents
	auto i_DrawComponent = static_cast<DrawComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentDraw));

	while (i_DrawComponent)
	{
		//Check if there is transform component
		if (auto i_TransformComponent = i_DrawComponent->m_po_TransformComponent) {

			//allow transparency to work !! must be first
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

			AEGfxSetTintColor(i_DrawComponent->m_f_RgbaColor.red, i_DrawComponent->m_f_RgbaColor.green, i_DrawComponent->m_f_RgbaColor.blue, i_DrawComponent->m_f_RgbaColor.alpha);
			AEGfxTextureSet(i_DrawComponent->m_px_Texture, 0, 0);
			AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
			AEGfxSetTransparency(1);
			AEGfxSetPosition(i_TransformComponent->m_x_Position.x, i_TransformComponent->m_x_Position.y);
			AEGfxSetTransform(i_DrawComponent->m_po_GlobalMatrix->m);
			AEGfxMeshDraw(i_DrawComponent->m_px_Mesh, AE_GFX_MDM_TRIANGLES);
		}
	}
}

void GraphicsSystem::UpdateMatrices(CameraComponent* cameraComponent) const
{
	auto i_DrawComponent = static_cast<DrawComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentDraw));

	while (i_DrawComponent)
	{
		//Check if there is transform component
		if (auto i_TransformComponent = i_DrawComponent->m_po_TransformComponent) {

			AEMtx33Rot(i_DrawComponent->m_po_RotationMatrix, i_TransformComponent->GetRotation());
			AEMtx33ScaleApply(
				i_DrawComponent->m_po_RotationMatrix, i_DrawComponent->m_po_RotationMatrix, 
				i_TransformComponent->m_f_Scale, i_TransformComponent->m_f_Scale
				);
			
			AEMtx33Trans(
				i_DrawComponent->m_po_TranslationMatrix, i_TransformComponent->m_x_Position.x, i_TransformComponent->m_x_Position.y
				);

			/*generate global matrix from rot and trans*/
			AEMtx33Concat(i_DrawComponent->m_po_GlobalMatrix,
				i_DrawComponent->m_po_TranslationMatrix, i_DrawComponent->m_po_RotationMatrix
				);

			AEMtx33TransApply(i_DrawComponent->m_po_GlobalMatrix,
				i_DrawComponent->m_po_GlobalMatrix, cameraComponent->m_f_VirtualOffsetX, cameraComponent->m_f_VirtualOffsetY
				);

			AEMtx33ScaleApply(i_DrawComponent->m_po_GlobalMatrix,
				i_DrawComponent->m_po_GlobalMatrix, cameraComponent->m_f_VirtualScale, cameraComponent->m_f_VirtualScale
				);

			i_DrawComponent = static_cast<DrawComponent*>(i_DrawComponent->m_po_NextComponent);
		}
	}
}
