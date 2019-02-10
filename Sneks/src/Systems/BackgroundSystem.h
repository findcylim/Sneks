#ifndef BACKGROUND_SYSTEM_H
#define BACKGROUND_SYSTEM_H
#pragma once
#include "BackgroundSystem.h"
#include "../ECS/System.h"
#include "GraphicsSystem.h"

class BackgroundSystem :
	public BaseSystem
{
public:
	BackgroundSystem(EntityManager* entityManagerPtr, GraphicsSystem* graphics);
	void Update(float dt);
	void Initialize();
	void CreateBackground(float posX, float posY, float sizeX, float sizeY, const char* textureName) const;
	void CreateInstancedBackgrounds(int instancesX, int instancesY, const char* textureName);
	~BackgroundSystem() = default;

private:
	GraphicsSystem* m_o_GraphicsSystem;
};

#endif //BACKGROUND_SYSTEM_H