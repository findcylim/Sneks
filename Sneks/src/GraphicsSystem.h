#pragma once
#include <map>
#include <AEEngine.h>
#include "Graphics/Camera.h"
#include "ECS/System.h"

class GraphicsSystem : public BaseSystem
{
private:
	Camera*				        m_px_Camera;
	std::vector<DrawObject*>  m_x_DrawObjectVec;

public:
	std::multimap<const char*, AEGfxTexture*> m_x_textureMap;

	GraphicsSystem();
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize(Camera*);
	AEGfxTexture* FetchTexture(const char* textureName);
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Draw(float dt);
};
