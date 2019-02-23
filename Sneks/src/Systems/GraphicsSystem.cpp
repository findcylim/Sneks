#include "GraphicsSystem.h"
#include <algorithm>
#include <vector>
#include "../Utility/FileIO.h"

GraphicsSystem::GraphicsSystem(EntityManager* entityManagerPtr) : BaseSystem(entityManagerPtr){};


GraphicsSystem::~GraphicsSystem()
{
	for (auto pairing : m_x_TextureMap)
	{
		AEGfxTextureUnload(pairing.second);
	}
	m_o_EventManagerPtr->RemoveListener<Events::EV_ENTITY_POOL_CHANGED>(this);
}

void GraphicsSystem::Initialize()
{
	//PreLoadTextures();
	m_o_EventManagerPtr->AddListener<Events::EV_ENTITY_POOL_CHANGED>(this);

}

void GraphicsSystem::Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	UpdateDrawOrderVector();
	UpdateMatrices(static_cast<CameraComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentCamera)));
}

AEGfxTexture* GraphicsSystem::FetchTexture(const char* textureName)
{
	for (auto pairing : m_x_TextureMap)
	{
		if (strcmp(textureName, pairing.first) == 0)
		{
			return pairing.second;
		}
	}
	return nullptr;
}

AEGfxTexture* GraphicsSystem::FetchTexture(const char* textureName, int* retWidth, int* retHeight)
{
	for (auto pairing : m_x_TextureMap)
	{
		if (strcmp(textureName, pairing.first) == 0)
		{
			FileIO::ReadPngDimensions(pairing.second->mpName, retWidth, retHeight);
			return pairing.second;
		}
	}
	return nullptr;
}

void GraphicsSystem::PreLoadTextures()
{
	//TODO: MAKE FILE PARSER RESOURCES NEEDED PER LEVEL?
	//SET NAMES TO BE FILE NAMES
	//ENFORCE FILE NAMES TO BE UNIQUE
	LoadTextureToMap("../Resources/head.png"				 , "SnekHead01");
	LoadTextureToMap("../Resources/head2.png"			 , "SnekHead02");
	LoadTextureToMap("../Resources/snake-body.png"		 , "SnekBody01");
	LoadTextureToMap("../Resources/snake-body2.png"	 , "SnekBody02");
	LoadTextureToMap("../Resources/head.png"				 , "SnekTail01");
	LoadTextureToMap("../Resources/head.png"				 , "SnekTail02");
	LoadTextureToMap("../Resources/map.png"				 , "Background01");
	LoadTextureToMap("../Resources/building.png"		 , "Building01"); 
	LoadTextureToMap("../Resources/horz-road.png"		 , "horz-road.png");
	LoadTextureToMap("../Resources/junction.png"		 , "junction.png");
	LoadTextureToMap("../Resources/vert-road.png"		 , "vert-road.png");
	LoadTextureToMap("../Resources/destroyed.png",		   "Destroyed01");

	LoadTextureToMap("../Resources/Ball.png", "Ball");
	LoadTextureToMap("../Resources/Moon.png", "Moon");

}

void GraphicsSystem::LoadTextureToMap(const char* fileName, const char* textureName)
{
	auto texture = AEGfxTextureLoad(fileName);
	m_x_TextureMap.insert(std::pair<const char*, AEGfxTexture*>(textureName,texture));
}

void GraphicsSystem::Update(float dt)
{
	Draw(dt);
}

void GraphicsSystem::UpdateDrawOrderVector(DrawComponent* firstDrawComponent)
{
	m_x_DrawOrder.clear();

	auto i_AddDrawComponent = firstDrawComponent;
	while (i_AddDrawComponent) {
		//m_x_DrawOrder.insert(std::pair<int, DrawComponent*>(i_AddDrawComponent->m_f_DrawPriority, i_AddDrawComponent));
		while (m_x_DrawOrder.size() <= static_cast<size_t>(i_AddDrawComponent->m_f_DrawPriority))
		{
			m_x_DrawOrder.emplace_back();
		}
		m_x_DrawOrder[i_AddDrawComponent->m_f_DrawPriority].push_back(i_AddDrawComponent);
		i_AddDrawComponent = static_cast<DrawComponent*>(i_AddDrawComponent->m_po_NextComponent);
	}
}
void GraphicsSystem::UpdateDrawOrderVector()
{
	auto firstDrawComponent = static_cast<DrawComponent*>(
		m_po_ComponentManager->GetFirstComponentInstance(kComponentDraw));
	UpdateDrawOrderVector(firstDrawComponent);
}

void GraphicsSystem::Draw(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	UpdateMatrices(static_cast<CameraComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentCamera)));
	//Look for DrawComponents
	auto firstDrawComponent = static_cast<DrawComponent*>(m_po_ComponentManager
		->GetFirstComponentInstance(kComponentDraw));
	auto i_DrawComponent = firstDrawComponent;
	int drawCount = 0;

	while (i_DrawComponent){
		drawCount++;
		i_DrawComponent = static_cast<DrawComponent*>(i_DrawComponent->m_po_NextComponent);
	}

	//TODO:: actually update draw order accordingly
	if (m_x_DrawOrder.size() != drawCount)
	{
		UpdateDrawOrderVector(firstDrawComponent); 
	}

	
	for (auto i_DrawVector = m_x_DrawOrder.size() - 1; i_DrawVector > 0; --i_DrawVector) {
		for (auto drawComponent : m_x_DrawOrder[i_DrawVector]) {
			//Check if there is draw component
			if (auto i_TransformComponent = drawComponent->m_po_TransformComponent) {

				//allow transparency to work !! must be first
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

				AEGfxSetTintColor(drawComponent->m_f_RgbaColor.red, drawComponent->m_f_RgbaColor.green, drawComponent->m_f_RgbaColor.blue, drawComponent->m_f_RgbaColor.alpha);
				AEGfxTextureSet(drawComponent->m_px_Texture, 0, 0);
				AEGfxSetTextureMode(AE_GFX_TM_AVERAGE);
				AEGfxSetTransparency(1);
				AEGfxSetPosition(i_TransformComponent->m_x_Position.x, i_TransformComponent->m_x_Position.y);
				AEGfxSetTransform(drawComponent->m_po_GlobalMatrix->m);
				AEGfxMeshDraw(drawComponent->m_px_Mesh, AE_GFX_MDM_TRIANGLES);
			}
			//i_DrawComponent = static_cast<DrawComponent*>(i_DrawComponent->m_po_PrevComponent);
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

		}

		i_DrawComponent = static_cast<DrawComponent*>(i_DrawComponent->m_po_NextComponent);
	}
}
