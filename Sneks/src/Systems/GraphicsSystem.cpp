#include "GraphicsSystem.h"
#include <algorithm>
#include <vector>
#include "../Utility/FileIO.h"
#include "../Utility/AlphaEngineHelper.h"
#include "../Components/TextRendererComponent.h"

static unsigned debugFont;
void PrintOnScreen(unsigned int fontId, const char* toPrint, float relativePosX, float relativePosY, float red, float green, float blue);

GraphicsSystem::GraphicsSystem(EntityManager* entityManagerPtr) : BaseSystem(entityManagerPtr)
{};


GraphicsSystem::~GraphicsSystem()
{
	for (auto pairing : m_x_TextureMap)
	{
		AEGfxTextureUnload(pairing.second);
	}
	for (auto pairing : m_x_MeshMap)
	{
		AEGfxMeshFree(pairing.second);
	}
	m_x_TextureMap.clear();
	AEGfxDestroyFont(debugFont);
	m_o_EventManagerPtr->RemoveListener<Events::EV_ENTITY_POOL_CHANGED>(this);
}

void GraphicsSystem::Initialize()
{
	m_o_EventManagerPtr->AddListener<Events::EV_ENTITY_POOL_CHANGED>(this);
	debugFont = AEGfxCreateFont("Segoe UI", 25, 1, 0);

	m_i_font = AEGfxCreateFont("Arial", 30, false, false);
}

void GraphicsSystem::InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture, const float sizeX,
	const float sizeY, HTColor color)
{
	auto largerDimension = max(sizeX, sizeY);

	auto normalizedX = sizeX / largerDimension;
	auto normalizedY = sizeY / largerDimension;

	dc->m_x_MeshSize  ={ normalizedX, normalizedY };

	dc->m_px_Texture  = texture;

	if (auto mesh = FetchMesh(texture->mpName))
	{
		dc->m_px_Mesh = mesh;
	}
	else {
		//check if mesh exists
		AEGfxMeshStart();
		AEGfxTriAdd(-(normalizedX / 2), -(normalizedY / 2), 0x00FFFFFF, 0, 1,
			normalizedX / 2, -(normalizedY / 2), 0x0000FFFF, 1, 1,
			-(normalizedX / 2), normalizedY / 2, 0x00FFFF00, 0, 0);

		AEGfxTriAdd(
			normalizedX / 2, normalizedY / 2, 0x00FFFFFF, 1, 0,
			-(normalizedX / 2), normalizedY / 2, 0x00FFFFFF, 0, 0,
			normalizedX / 2, -(normalizedY / 2), 0x00FFFFFF, 1, 1);
		dc->m_px_Mesh = AEGfxMeshEnd();

		m_x_MeshMap.insert(std::pair<const char*, AEGfxVertexList*>(texture->mpName, dc->m_px_Mesh));
	}

	dc->m_po_TransformComponent->m_f_Scale = dc->m_po_TransformComponent->m_f_Scale * largerDimension;

	dc->m_f_RgbaColor = color;
}

void GraphicsSystem::InitializeDrawComponent(DrawComponent* dc, const char* texture, const float sizeX,
	const float sizeY, HTColor color)
{
	InitializeDrawComponent(dc, FetchTexture(texture), sizeX, sizeY, color);
}

void GraphicsSystem::InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture, HTColor color)
{
	int sizeX, sizeY;
	FileIO::ReadPngDimensions(texture->mpName, &sizeX, &sizeY);
	InitializeDrawComponent(dc, texture, static_cast<float>(sizeX), static_cast<float>(sizeY), color);
}

void GraphicsSystem::InitializeDrawComponent(DrawComponent* dc, const char* texture, HTColor color)
{
	InitializeDrawComponent(dc, FetchTexture(texture), color);
}



