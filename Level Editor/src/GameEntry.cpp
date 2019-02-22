#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <map>
#include <string>
#include <fstream>
#include "Graphics/DrawObject.h"
#include "Graphics/SimpleDraw.h"
#include "Math/HTVector2.h"
#include "AEEngine.h"
#include "Math/Aabb.h"
#include <sys/stat.h>
#include <Windows.h>
#include <vector>
#include <list>
#include <commdlg.h>

#include "Utility/AlphaEngineHelper.h"
#include "ECS/ECSystem.h"

constexpr int kNumBodyParts = 20;

float DrawObject::m_f_GlobalScale = 1.0f;
float DrawObject::m_f_GlobalCameraOffsetX = 0.0f;
float DrawObject::m_f_GlobalCameraOffsetY = 0.0f;

float SimpleDraw::m_f_GlobalScale = 1.0f;
float SimpleDraw::m_f_GlobalCameraOffsetX = 0.0f;
float SimpleDraw::m_f_GlobalCameraOffsetY = 0.0f;

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

enum SnappingState
{
	kSnapXGrid,
	kSnapYGrid,
	kSnapBothAxis,
	kSnapXObject,
	kSnapYObject,
	kSnapNone,
};

bool isfileExists(const std::string& file)
{
	struct stat buffer;
	if (stat(file.c_str(), &buffer) != -1)
	{
		return true;
	}
	return false;
}

/*
	1- Left
	2- Top
	3- Right
	4- Bottom
*/
char CalculateOrientation(Aabb& main, Aabb& orientationTo)
{
	if (main.min.x > orientationTo.max.x)
		return 3; // Right
	if (main.min.y > orientationTo.max.y)
		return 2; //Top
	if (orientationTo.min.x > main.max.x)
		return 1; // Left
	if (orientationTo.min.y > main.max.y)
		return 4; //Bottom
	return -1;
}




float Distance(HTVector2 lhs, HTVector2 rhs)
{
	return (abs((lhs.x - rhs.x)*(lhs.x - rhs.x)) + abs((lhs.y - rhs.y)*(lhs.y - rhs.y)));
}

float DistanceX(HTVector2 lhs, HTVector2 rhs)
{
	return abs(lhs.x - rhs.x);
}

float DistanceY(HTVector2 lhs, HTVector2 rhs)
{
	return abs(lhs.y - rhs.y);
}

void AEInit(HINSTANCE hInstance,int nCmdShow)
{
	AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 300, false, nullptr);
	AESysSetWindowTitle("Level Editor");
	AEToogleFullScreen(false);
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 1);
}

void RenderObjects(std::vector<DrawObject*> objs)
{
	for (auto obj : objs)
	{
		obj->Draw();
	}
}

DrawObject* CheckNearestSnap(std::map<Objects,std::vector<DrawObject*>>& AllObjects,Aabb& boundsCheck)
{
	DrawObject* closestObjX = nullptr;
	DrawObject* closestObjY = nullptr;
	DrawObject* closestObj = nullptr;
	float shortestDistX = -1;
	float shortestDistY = -1;
	float shortestDist = -1;
	HTVector2 boundOrigin = { boundsCheck.min.x + (boundsCheck.max.x - boundsCheck.min.x)/2,
							  boundsCheck.min.y + (boundsCheck.max.y - boundsCheck.min.y)/2 };
	for (auto iterator = AllObjects.begin(); iterator != AllObjects.end(); ++iterator)
	{
		for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
		{
			Aabb iterCheck = { (*innerIter)->GetMin(),(*innerIter)->GetMax() };
			if (AabbHelper::CheckAabbIntersect(&boundsCheck, &iterCheck))
			{
				//continue;
			}

			float distX = DistanceX(boundOrigin, (*innerIter)->GetPosition());
			float distY = DistanceY(boundOrigin, (*innerIter)->GetPosition());
			float Dist = Distance(boundOrigin, (*innerIter)->GetPosition());
			if (shortestDist < 0)
			{
				shortestDistX = distX;
				shortestDistY = distY;
				closestObjY = closestObj = closestObjX = *innerIter;
				shortestDist = Dist;
			}
			else if (Dist < shortestDist)
			{
				shortestDist = Dist;
				closestObj = *innerIter;
				if ((shortestDistX < 0 && shortestDistY < 0) || distX >= 0 && distY >= 0)
				{
					if (distX < shortestDistX)
					{
						if (distY == shortestDistY)
						{
							closestObjY = *innerIter;
						}
						shortestDistX = distX;
						closestObjX = *innerIter;
					}
					if (distY < shortestDistY)
					{
						if (distX == shortestDistX)
						{
							closestObjX = *innerIter;
						}
						shortestDistY = distY;
						closestObjY = *innerIter;
					}
					if (shortestDistX < 0 && shortestDistY < 0)
					{
						shortestDistX = distX;
						shortestDistY = distY;
						closestObjX = *innerIter;
						closestObjY = *innerIter;
					}
				}
			}
		}
	}

	if (closestObjX == closestObjY && closestObjY == closestObj)
		return closestObjX;

	if ((DistanceX(boundOrigin, closestObjY->GetPosition()) - (closestObjY->GetSizeY() / 2 + (boundOrigin.y - boundsCheck.min.y))) >
		(DistanceY(boundOrigin, closestObjX->GetPosition()) - (closestObjX->GetSizeX() / 2 + (boundOrigin.x - boundsCheck.min.x))))
		return closestObjX;
	else
		return closestObjY;
}

