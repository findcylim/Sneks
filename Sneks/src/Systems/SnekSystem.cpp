#include "SnekSystem.h"

SnekSystem::SnekSystem(EntityManager* entityManagerPtr) : BaseSystem(entityManagerPtr) {};


void SnekSystem::Update(float dt)
{
}

void SnekSystem::Initialize()
{
}

void SnekSystem::CreateSnek(const int numBodyParts, float posX, float posY,
	AEGfxTexture* snakeHeadTexture, AEGfxTexture* snakeBodyTexture)
{
	
}