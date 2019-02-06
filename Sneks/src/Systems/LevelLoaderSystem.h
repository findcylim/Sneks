#pragma once
#include "../ECS/System.h"
#include "../Utility/GameStateManager.h"

enum LevelID
{
	kLevel1,
	kLevel2
};

class LevelLoaderSystem :
	public BaseSystem
{
public:
	LevelLoaderSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GameStateManager* gameStateManager);
	~LevelLoaderSystem();
	bool LoadLevel(LevelID levelID);
	void Update(float dt) override;
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
};

