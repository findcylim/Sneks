#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H
#pragma once
#include <map>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"

class GraphicsSystem : public BaseSystem
{
public:
	std::multimap<const char*, AEGfxTexture*> m_x_TextureMap;

	GraphicsSystem(EntityManager* entityManagerPtr);
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize(EntityManager* entityManager);
	AEGfxTexture* FetchTexture(const char* textureName);
	AEGfxTexture* FetchTexture(const char* textureName, int* retWidth, int* retHeight);
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Update(float dt) override;
	void UpdateDrawOrderVector(DrawComponent* firstDrawComponent);
	void Draw(float dt);
	void UpdateMatrices(CameraComponent*) const;

private:
	std::vector<std::vector<DrawComponent*>> m_x_DrawOrder ={};

};
#endif