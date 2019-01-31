#pragma once
#include <map>
#include <AEEngine.h>
#include "../Components/CameraComponent.h"
#include "../ECS/System.h"
#include "../ECS/EntityManager.h"

class GraphicsSystem : public BaseSystem
{
public:
	std::multimap<const char*, AEGfxTexture*> m_x_textureMap;
	EntityManager*		m_po_EntityManager;
	ComponentManager* m_po_ComponentManager;

	GraphicsSystem();
	GraphicsSystem(GraphicsSystem&) = delete;
	~GraphicsSystem();
	void Initialize(EntityManager* entityManager);
	AEGfxTexture* FetchTexture(const char* textureName);
	void PreLoadTextures();
	void LoadTextureToMap(const char* fileName, const char* textureName);
	void Draw(float dt);
};
