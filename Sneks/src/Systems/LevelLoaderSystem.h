/* Start Header ***************************************************************/
/*!TODO
\file InvulnerableComponent.h
\author Lim Chu Yan, chuyan.lim, 440002918 
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 06/04/2019
\brief This file contains 

\par Contribution (hours): CY - 3

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

