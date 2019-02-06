#include "LevelLoaderSystem.h"
#include <iostream>
#include <fstream>
#include <string>
#include <typeindex>

// Will handle loading and unloading of StaticObjects and Background Objects

LevelLoaderSystem::LevelLoaderSystem(EntityManager* entityManagerPtr, EventManager* eventManager, GameStateManager* gameStateManager)
	:BaseSystem(entityManagerPtr)
{
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

	StaticObjectEntity* newEntity = (StaticObjectEntity*)m_po_EntityManager->NewEntity(kEntityStaticObject,"Test");
	while (std::getline(inFile, output))
	{
		if (strcmp(output.c_str(), "<Entity>"))
		{
			const char* cPointer;
			std::getline(inFile, output);
			if (strncmp(output.c_str(), "\t<EntityType=StaticObject>", 26))
			{
				std::getline(inFile, output);
				cPointer = output.c_str();
				while (*(cPointer++) != '=');

				int nameLen = strlen(cPointer);
				char * tempName = new char[nameLen - 1];
				strncpy_s(tempName, nameLen, cPointer, nameLen - 1);
				StaticObjectEntity* newEntity = (StaticObjectEntity*)m_po_EntityManager->NewEntity(kEntityStaticObject, tempName);
				delete[] tempName;
			}
			else
			{
				std::getline(inFile, output);
				cPointer = output.c_str();
				while (*(cPointer++) != '=');

				int nameLen = strlen(cPointer);
				char * tempName = new char[nameLen - 1];
				strncpy_s(tempName, nameLen, cPointer, nameLen - 1);
				BackgroundEntity* newEntity = (BackgroundEntity*)m_po_EntityManager->NewEntity(kEntityBackground, tempName);

				delete[] tempName;
			}
			

			std::getline(inFile, output);
			cPointer = output.c_str();
			while (*(cPointer++) != '=');
			for (auto comp : newEntity->m_v_AttachedComponentsList)
			{
				if (comp->m_x_ComponentID == kComponentTransform)
				{
					float x = atof(cPointer);
					while (*(cPointer++) != '=');
					float y = atof(cPointer);
					static_cast<TransformComponent*>(comp)->SetPosition(x,y);

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=');
					x = atof(cPointer);
					static_cast<TransformComponent*>(comp)->SetRotation(x);
				}
			}
			std::getline(inFile, output);
			cPointer = output.c_str();
			while (*(cPointer++) != '=');
			for (auto comp : newEntity->m_v_AttachedComponentsList)
			{
				if (comp->m_x_ComponentID == kComponentDraw)
				{
					float x = atof(cPointer);
					while (*(cPointer++) != '=');
					float y = atof(cPointer);
					static_cast<DrawComponent*>(comp)->m_x_MeshSize.x = x;
					static_cast<DrawComponent*>(comp)->m_x_MeshSize.y = y;

					std::getline(inFile, output);
					cPointer = output.c_str();
					while (*(cPointer++) != '=');
					int len = strlen(cPointer);
					char * texName = new char[len];
					strncpy_s(texName, len -1 , cPointer, len-1);
					texName[len - 1] = '\0';
					static_cast<DrawComponent*>(comp)->m_px_Texture = AEGfxTextureLoad(texName); //Change this to texture pool later TODO;
				}
			}
			std::getline(inFile, output);
		}
	}

	return true;
}