void SnapToOrientation(char orientationCheck,bool hardSnap,HTVector2& positionToSnap,SnappingState& snapState,DrawObject* closestObj,float sizeX,float sizeY)
{
	float xDiffClose, yDiffClose;
	float shortestDist = -1;
	float offset = 50;
	switch (orientationCheck)
	{
	case 1:
		//Left
		if (hardSnap)
		{
			positionToSnap.y = closestObj->GetPosition().y;
			positionToSnap.x = closestObj->GetPosition().x - closestObj->GetSizeX() / 2 - sizeX / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = DistanceX(positionToSnap, closestObj->GetPosition());
			if (shortestDist < ((xDiffClose = sizeX / 2 + closestObj->GetSizeX() / 2) + offset))
			{
				positionToSnap.x += shortestDist - xDiffClose;
				snapState = kSnapXObject;
			}
		}
		break;
	case 2:
		//Top
		if (hardSnap)
		{
			positionToSnap.x = closestObj->GetPosition().x;
			positionToSnap.y = closestObj->GetPosition().y + sizeY / 2 + closestObj->GetSizeY() / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = DistanceY(positionToSnap, closestObj->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((yDiffClose = sizeY / 2 + closestObj->GetSizeY() / 2) + offset))
			{
				snapState = kSnapYObject;
				positionToSnap.y -= shortestDist - yDiffClose;
			}
		}
		break;
	case 3:
		//Right

		if (hardSnap)
		{
			positionToSnap.y = closestObj->GetPosition().y;
			positionToSnap.x = closestObj->GetPosition().x + closestObj->GetSizeX() / 2 + sizeX / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = DistanceX(positionToSnap, closestObj->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((xDiffClose = sizeX / 2 + closestObj->GetSizeX() / 2) + offset))
			{
				snapState = kSnapXObject;
				positionToSnap.x -= shortestDist - xDiffClose;
			}
		}
		break;
	case 4:
		//Bottom

		if (hardSnap)
		{
			positionToSnap.x = closestObj->GetPosition().x;
			positionToSnap.y = closestObj->GetPosition().y - sizeY / 2 - closestObj->GetSizeY() / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = DistanceY(positionToSnap, closestObj->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((yDiffClose = sizeY / 2 + closestObj->GetSizeY() / 2) + offset))
			{
				snapState = kSnapYObject;
				positionToSnap.y += shortestDist - yDiffClose;
			}
		}
		break;
	}
}


DrawObject* CheckPointCollideWithAnything(std::map<Objects, std::vector<DrawObject*>>& AllObjects, HTVector2 pos)
{
	for (auto iterator = AllObjects.begin(); iterator != AllObjects.end(); ++iterator)
	{
		for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
		{
			Aabb currentSelectionAabb = {};
			currentSelectionAabb.min = (*innerIter)->GetMin();
			currentSelectionAabb.max = (*innerIter)->GetMax();

			if (AabbHelper::CheckPointAabbIntersect(&currentSelectionAabb, pos))
			{
				return *innerIter;
			}
		}
	}
	return nullptr;
}

void ClearSelected(std::list<DrawObject*>& SelectedList)
{
	for (auto iter = SelectedList.begin(); iter != SelectedList.end(); iter++)
	{
		(*iter)->SetColor(1, 1, 1, 1);
	}
	SelectedList.clear();
}

void AddToSelected(std::list<DrawObject*>& SelectedList, DrawObject* object)
{
	object->SetColor(0.5f, 1, 0.5f, 1);
	SelectedList.push_back(object);
}

