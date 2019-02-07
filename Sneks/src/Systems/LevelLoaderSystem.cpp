#include "LevelLoaderSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <typeindex>

// Will handle loading and unloading of StaticObjects and Background Objects

LevelLoaderSystem::LevelLoaderSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GameStateManager* gameStateManager,GraphicsSystem* graphicsSystem)
	:BaseSystem(entityManagerPtr)
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
				int nameLen = strlen(cPointer);
				char tempName[100];
				strncpy_s(tempName, nameLen, cPointer,nameLen - 1);
				newEntity = (StaticObjectEntity*)m_po_EntityManager->NewEntity(kEntityStaticObject, tempName);
			}
			else
			{
				std::getline(inFile, output);
				cPointer = output.c_str();
				while (*(cPointer++) != '=') {}
				int nameLen = strlen(cPointer);
				char tempName[100];
				strncpy_s(tempName, nameLen, cPointer, nameLen - 1);
				newEntity = (BackgroundEntity*)m_po_EntityManager->NewEntity(kEntityBackground, tempName);
			}
			

			std::getline(inFile, output);
			cPointer = output.c_str();
			while (*(cPointer++) != '=') {}
			for (auto comp : newEntity->m_v_AttachedComponentsList)
			{
				if (comp->m_x_ComponentID == kComponentTransform)
				{
					float x = atof(cPointer);
					while (*(cPointer++) != '=') {}
					float y = atof(cPointer);
					static_cast<TransformComponent*>(comp)->SetPosition(x,y);

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=') {}
					x = atof(cPointer);
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
					float x = atof(cPointer);
					while (*(cPointer++) != '=') {}
					float y = atof(cPointer);

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=') {}
					int len = strlen(cPointer);
					char texName[100];
					strncpy_s(texName, len, cPointer, len - 1);
					//Change this to texture pool later TODO;

					static_cast<DrawComponent*>(comp)->Initialize(m_o_GraphicsSystem->FetchTexture(texName), x, y, { 1,1,1,1 });

					break;
				}
			}

			std::getline(inFile, output);
		}
	}
	m_po_EntityManager->NewEntity(kEntityCamera, "Camera");
	return true;
}