#pragma once
#include <map>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"
#include "../ECS/System.h"

class GraphicsSystem : public BaseSystem
{
public:
	std::multimap<const char*, AEGfxTexture*> m_x_textureMap;

	GraphicsSystem();
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize();
	AEGfxTexture* FetchTexture(const char* textureName);
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Draw(float dt);
};
