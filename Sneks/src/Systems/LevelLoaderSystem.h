/* Start Header****************************************************************/
/*!
\file LevelLoaderSystem.h
\author Primary Author : Muhammad Adam Bin Mohamad Amin, m.muhammadadambin
\par email: m.muhammadadambin\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains deprecated/for future use parsing code for a level
editor saved .xml files

\par Contribution : Adam   - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

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

