#pragma once
#include "../ECS/System.h"
#include "../Utility/GameStateManager.h"
#include "GraphicsSystem.h"

enum LevelID
{
	kLevel1,
	kLevel2
};

class LevelLoaderSystem :
	public BaseSystem
{
	GraphicsSystem* m_o_GraphicsSystem;
public:
	LevelLoaderSystem( EventManager* eventManager, GameStateManager* gameStateManager,GraphicsSystem* graphicsSystem);
	~LevelLoaderSystem();
	bool LoadLevel(LevelID levelID);
	void Update(float dt) override;
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
};

