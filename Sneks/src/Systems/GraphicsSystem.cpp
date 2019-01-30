#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{
	
};


GraphicsSystem::~GraphicsSystem()
{
	for (auto pairing : m_x_textureMap)
	{
		AEGfxTextureUnload(pairing.second);
	}
}

void GraphicsSystem::Initialize(Camera* camera)
{
	m_px_Camera = camera;

}

AEGfxTexture* GraphicsSystem::FetchTexture(const char* textureName)
{
	AEGfxTexture* retTexture = m_x_textureMap.find(textureName)->second;
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
	m_x_textureMap.insert(std::pair<const char*, AEGfxTexture*>(textureName,texture));
}

void GraphicsSystem::Draw(float dt)
{

}