bool Listfind(std::list<DrawObject*>& SelectedList, DrawObject* object)
{
	for (auto Object : SelectedList)
	{
		if (Object == object)
		{
			return true;
		}
	}
	return false;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	AlphaEngineHelper::AlphaInitialize(hInstance, nCmdShow);
	ECSystem* Engine = new ECSystem;
	Engine->InitializeEngine();

	//MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);

	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}

	delete(Engine);

	return 0;



	//srand(static_cast<unsigned int>(time(nullptr)));

	//
	//HWND windowHandle = AESysGetWindowHandle();
	//RECT windowRect; 

	//auto horRoad = new DrawObject(0, 0, 71, 9, horizontalRoadTexture,"Horizontal Road","horz-road.png");
	//auto verRoad = new DrawObject(100, 100, 9, 42, verticalRoadTexture, "Vertical Road","vert-road.png");
	//auto buildingObj = new DrawObject(0, 0, 71, 42, buildingTexture,"Building1","building.png");
	//auto selectionSquare = new DrawObject(0, 0, 1, 1,selectionSquareTexture, "Selection Square","SelectionSquare.png");
	//auto junctionObj = new DrawObject(0, 0, 9, 9, junctionTexture, "Junction", "junction.png");
	//auto parkObj = new DrawObject(0, 0, 71, 42, parkTexture, "Park", "park.png");

	///********************************************************************************************************************/
	////Editor UI Objects

	//auto EditorInfoPanelUI = new DrawObject(0, 0, 1920, 1080, infoPanelTexture, "InfoPanelUI", "EditorInfoPanel.png");

	///********************************************************************************************************************/

	//DrawObject* SelectedObject = nullptr;

	//HTVector2 lastClickPosition = { 0,0 };
	//HTVector2 copiedPosition = { 0,0 };
	//POINT initialMousePos, currentMousePos;

	//std::vector<DrawObject*> PrefabVector;
	//std::map<Objects, std::vector<DrawObject*>> ToSavePrefabMap;
	//std::list<DrawObject*> selectedList, copiedList;

	//PrefabVector.push_back(buildingObj);
	//PrefabVector.push_back(verRoad);
	//PrefabVector.push_back(horRoad);
	//PrefabVector.push_back(junctionObj);
	//PrefabVector.push_back(parkObj);


	//bool isTabPressed = false;
	//bool isMiddleMouseClicked = false;
	//bool isLeftMouseClicked = false;
	//bool isSaved = false;
	//bool isLeftShift = false;
	//bool isCapsLock = false;
	//bool isGridLock = false;
	//bool isDrawSelection = false;
	//bool isLeft = false, isRight = false, isUp = false, isDown = false;
	//bool isCopied = false;
	//bool isCopying = false;

	//char ObjCounter = kBuildingObj;
	//char orientationCheck = -1;

	//SnappingState SnapState = kSnapNone;

	//auto font = AEGfxCreateFont("Arial", 20, false, false);
	//auto winFont = AEGfxCreateFont("Arial", 500, 1, 0);

	//float zoomLimitMin = 0.8f, zoomLimitMax = 1.2f;
	//float currentCamPosX, currentCamPosY,defaultCamPosX, defaultCamPosY;
	//float ScreenSizeX = (AEGfxGetWinMaxX() - AEGfxGetWinMinX())/2;
	//float ScreenSizeY = (AEGfxGetWinMaxY() - AEGfxGetWinMinY())/2;

	//AEGfxGetCamPosition(&currentCamPosX, &currentCamPosY);
	//AEGfxGetCamPosition(&defaultCamPosX, &defaultCamPosY);

	//int winner = 0;




	//while (!winner) {

	//	AESysFrameStart();
	//	AEInputUpdate();

	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//	AEGfxSetBlendMode(AE_GFX_BM_NONE);

	//	char count;

	//	for (auto& i_Backgrounds : background) {
	//		i_Backgrounds->Draw();
	//	}

	//	s8 chars3[100] = {};
	//	sprintf_s(chars3, 100, "+");
	//	AEGfxPrint(font, chars3, -3, -9, 0, 0, 0);

	//	GetWindowRect(windowHandle, &windowRect);

	//	GetCursorPos(&currentMousePos);

	//	if (GetAsyncKeyState(4) < 0)
	//	{
	//		if (isMiddleMouseClicked == false)
	//		{
	//			isMiddleMouseClicked = true;
	//			GetCursorPos(&initialMousePos);
	//		}

	//		float diffX = initialMousePos.x - currentMousePos.x;
	//		float diffY = initialMousePos.y - currentMousePos.y;

	//		currentCamPosX = defaultCamPosX + diffX;
	//		currentCamPosY = defaultCamPosY - diffY;

	//		AEGfxSetCamPosition(currentCamPosX, currentCamPosY);
	//	}
	//	else
	//	{
	//		if (isMiddleMouseClicked)
	//		{
	//			isMiddleMouseClicked = false;
	//			defaultCamPosX = currentCamPosX;
	//			defaultCamPosY = currentCamPosY;
	//		}
	//	}

	//	
	//	if (GetAsyncKeyState(VK_ESCAPE) < 0)
	//	{
	//		ObjCounter = kBlankState;
	//		for (auto iter = selectedList.begin(); iter != selectedList.end(); iter++)
	//		{
	//			(*iter)->SetColor(1, 1, 1, 1);
	//		}
	//		selectedList.clear();
	//	}

	//	if (GetAsyncKeyState(AEVK_CAPSLOCK) < 0)
	//	{
	//		if (!isCapsLock)
	//		{
	//			isGridLock = !isGridLock;
	//		}
	//		isCapsLock = true;
	//	}
	//	else
	//	{
	//		isCapsLock = false;
	//	}

	//	HTVector2 DrawPosition{ (static_cast<float>((currentCamPosX - ScreenSizeX) + currentMousePos.x  ) - windowRect.left )* DrawObject::m_f_GlobalScale
	//						   ,(static_cast<float>((currentCamPosY + ScreenSizeY) - currentMousePos.y+14 + windowRect.top )* DrawObject::m_f_GlobalScale )};
	//	
	//	if (GetAsyncKeyState(VK_TAB) < 0)
	//	{
	//		if (!isTabPressed)
	//		{
	//			isTabPressed = true;
	//			ObjCounter++;
	//			if (ObjCounter >= kNumberOfObjects)
	//			{
	//				ObjCounter = 0;
	//			}
	//		}
	//	}
	//	else 
	//	{
	//		isTabPressed = false;
	//		char count = 0;

	//		for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin();iter<PrefabVector.end();++iter,++count)
	//		{
	//			if (count == ObjCounter)
	//			{
	//				if (ObjCounter == kBlankState)
	//					break;
	//				if (GetAsyncKeyState(VK_LCONTROL) < 0)
	//				{
	//					if (GetAsyncKeyState(VK_LSHIFT) < 0)
	//					{
	//						DrawPosition.y = (DrawPosition.y - (static_cast<int>(DrawPosition.y) % 50)) + ((*iter)->GetSizeY() / 2);
	//						SnapState = kSnapYGrid;
	//					}
	//					else
	//					{
	//						if (DrawPosition.x < 0)
	//							DrawPosition.x = (DrawPosition.x - (50+ (static_cast<int>(DrawPosition.x) % 50) )) + ((*iter)->GetSizeX() / 2);
	//						else
	//							DrawPosition.x = (DrawPosition.x - (static_cast<int>(DrawPosition.x) % 50)) + ((*iter)->GetSizeX() / 2);

	//						SnapState = kSnapXGrid;
	//					}
	//				}
	//				else if (GetAsyncKeyState(VK_LSHIFT) < 0)
	//				{
	//					Aabb currentSelectionAabb ={ (*iter)->GetMin(),(*iter)->GetMax() };

	//					float shortestDist = -1;
	//					float xDiffClose, yDiffClose;

	//					DrawObject* closestObj = CheckNearestSnap(ToSavePrefabMap,currentSelectionAabb);
	//					
	//					if (closestObj)
	//					{
	//						Aabb closestIter = { closestObj->GetMin() ,closestObj->GetMax() };
	//						if (isLeftShift == false)
	//						{
	//							//First Time Pressing Shift
	//							isLeftShift = true;

	//							char check = CalculateOrientation(currentSelectionAabb, closestIter);
	//							if (check != -1)
	//								if (check != orientationCheck)
	//									orientationCheck = check;
	//						}
	//						SnapToOrientation(orientationCheck, isGridLock, DrawPosition, SnapState, closestObj, (*iter)->GetSizeX(), (*iter)->GetSizeY());
	//					}
	//				}
	//				else
	//				{
	//					isLeftShift = false;
	//					SnapState = kSnapNone;
	//				}

	//				(*iter)->SetPosition(DrawPosition.x,DrawPosition.y);
	//				(*iter)->Draw();
	//				break;
	//			}
	//		}
	//	}

	//	/**********************************************************************************/
	//	// Zooming in and Out
	//	// Still doesnt scale properly yet TODO
	//	/**********************************************************************************/
	//	if (GetAsyncKeyState(AEVK_MINUS) < 0)
	//	{
	//		if (DrawObject::m_f_GlobalScale >= zoomLimitMin)
	//		{
	//			DrawObject::m_f_GlobalScale -= 1 * AEFrameRateControllerGetFrameTime();
	//		}
	//		else
	//		{
	//			DrawObject::m_f_GlobalScale = zoomLimitMin;
	//		}
	//	}
	//	else if (GetAsyncKeyState(AEVK_EQUAL) < 0)
	//	{
	//		if (DrawObject::m_f_GlobalScale <= zoomLimitMax)
	//		{
	//			DrawObject::m_f_GlobalScale += 1 * AEFrameRateControllerGetFrameTime();
	//		}
	//		else
	//		{
	//			DrawObject::m_f_GlobalScale = zoomLimitMax;
	//		}
	//	}
	//	/**********************************************************************************/

	//	/**********************************************************************************/
	//	// Move individual objects
	//	/**********************************************************************************/
	//	if (SelectedObject)
	//	{
	//		SelectedObject->SetColor(0.5,1.0f,0.5f,1.0f);
	//		if (GetAsyncKeyState(VK_LEFT) < 0)
	//		{
	//			SelectedObject->SetPositionX(SelectedObject->GetPosition().x - 1);
	//		}
	//		if (GetAsyncKeyState(VK_RIGHT) < 0)
	//		{
	//			SelectedObject->SetPositionX(SelectedObject->GetPosition().x + 1);
	//		}
	//		if (GetAsyncKeyState(VK_UP) < 0)
	//		{
	//			SelectedObject->SetPositionY(SelectedObject->GetPosition().y + 1);
	//		}
	//		if (GetAsyncKeyState(VK_DOWN) < 0)
	//		{
	//			SelectedObject->SetPositionY(SelectedObject->GetPosition().y - 1);
	//		}
	//	}
	//	/**********************************************************************************/

	//	/**********************************************************************************/
	//	//	Move groups of objects
	//	/**********************************************************************************/
	//	if (selectedList.size() != 0)
	//	{
	//		if (GetAsyncKeyState(VK_LEFT) < 0)
	//		{
	//			for (auto iter = selectedList.begin(); iter != selectedList.end(); iter++)
	//			{
	//				(*iter)->SetPositionX((*iter)->GetPosition().x - 100 * AEFrameRateControllerGetFrameTime());
	//			}
	//		}
	//		if (GetAsyncKeyState(VK_RIGHT) < 0)
	//		{
	//			for (auto iter = selectedList.begin(); iter != selectedList.end(); iter++)
	//			{
	//				(*iter)->SetPositionX((*iter)->GetPosition().x + 100 * AEFrameRateControllerGetFrameTime());
	//			}
	//		}
	//		if (GetAsyncKeyState(VK_UP) < 0)
	//		{
	//			for (auto iter = selectedList.begin(); iter != selectedList.end(); iter++)
	//			{
	//				(*iter)->SetPositionY((*iter)->GetPosition().y + 100 * AEFrameRateControllerGetFrameTime());
	//			}
	//		}
	//		if (GetAsyncKeyState(VK_DOWN) < 0)
	//		{
	//			for (auto iter = selectedList.begin(); iter != selectedList.end(); iter++)
	//			{
	//				(*iter)->SetPositionY((*iter)->GetPosition().y - 100 * AEFrameRateControllerGetFrameTime());
	//			}
	//		}
	//	}

	//	if (GetAsyncKeyState(1) < 0)
	//	{
	//		if (!isLeftMouseClicked)
	//		{
	//			lastClickPosition.x = DrawPosition.x;
	//			lastClickPosition.y = DrawPosition.y;

	//			isLeftMouseClicked = true;
	//			bool found = false;

	//			if (ObjCounter == kBlankState)
	//			{
	//				DrawObject* collideObject = CheckPointCollideWithAnything(ToSavePrefabMap, DrawPosition);


	//				if (collideObject)
	//				{
	//					if (GetAsyncKeyState(VK_LCONTROL) < 0)
	//					{
	//						if (!Listfind(selectedList, collideObject))
	//						{
	//							AddToSelected(selectedList, collideObject);
	//						}
	//					}
	//					else
	//					{
	//						ClearSelected(selectedList);
	//						AddToSelected(selectedList, collideObject);
	//					}
	//					found = true;
	//				}
	//				if (!found)
	//				{
	//					if (SelectedObject)
	//					{
	//						SelectedObject->SetColor(1.0f,1.0f,1.0f,1.0f);
	//					}
	//					SelectedObject = nullptr;

	//					isDrawSelection = true;
	//					selectionSquare->SetPosition(DrawPosition.x, DrawPosition.y);
	//				}
	//			}
	//			else
	//			{
	//				switch (ObjCounter)
	//				{
	//					case kBuildingObj:
	//					{
	//						auto newObject = new DrawObject(*buildingObj);
	//						auto iter = ToSavePrefabMap.find(kBuildingObj);
	//						if (iter != ToSavePrefabMap.end())
	//						{
	//							iter->second.push_back(newObject);
	//						}
	//						else
	//						{
	//							std::vector<DrawObject*> v_newDrawObject;
	//							v_newDrawObject.push_back(newObject);
	//							ToSavePrefabMap.insert({ kBuildingObj,v_newDrawObject });
	//						}
	//						break;
	//					}
	//					case kVerticalRoadObj:
	//					{
	//						auto newObject = new DrawObject(*verRoad);
	//						auto iter = ToSavePrefabMap.find(kVerticalRoadObj);
	//						if (iter != ToSavePrefabMap.end())
	//						{
	//							iter->second.push_back(newObject);
	//						}
	//						else
	//						{
	//							std::vector<DrawObject*> v_newDrawObject;
	//							v_newDrawObject.push_back(newObject);
	//							ToSavePrefabMap.insert({ kVerticalRoadObj,v_newDrawObject });
	//						}
	//						break;
	//					}
	//					case kHorizontalRoadObj:
	//					{
	//						auto newObject = new DrawObject(*horRoad);
	//						auto iter = ToSavePrefabMap.find(kHorizontalRoadObj);
	//						if (iter != ToSavePrefabMap.end())
	//						{
	//							iter->second.push_back(newObject);
	//						}
	//						else
	//						{
	//							std::vector<DrawObject*> v_newDrawObject;
	//							v_newDrawObject.push_back(newObject);
	//							ToSavePrefabMap.insert({ kHorizontalRoadObj,v_newDrawObject });
	//						}
	//						break;
	//					}
	//					case kJunctionObj:
	//					{
	//						auto newObject = new DrawObject(*junctionObj);
	//						auto iter = ToSavePrefabMap.find(kJunctionObj);
	//						if (iter != ToSavePrefabMap.end())
	//						{
	//							iter->second.push_back(newObject);
	//						}
	//						else
	//						{
	//							std::vector<DrawObject*> v_newDrawObject;
	//							v_newDrawObject.push_back(newObject);
	//							ToSavePrefabMap.insert({ kJunctionObj,v_newDrawObject });
	//						}
	//						break;
	//					}
	//					case kParkObj:
	//					{
	//						auto newObject = new DrawObject(*parkObj);
	//						auto iter = ToSavePrefabMap.find(kParkObj);
	//						if (iter != ToSavePrefabMap.end())
	//						{
	//							iter->second.push_back(newObject);
	//						}
	//						else
	//						{
	//							std::vector<DrawObject*> v_newDrawObject;
	//							v_newDrawObject.push_back(newObject);
	//							ToSavePrefabMap.insert({ kParkObj,v_newDrawObject });
	//						}
	//						break;
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			
	//		}
	//	}
	//	else
	//	{
	//		isLeftMouseClicked = false; //Potential Bug
	//		//do stufff
	//		if (isDrawSelection)
	//		{
	//			Aabb selectionBounds = {};
	//			selectionBounds.min = selectionSquare->GetMin();
	//			selectionBounds.max = selectionSquare->GetMax();

	//			ClearSelected(selectedList);

	//			for (auto iterator = ToSavePrefabMap.begin(); iterator != ToSavePrefabMap.end(); ++iterator)
	//			{
	//				for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
	//				{
	//					Aabb checkBox = { (*innerIter)->GetMin(),(*innerIter)->GetMax() };
	//					if (CheckAabbIntersect(&selectionBounds, &checkBox))
	//					{
	//						(*innerIter)->SetColor(0.5, 1.0f, 0.5f, 1.0f);
	//						selectedList.push_back((*innerIter));

	//					}
	//				}
	//			}
	//		}
	//		isDrawSelection = false;
	//	}

	//	if (GetAsyncKeyState(VK_DELETE) < 0)
	//	{
	//		if (selectedList.size() != 0)
	//		{
	//			for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
	//			{
	//				for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end();)
	//				{
	//					bool deleted = false;
	//					for (auto iterer = selectedList.begin(); iterer != selectedList.end();)
	//					{
	//						if ((*innerIter) == (*iterer))
	//						{
	//							delete *iterer;
	//							deleted = true;
	//							iterer = selectedList.erase(iterer);
	//							innerIter = (*iter).second.erase(innerIter);
	//							break;
	//						}
	//						else
	//						{
	//							++iterer;
	//						}
	//					}
	//					if(!deleted)
	//						++innerIter;
	//				}

	//			}
	//		}
	//	}

	//	for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
	//	{
	//		for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
	//		{
	//			(*innerIter)->Draw();
	//		}
	//	}

	//	if (GetAsyncKeyState(VK_LCONTROL) < 0)
	//	{
	//		if (GetAsyncKeyState(AEVK_C) < 0)
	//		{
	//			if (!isCopying)
	//			{
	//				isCopying = true;
	//				copiedList = static_cast<const std::list <DrawObject*>>(selectedList);
	//				ClearSelected(selectedList);
	//				copiedPosition.x = DrawPosition.x;
	//				copiedPosition.y = DrawPosition.y;
	//			}
	//		}
	//		else if (GetAsyncKeyState(AEVK_V) < 0)
	//		{
	//			if (copiedList.size() != 0)
	//			{
	//				if (!isCopied)
	//				{
	//					float diffX, diffY;
	//					diffX = DrawPosition.x - copiedPosition.x;
	//					diffY = DrawPosition.y - copiedPosition.y;
	//					isCopied = true;
	//					for (auto iter = copiedList.begin(); iter != copiedList.end(); iter++)
	//					{
	//						if (*iter)
	//						{
	//							if (strcmp((*iter)->GetName(), "Horizontal Road") == 0)
	//							{
	//								auto newObject = new DrawObject(*(*iter));
	//								auto iter = ToSavePrefabMap.find(kHorizontalRoadObj);
	//								if (iter != ToSavePrefabMap.end())
	//								{
	//									iter->second.push_back(newObject);
	//								}
	//								else
	//								{
	//									std::vector<DrawObject*> v_newDrawObject;
	//									v_newDrawObject.push_back(newObject);
	//									ToSavePrefabMap.insert({ kHorizontalRoadObj,v_newDrawObject });
	//								}
	//								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
	//							}
	//							else if (strcmp((*iter)->GetName(), "Vertical Road") == 0)
	//							{
	//								auto newObject = new DrawObject(*(*iter));
	//								auto iter = ToSavePrefabMap.find(kVerticalRoadObj);
	//								if (iter != ToSavePrefabMap.end())
	//								{
	//									iter->second.push_back(newObject);
	//								}
	//								else
	//								{
	//									std::vector<DrawObject*> v_newDrawObject;
	//									v_newDrawObject.push_back(newObject);
	//									ToSavePrefabMap.insert({ kVerticalRoadObj,v_newDrawObject });
	//								}
	//								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
	//							}
	//							else if (strcmp((*iter)->GetName(), "Building1") == 0)
	//							{
	//								auto newObject = new DrawObject(*(*iter));
	//								auto iter = ToSavePrefabMap.find(kBuildingObj);
	//								if (iter != ToSavePrefabMap.end())
	//								{
	//									iter->second.push_back(newObject);
	//								}
	//								else
	//								{
	//									std::vector<DrawObject*> v_newDrawObject;
	//									v_newDrawObject.push_back(newObject);
	//									ToSavePrefabMap.insert({ kBuildingObj,v_newDrawObject });
	//								}
	//								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
	//							}
	//							else if (strcmp((*iter)->GetName(), "Junction") == 0)
	//							{
	//								auto newObject = new DrawObject(*(*iter));
	//								auto iter = ToSavePrefabMap.find(kJunctionObj);
	//								if (iter != ToSavePrefabMap.end())
	//								{
	//									iter->second.push_back(newObject);
	//								}
	//								else
	//								{
	//									std::vector<DrawObject*> v_newDrawObject;
	//									v_newDrawObject.push_back(newObject);
	//									ToSavePrefabMap.insert({ kJunctionObj,v_newDrawObject });
	//								}
	//								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
	//							}
	//							else if (strcmp((*iter)->GetName(), "Park") == 0)
	//							{
	//								auto newObject = new DrawObject(*(*iter));
	//								auto iter = ToSavePrefabMap.find(kParkObj);
	//								if (iter != ToSavePrefabMap.end())
	//								{
	//									iter->second.push_back(newObject);
	//								}
	//								else
	//								{
	//									std::vector<DrawObject*> v_newDrawObject;
	//									v_newDrawObject.push_back(newObject);
	//									ToSavePrefabMap.insert({ kParkObj,v_newDrawObject });
	//								}
	//								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else if (GetAsyncKeyState(0x53) < 0)
	//		{
	//			if (!isSaved)
	//			{
	//				isSaved = true;
	//				std::ofstream outFile;

	//				OPENFILENAME ofn;

	//				char szFileName[MAX_PATH] = "";

	//				ZeroMemory(&ofn, sizeof(ofn));

	//				ofn.lStructSize = sizeof(ofn);
	//				ofn.hwndOwner = NULL;
	//				ofn.lpstrFilter = (LPCSTR)"XML(*.xml)\0All File\0.*\0";
	//				ofn.lpstrFile = (LPSTR)szFileName;
	//				ofn.nMaxFile = MAX_PATH;
	//				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST ;
	//				ofn.lpstrDefExt = (LPCSTR)"xml";
	//				
	//				GetSaveFileName(&ofn);

	//				
	//				outFile.open(ofn.lpstrFile);

	//				for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
	//				{
	//					for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
	//					{
	//						outFile << "<Entity>" << std::endl;
	//						outFile << "\t<EntityType=" << ((strcmp((*innerIter)->GetName(), "Building1") == 0) ? "StaticObject" : "Background") << ">" << std::endl;
	//						outFile << "\t<Name=" << (*innerIter)->GetName() << ">" << std::endl;
	//						outFile << "\t\t<Pos X=" << (*innerIter)->GetPosition().x << " Y=" << (*innerIter)->GetPosition().y << ">" << std::endl;
	//						outFile << "\t\t<Rotation=" << (*innerIter)->GetRotation() << ">" << std::endl;
	//						outFile << "\t\t<Size X=" << (*innerIter)->GetSizeX() << " Y=" << (*innerIter)->GetSizeY() << ">" << std::endl;
	//						outFile << "\t\t<TextureName=" << (*innerIter)->GetTexName() << ">" << std::endl;
	//						outFile << "</Entity>" << std::endl;

	//					}
	//				}
	//				outFile.close();
	//			}
	//		}
	//		else
	//		{
	//			isSaved = false;
	//			isCopying = false;
	//			isCopied = false;
	//		}
	//	}
	//	if (isDrawSelection)
	//	{
	//		selectionSquare->SetScale(-(lastClickPosition.x - DrawPosition.x), lastClickPosition.y - DrawPosition.y);
	//		selectionSquare->SetPositionX(lastClickPosition.x + selectionSquare->GetScaleX() / 2);
	//		selectionSquare->SetPositionY(lastClickPosition.y - selectionSquare->GetScaleY() / 2);
	//		selectionSquare->Draw();
	//	}

	//	EditorInfoPanelUI->SetPosition(currentCamPosX, currentCamPosY);
	//	EditorInfoPanelUI->Draw();

	//	if (selectedList.size() == 1)
	//	{
	//		s8 chars[100] = {};
	//		sprintf_s(chars, 100, "Name: %s",(*selectedList.begin())->GetName());
	//		AEGfxPrint(font, chars, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9, 1, 1, 1);
	//		s8 chars2[100] = {};
	//		sprintf_s(chars2, 100, "Pos: X: %.3f Y: %.3f ", (*selectedList.begin())->GetPosition().x, (*selectedList.begin())->GetPosition().y);
	//		AEGfxPrint(font, chars2, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9-30, 1, 1, 1);
	//		s8 chars3[100] = {};
	//		sprintf_s(chars3, 100, "Rotation: %.3f ", (*selectedList.begin())->GetRotation());
	//		AEGfxPrint(font, chars3, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9 - 60, 1, 1, 1);
	//		s8 chars4[100] = {};
	//		sprintf_s(chars4, 100, "Scale: X=%.3f Y=%.3f", (*selectedList.begin())->GetScaleX(), (*selectedList.begin())->GetScaleY());
	//		AEGfxPrint(font, chars4, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9 - 90, 1, 1, 1);
	//	}

	//	s8 chars[100] = {};
	//	s8 chars2[100]= {};
	//	s8 chars4[100]= {};
	//	s8 chars5[100]= {};
	//	s8 chars6[100] = "Snapping: ";
	//	s8 chars7[100] = {};

	//	switch (SnapState)
	//	{
	//	case kSnapXObject:
	//		strcat_s<100>(chars6, "Object X-Axis");
	//		break;
	//	case kSnapYObject:
	//		strcat_s<100>(chars6, "Object Y-Axis");
	//		break;
	//	case kSnapNone:
	//		strcat_s<100>(chars6, "NIL");
	//		break;
	//	case kSnapBothAxis:
	//		strcat_s<100>(chars6, "XY-Axis");
	//		break;
	//	case kSnapXGrid:
	//		strcat_s<100>(chars6, "X-Axis");
	//		break;
	//	case kSnapYGrid:
	//		strcat_s<100>(chars6, "Y-Axis");
	//		break;
	//	}
	//	
	//	
	//	sprintf_s(chars, 100, "Mouse Game Pos: %.2f,%.2f", selectionSquare->GetPosition().x, selectionSquare->GetPosition().y);
	//	sprintf_s(chars2, 100, "Mouse Screen Pos: %.2f,%.2f", static_cast<float>(currentMousePos.x),static_cast<float>(currentMousePos.y));
	//	sprintf_s(chars3, 100, "+");
	//	sprintf_s(chars5, 100, "SelectedObj: %s", SelectedObject != nullptr ? SelectedObject->GetName() : "---");
	//	sprintf_s(chars7, 100, "GridLock: %s", isGridLock? "On" : "Off");

	//	count = 0;
	//	for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin(); iter < PrefabVector.end(); ++iter, ++count)
	//	{
	//		if (count == ObjCounter)
	//		{
	//			sprintf_s(chars4, 100, "%s", (*iter)->GetName());
	//			break;
	//		}
	//	}

	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//	AEGfxSetBlendMode(AE_GFX_BM_NONE);

	//	AEGfxPrint(font, chars,  currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9, 1, 1, 1);
	//	AEGfxPrint(font, chars2, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 30, 1, 1, 1);
	//	AEGfxPrint(font, chars4, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 60, 1, 1, 1);
	//	AEGfxPrint(font, chars5, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 90, 1, 1, 1);
	//	AEGfxPrint(font, chars6, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 120, 1, 1, 1);
	//	AEGfxPrint(font, chars7, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 150, 0, 0, 1);

	//	/*
	//	*///DRAW ENDS////////////////////////////////////////////////////////////////////////////////////

	//	AESysFrameEnd();
	//}
	//AESysExit();

	//return 0;
}