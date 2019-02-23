#ifndef LEVEL_EDITOR_SYSTEM_H
#define LEVEL_EDITOR_SYSTEM_H

#pragma once

#include "../ECS/System.h"
#include "../ECS/EntityManager.h"
#include "../Systems/GraphicsSystem.h"
#include "../Utility/GameStateManager.h"
#include "../ECS/EventManager.h"
#include "../Utility/Logger.h"
#include "../Math/Aabb.h"

#include "../../Dependencies/glew/include/GL/glew.h"
#include "../../Dependencies/imgui-1.68/imgui.h"
#include "../../Dependencies/imgui-1.68/imgui_impl_opengl3.h"
#include "../../Dependencies/imgui-1.68/imgui_internal.h"

#include "AEEngine.h"
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
	std::map<Objects, std::vector<StaticObjectEntity*>> m_x_ToSavePrefabMap;
	std::list<StaticObjectEntity*> m_x_SelectedList, m_x_CopiedList;
	StaticObjectEntity* selectionSquare;

	StaticObjectEntity* SelectedObject = nullptr;
	StaticObjectEntity* ChosenObject = nullptr;

	StaticObjectEntity* CreateNewPrefab(float posX, float posY, 
										float sizeX, float sizeY, 
										const char* textureName, 
										const char * objectName);
	StaticObjectEntity* CopyPrefab(StaticObjectEntity& rhs);

	StaticObjectEntity* CheckPointCollideWithAnything(HTVector2 pos);
	StaticObjectEntity* CheckNearestSnap(Aabb& boundsCheck);
	void SnapToOrientation(bool hardSnap, HTVector2& positionToSnap, SnappingState& snapState,
		StaticObjectEntity* closestObj, float sizeX, float sizeY);

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

	HTVector2 lastMousePosition = { 0,0 };
	HTVector2 lastClickPosition = { 0,0 };
	HTVector2 copiedPosition = { 0,0 };
	POINT initialMousePos, currentMousePos;

	int font = AEGfxCreateFont("Arial", 20, false, false);
	////imGui Stuff
	//HGLRC   g_GLRenderContext;
	//HDC     g_HDCDeviceContext;
	//ImVec4 clear_color = ImVec4(0.000F, 1.000F, 1.000F, 1.0F);

};

#endif