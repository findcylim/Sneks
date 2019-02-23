#ifndef LEVEL_EDITOR_SYSTEM_H
#define LEVEL_EDITOR_SYSTEM_H

#pragma once

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Systems/GraphicsSystem.h"
#include "../Utility/Logger.h"
#include "../Math/Aabb.h"
#include <Windows.h>
#include <vector>
#include <map>

enum SnappingState
{
	kSnapXGrid,
	kSnapYGrid,
	kSnapBothAxis,
	kSnapXObject,
	kSnapYObject,
	kSnapNone,
};

enum Objects
{
	kBuildingObj,
	kVerticalRoadObj,
	kHorizontalRoadObj,
	kJunctionObj,
	kParkObj,
	kBlankState,
	kNumberOfObjects
};


class LevelEditorSystem : public BaseSystem
{


public:
	LevelEditorSystem(EntityManager* entityManagerPtr, Logger* logger, GraphicsSystem* graphics);
	~LevelEditorSystem();
	void Update(float dt) override;

private:
	
	EventManager*		m_o_EventManager;
	GameStateManager*	m_o_GameStateManager;
	Logger*				m_o_Logger;
	GraphicsSystem*		m_o_GraphicsSystem;
	std::vector<StaticObjectEntity*> m_v_PrefabVector;
	std::map<Objects, std::list<StaticObjectEntity*>> m_x_ToSavePrefabMap;
	std::list<StaticObjectEntity*> m_x_SelectedList, m_x_CopiedList;

	StaticObjectEntity* SelectedObject = nullptr

	StaticObjectEntity* CreateNewPrefab(float posX, float posY, 
										float sizeX, float sizeY, 
										const char* textureName, 
										const char * objectName);

	StaticObjectEntity* CheckPointCollideWithAnything(HTVector2 pos);
	StaticObjectEntity* CheckNearestSnap(Aabb& boundsCheck);
	void ClearSelected();
	void AddToSelected(StaticObjectEntity* object);
	bool Listfind(StaticObjectEntity* object);

	bool isTabPressed = false;
	bool isMiddleMouseClicked = false;
	bool isLeftMouseClicked = false;
	bool isSaved = false;
	bool isLeftShift = false;
	bool isCapsLock = false;
	bool isGridLock = false;
	bool isDrawSelection = false;
	bool isLeft = false, isRight = false, isUp = false, isDown = false;
	bool isCopied = false;
	bool isCopying = false;

	char ObjCounter = kBuildingObj;
	char orientationCheck = -1;

	SnappingState SnapState = kSnapNone;

	HWND windowHandle;
	RECT windowRect; 


	HTVector2 lastClickPosition = { 0,0 };
	HTVector2 copiedPosition = { 0,0 };
	POINT initialMousePos, currentMousePos;



















}

#endif