void GraphicsSystem::Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData)
{
	UNREFERENCED_PARAMETER(eventData);
	UpdateDrawOrderVector();
	UpdateMatrices(m_po_ComponentManager
		->GetFirstComponentInstance<CameraComponent>(kComponentCamera));
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

AEGfxVertexList* GraphicsSystem::FetchMesh(const char* meshName)
{
	for (auto pairing : m_x_MeshMap)
	{
		if (strcmp(meshName, pairing.first) == 0)
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
	LoadTextureToMap("../Resources/MainMenuLogo.png", "MainMenuLogo");
	LoadTextureToMap("../Resources/MouseCollider.png", "MouseCollider");
	LoadTextureToMap("../Resources/UIBack.png", "UIBack");
	
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
	auto firstDrawComponent = 
		m_po_ComponentManager->GetFirstComponentInstance<DrawComponent>(kComponentDraw);
	UpdateDrawOrderVector(firstDrawComponent);
}

void GraphicsSystem::Draw(float dt)
{
	UNREFERENCED_PARAMETER(dt);

	UpdateMatrices(m_po_ComponentManager
		->GetFirstComponentInstance<CameraComponent>(kComponentCamera));
	//Look for DrawComponents
	auto firstDrawComponent = m_po_ComponentManager
		->GetFirstComponentInstance<DrawComponent>(kComponentDraw);
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

	if (m_x_DrawOrder.empty())
		return;

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


	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	SnekHeadEntity* snekHeadEntity = m_po_EntityManager->GetFirstEntityInstance<SnekHeadEntity>(kEntitySnekHead);

	auto snekPhysics = snekHeadEntity->GetComponent<PhysicsComponent>();

	s8 strBuffer[500];

	sprintf_s(strBuffer, sizeof(strBuffer), "Snek 1 Speed: %f / %f", snekPhysics->m_f_Speed, snekPhysics->m_f_MaxSpeed);

	AEGfxPrint(debugFont, strBuffer, -900, 480, 0, 0, 1);

	snekHeadEntity = static_cast<SnekHeadEntity*>(snekHeadEntity->m_po_NextEntity);

	snekPhysics = snekHeadEntity->GetComponent<PhysicsComponent>();

	sprintf_s(strBuffer, sizeof(strBuffer), "Snek 2 Speed: %f / %f", snekPhysics->m_f_Speed, snekPhysics->m_f_MaxSpeed);

	AEGfxPrint(debugFont, strBuffer, 200, 480, 1, 0, 0);

	DrawTextRenderer();
}

void GraphicsSystem::UpdateMatrices(CameraComponent* cameraComponent) const
{
	auto i_DrawComponent = m_po_ComponentManager
		->GetFirstComponentInstance<DrawComponent>(kComponentDraw);

	while (i_DrawComponent)
	{
		//Check if there is transform component
		if (auto i_TransformComponent = i_DrawComponent->m_po_TransformComponent) {

			AEMtx33 globalTransform;

			AEMtx33Identity(&globalTransform);

			AEMtx33Scale(i_DrawComponent->m_po_ScaleMatrix, 
				i_TransformComponent->m_f_Scale.x * i_TransformComponent->m_f_ScaleMultiplier.x,
				i_TransformComponent->m_f_Scale.y * i_TransformComponent->m_f_ScaleMultiplier.y
				);

			AEMtx33Concat(&globalTransform, i_DrawComponent->m_po_ScaleMatrix,
				&globalTransform
			);

			AEMtx33Rot(i_DrawComponent->m_po_RotationMatrix, i_TransformComponent->GetRotation());

			AEMtx33Concat(&globalTransform, i_DrawComponent->m_po_RotationMatrix,
				&globalTransform
			);

			AEMtx33Trans(
				i_DrawComponent->m_po_TranslationMatrix, i_TransformComponent->m_x_Position.x, i_TransformComponent->m_x_Position.y
				);

			/*generate global matrix from rot and trans and scale*/
			AEMtx33Concat(i_DrawComponent->m_po_GlobalMatrix, i_DrawComponent->m_po_TranslationMatrix,
				&globalTransform	
				);

			AEMtx33 cameraTransform;
			AEMtx33Identity(&cameraTransform);

			AEMtx33TransApply(&cameraTransform,
				&cameraTransform, cameraComponent->m_f_VirtualOffsetX, cameraComponent->m_f_VirtualOffsetY
				);

			AEMtx33 rotMatrix;
			AEMtx33Rot(&rotMatrix, cameraComponent->m_f_VirtualRotation);

			AEMtx33Concat(&cameraTransform,
				&rotMatrix, &cameraTransform
			);

			AEMtx33ScaleApply(&cameraTransform,
				&cameraTransform, cameraComponent->m_f_VirtualScale, cameraComponent->m_f_VirtualScale
				);

			AEMtx33Concat(i_DrawComponent->m_po_GlobalMatrix, 
				&cameraTransform, i_DrawComponent->m_po_GlobalMatrix
				);

		}
		i_DrawComponent = static_cast<DrawComponent*>(i_DrawComponent->m_po_NextComponent);
	}
}


void GraphicsSystem::DrawTextRenderer()const
{
	auto text_Comp = m_po_ComponentManager->GetFirstComponentInstance<TextRendererComponent>(kComponentTextRenderer);
	while (text_Comp)
	{
		char textToDraw[100];
		sprintf_s(textToDraw, 100, "%s", text_Comp->m_p_Text);
		AEGfxPrint(m_i_font, textToDraw, static_cast<s32>(text_Comp->m_po_LinkedTransform->m_x_Position.x-AEGfxGetWinMaxX() + text_Comp->m_o_PositionOffset.x), 
										 static_cast<s32>(text_Comp->m_po_LinkedTransform->m_x_Position.y+ AEGfxGetWinMaxY() + text_Comp->m_o_PositionOffset.y), 0, 0, 0);
		text_Comp = static_cast<TextRendererComponent*>(text_Comp->m_po_NextComponent);
	}
}

void PrintOnScreen(unsigned int fontId, const char* toPrint, float relativePosX, float relativePosY, float red, float green, float blue)
{
	float halfScreenSizeX, halfScreenSizeY;
	AlphaEngineHelper::GetScreenSize(&halfScreenSizeX, &halfScreenSizeY);
	halfScreenSizeX /= 2;
	halfScreenSizeY /= 2;

	s8 strBuffer[500];
	float posX = -halfScreenSizeX + relativePosX * halfScreenSizeX * 2;
	float posY = -halfScreenSizeY + relativePosY * halfScreenSizeY * 2;
	sprintf_s(strBuffer, sizeof(strBuffer), toPrint);
	AEGfxPrint(fontId, strBuffer, (int)posX, (int)posY, red, green, blue);
}