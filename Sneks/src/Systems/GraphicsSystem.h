/* Start Header ***************************************************************/
/*!
\file GraphicsSystem.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains 

\par Contribution (hours): CY - 15

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H
#pragma once
#include <map>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Components/AnimationComponent.h"

class GraphicsSystem : public BaseSystem, 
	public EventListener<Events::EV_ENTITY_POOL_CHANGED>
{
public:
	std::multimap<const char*, AEGfxTexture*> m_x_TextureMap;
	std::multimap<const char*, AEGfxVertexList*> m_x_MeshMap;
	u32 m_i_font;
	GraphicsSystem(EntityManager* entityManagerPtr);
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize();
	void InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture, const float sizeX, const float sizeY,
											HTColor color ={ 1,1,1,1 }, int spriteCountX = 1, int spriteCountY = 1);

	void InitializeDrawComponent(DrawComponent* dc, const char* texture, const float sizeX, const float sizeY, 
											HTColor color ={ 1,1,1,1 }, int spriteCountX = 1, int spriteCountY = 1);
	void InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture,
											HTColor color ={ 1,1,1,1 }, int spriteCountX = 1, int spriteCountY = 1);
	void InitializeDrawComponent(DrawComponent* dc, const char* texture, 
											HTColor color = {1,1,1,1}, int spriteCountX = 1, int spriteCountY = 1);
	void Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData) override;
	AEGfxTexture* FetchTexture(const char* textureName);
	AEGfxVertexList* FetchMesh(const char* meshName);
	AEGfxTexture* FetchTexture(const char* textureName, int* retWidth, int* retHeight);
	void PreLoadTextures();
	AEGfxTexture* LoadTextureToMap(const char* fileName, const char* textureName);
	void Update(float dt) override;
	void UpdateDrawOrderVector(DrawComponent* firstDrawComponent);
	void UpdateDrawOrderVector();
	void Draw(float dt);
	void DrawBloom(DrawComponent* drawComponent);
	void UpdateMatrices(CameraComponent*) const;
	void DrawTextRenderer()const;
private:
	std::vector<std::vector<DrawComponent*>> m_x_DrawOrder ={};
	//std::vector<DrawComponent* [10000]> m_x_DrawOrderTest;

};
#endif