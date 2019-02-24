#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H
#pragma once
#include <map>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"

class GraphicsSystem : public BaseSystem, 
	public EventListener<Events::EV_ENTITY_POOL_CHANGED>
{
public:
	std::multimap<const char*, AEGfxTexture*> m_x_TextureMap;
	std::multimap<const char*, AEGfxVertexList*> m_x_MeshMap;

	GraphicsSystem(EntityManager* entityManagerPtr);
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize();
	void InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture, const float sizeX, const float sizeY, HTColor color ={ 1,1,1,1 });
	void InitializeDrawComponent(DrawComponent* dc, const char* texture, const float sizeX, const float sizeY, HTColor color ={ 1,1,1,1 });
	void InitializeDrawComponent(DrawComponent* dc, AEGfxTexture* texture, HTColor color ={ 1,1,1,1 });
	void InitializeDrawComponent(DrawComponent* dc, const char* texture, HTColor color = {1,1,1,1});
	void Receive(const Events::EV_ENTITY_POOL_CHANGED& eventData) override;
	AEGfxTexture* FetchTexture(const char* textureName);
	AEGfxVertexList* FetchMesh(const char* meshName);
	AEGfxTexture* FetchTexture(const char* textureName, int* retWidth, int* retHeight);
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Update(float dt) override;
	void UpdateDrawOrderVector(DrawComponent* firstDrawComponent);
	void UpdateDrawOrderVector();
	void Draw(float dt);
	void UpdateMatrices(CameraComponent*) const;

private:
	std::vector<std::vector<DrawComponent*>> m_x_DrawOrder ={};

};
#endif