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

#include "LevelLoaderSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <typeindex>

// Will handle loading and unloading of StaticObjects and Background Objects

LevelLoaderSystem::LevelLoaderSystem( EventManager* eventManager, GameStateManager* gameStateManager,GraphicsSystem* graphicsSystem)
{
	m_o_GraphicsSystem = graphicsSystem;
	m_o_EventManager = eventManager;
	m_o_GameStateManager = gameStateManager;
}


LevelLoaderSystem::~LevelLoaderSystem()
{
}


void LevelLoaderSystem::Update(float dt)
{
	UNREFERENCED_PARAMETER(dt);
}

bool LevelLoaderSystem::LoadLevel(LevelID levelID)
{
	std::string levelName;
	
	switch (levelID)
	{
	case kLevel1:
		levelName = "../Resources/Levels/Level1.xml";
		break;
	case kLevel2:
		levelName = "../Resources/Levels/Level2.xml";
		break;
	}
	std::fstream inFile;
	std::string output;
	inFile.open(levelName);
	if (!inFile.is_open())
		return false;

	while (std::getline(inFile, output))
	{
		if (strcmp(output.c_str(), "<Entity>") == 0)
		{
			const char* cPointer;
			std::getline(inFile, output);
			BaseEntity* newEntity;
			if (strncmp(output.c_str(), "\t<EntityType=StaticObject>", 26) == 0)
			{
				std::getline(inFile, output);
				cPointer = output.c_str();
				while (*(cPointer++) != '=') {}
				auto nameLen = strlen(cPointer);
				char tempName[100];
				strncpy_s(tempName, nameLen, cPointer,nameLen - 1);
				newEntity = m_po_EntityManager->NewEntity<StaticObjectEntity>(kEntityStaticObject, tempName);
			}
			else
			{
				std::getline(inFile, output);
				cPointer = output.c_str();
				while (*(cPointer++) != '=') {}
				auto nameLen = strlen(cPointer);
				char tempName[100];
				strncpy_s(tempName, nameLen, cPointer, nameLen - 1);
				newEntity = m_po_EntityManager->NewEntity<BackgroundEntity>(kEntityBackground, tempName);
			}
			

			std::getline(inFile, output);
			cPointer = output.c_str();
			while (*(cPointer++) != '=') {}
			for (auto comp : newEntity->m_v_AttachedComponentsList)
			{
				if (comp->m_x_ComponentID == kComponentTransform)
				{
					float x = static_cast<float>(atof(cPointer));
					while (*(cPointer++) != '=') {}
					float y = static_cast<float>(atof(cPointer));
					static_cast<TransformComponent*>(comp)->m_x_Position.x = (x);
					static_cast<TransformComponent*>(comp)->m_x_Position.y=(y);

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=') {}
					x = static_cast<float>(atof(cPointer));
					static_cast<TransformComponent*>(comp)->SetRotation(x);
					break;
				}
			}
			std::getline(inFile, output);
			cPointer = output.c_str();
			while (*(cPointer++) != '=') {}
			for (auto comp : newEntity->m_v_AttachedComponentsList)
			{
				if (comp->m_x_ComponentID == kComponentDraw)
				{
					//TODO REMOVE SIZE FROM FILE INFO (IT IS NOW AUTO GENERATED)
					auto x = atof(cPointer);
					while (*(cPointer++) != '=') {}
					auto y = atof(cPointer);
					UNREFERENCED_PARAMETER(x);
					UNREFERENCED_PARAMETER(y);

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=') {}
					auto len = strlen(cPointer);
					char texName[100];
					strncpy_s(texName, len, cPointer, len - 1);
					//Change this to texture pool later TODO;

					m_o_GraphicsSystem->InitializeDrawComponent(static_cast<DrawComponent*>(comp), texName);

					break;
				}
			}

			std::getline(inFile, output);
		}
	}
	m_po_EntityManager->NewEntity<CameraEntity>(kEntityCamera, "Camera");
	return true;
}