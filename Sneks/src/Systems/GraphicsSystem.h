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
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Update(float dt) override;
	void Draw(float dt) const;
	void UpdateMatrices(CameraComponent*) const;
};
#endif