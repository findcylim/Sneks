#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <map>
#include <string>
#include <fstream>
#include "Graphics/DrawObject.h"
#include "Math/HTVector2.h"
#include "AEEngine.h"
#include "Math/Aabb.h"
#include <sys/stat.h>
#include <Windows.h>
#include <vector>
#include <commdlg.h>

constexpr int kNumBodyParts = 20;

float DrawObject::m_f_GlobalScale = 1.0f;
float DrawObject::m_f_GlobalCameraOffsetX = 0.0f;
float DrawObject::m_f_GlobalCameraOffsetY = 0.0f;
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	//MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 300, false, nullptr);
	AESysSetWindowTitle("Level Editor");
	AEToogleFullScreen(false);
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 1);

	
	auto snakeHeadTexture           = AEGfxTextureLoad("../../Resources/snake-head.png");
	auto snakeHeadLTexture          = AEGfxTextureLoad("../../Resources/snek_hed_l.jpg");
	auto snakeHeadRTexture          = AEGfxTextureLoad("../../Resources/snek_hed_r.jpg");
	auto snake2HeadTexture          = AEGfxTextureLoad("../../Resources/head2.png");
	auto snakeBodyTexture           = AEGfxTextureLoad("../../Resources/snake-body.png");
	auto snake2BodyTexture          = AEGfxTextureLoad("../../Resources/snake-body2.png");
	auto rocketTexture              = AEGfxTextureLoad("../../Resources/rocket_booster.jpg");
	auto smokeTexture               = AEGfxTextureLoad("../../Resources/smoke.jpg");
	auto cityTexture		        = AEGfxTextureLoad("../../Resources/map.png");
	auto verticalRoadTexture		= AEGfxTextureLoad("../../Resources/vert-road.png");
	auto horizontalRoadTexture		= AEGfxTextureLoad("../../Resources/horz-road.png");
	auto buildingTexture    		= AEGfxTextureLoad("../../Resources/building.png");
	auto editorScale				= AEGfxTextureLoad("../../Resources/EditorScale.png");
	auto junctionTexture			= AEGfxTextureLoad("../../Resources/junction.png");
	auto parkTexture			    = AEGfxTextureLoad("../../Resources/park.png");

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<DrawObject*> background = {};
	for (int i_Background = -5; i_Background <= 5; i_Background++) {
		for (int i_BackgroundY = -5; i_BackgroundY <= 5; i_BackgroundY++) {
			auto bg = new DrawObject(i_Background * 1000, i_BackgroundY * 1000, 1000, 1000, editorScale,"EditorScale","EditorScale.png");
			background.push_back(bg);
		}
	}

	HWND windowHandle = AESysGetWindowHandle();
	RECT windowRect; 
	auto horRoad = new DrawObject(0, 0, 71, 9, horizontalRoadTexture,"Horizontal Road","horz-road.png");
	auto verRoad = new DrawObject(100, 100, 9, 42, verticalRoadTexture, "Vertical Road","vert-road.png");
	auto buildingObj = new DrawObject(0, 0, 71, 42, buildingTexture,"Building1","building.png");
	auto junctionObj = new DrawObject(0, 0, 9, 9, buildingTexture, "Junction", "junction.png");
	auto parkObj = new DrawObject(0, 0, 71, 42, parkTexture, "Park", "park.png");
	bool isTabPressed = false;
	char ObjCounter = kBuildingObj;
	SnappingState SnapState = kSnapNone;

	std::vector<DrawObject*> PrefabVector;
	std::map<Objects, std::vector<DrawObject*>> ToSavePrefabMap;

	PrefabVector.push_back(buildingObj);
	PrefabVector.push_back(verRoad);
	PrefabVector.push_back(horRoad);
	PrefabVector.push_back(junctionObj);
	PrefabVector.push_back(parkObj);


	float zoomLimitMin = 0.8f, zoomLimitMax = 1.2f;

	auto font = AEGfxCreateFont("Arial", 20, false, false);
	auto winFont = AEGfxCreateFont("Arial", 500, 1, 0);

	float currentCamPosX, currentCamPosY,defaultCamPosX, defaultCamPosY;
	bool isMiddleMouseClicked	= false;
	bool isLeftMouseClicked		= false;
	bool isSaved				= false;
	bool isLeftShift = false;
	bool isCapsLock = false;
	bool isGridLock = false;
	bool isLeft = false, isRight = false, isUp = false, isDown = false;
	POINT initialMousePos,currentMousePos;
	float ScreenSizeX = (AEGfxGetWinMaxX() - AEGfxGetWinMinX())/2;
	float ScreenSizeY = (AEGfxGetWinMaxY() - AEGfxGetWinMinY())/2;
	AEGfxGetCamPosition(&currentCamPosX, &currentCamPosY);
	AEGfxGetCamPosition(&defaultCamPosX, &defaultCamPosY);
	int winner = 0;
	DrawObject* SelectedObject = nullptr;

	char orientationCheck = -1;


	while (!winner) {
		AESysFrameStart();
		AEInputUpdate();

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);
		char count;
		for (auto& i_Backgrounds : background) {
			i_Backgrounds->Draw();
		}
		s8 chars3[100] = {};
		sprintf_s(chars3, 100, "+");
		AEGfxPrint(font, chars3, -3, -9, 0, 0, 0);

		GetWindowRect(windowHandle, &windowRect);

		GetCursorPos(&currentMousePos);

		if (GetAsyncKeyState(4) < 0)
		{
			if (isMiddleMouseClicked == false)
			{
				isMiddleMouseClicked = true;
				GetCursorPos(&initialMousePos);
			}
			float diffX = initialMousePos.x - currentMousePos.x;
			float diffY = initialMousePos.y - currentMousePos.y;

			currentCamPosX = defaultCamPosX + diffX;
			currentCamPosY = defaultCamPosY - diffY;
			AEGfxSetCamPosition(currentCamPosX, currentCamPosY);

		}
		else
		{
			if (isMiddleMouseClicked)
			{
				isMiddleMouseClicked = false;
				defaultCamPosX = currentCamPosX;
				defaultCamPosY = currentCamPosY;
			}
		}
		if (GetAsyncKeyState(VK_ESCAPE) < 0)
		{
			ObjCounter = kBlankState;
		}
		if (GetAsyncKeyState(AEVK_CAPSLOCK) < 0)
		{
			if (!isCapsLock)
			{
				isGridLock = !isGridLock;
			}
			isCapsLock = true;
			
		}
		else
		{
			isCapsLock = false;
		}






		HTVector2 DrawPosition{ (static_cast<float>((currentCamPosX - ScreenSizeX) + currentMousePos.x  ) - windowRect.left )* DrawObject::m_f_GlobalScale
										 ,(static_cast<float>((currentCamPosY + ScreenSizeY) - currentMousePos.y + windowRect.top )* DrawObject::m_f_GlobalScale )};
		
		if (GetAsyncKeyState(VK_TAB) < 0)
		{
			if (!isTabPressed)
			{
				isTabPressed = true;
				ObjCounter++;
				if (ObjCounter >= kNumberOfObjects)
				{
					ObjCounter = 0;
				}

			}

		}
		else 
		{
			isTabPressed = false;
			char count = 0;

			for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin();iter<PrefabVector.end();++iter,++count)
			{
				if (count == ObjCounter)
				{
					if (ObjCounter == kBlankState)
						break;
					if (GetAsyncKeyState(VK_LCONTROL) < 0)
					{
						if (GetAsyncKeyState(VK_LSHIFT) < 0)
						{
							DrawPosition.y = (DrawPosition.y - (static_cast<int>(DrawPosition.y) % 50)) + ((*iter)->GetSizeY() / 2);
							SnapState = kSnapYGrid;
						}
						else
						{
							if (DrawPosition.x < 0)
							{
								DrawPosition.x = (DrawPosition.x - (50+ (static_cast<int>(DrawPosition.x) % 50) )) + ((*iter)->GetSizeX() / 2);
							}
							else
							{
								DrawPosition.x = (DrawPosition.x - (static_cast<int>(DrawPosition.x) % 50)) + ((*iter)->GetSizeX() / 2);
							}
							SnapState = kSnapXGrid;
						}
					}
					else if (GetAsyncKeyState(VK_LSHIFT) < 0)
					{
						Aabb currentSelectionAabb ={};
						currentSelectionAabb.min = (*iter)->GetMin();
						currentSelectionAabb.max = (*iter)->GetMax();
						float shortestDist = -1;
						float xDiffClose, yDiffClose;
						DrawObject* closestObj = nullptr;
						for (auto iterator = ToSavePrefabMap.begin(); iterator != ToSavePrefabMap.end(); ++iterator)
						{
							for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
							{
								float dist = Distance(DrawPosition, (*innerIter)->GetPosition());
								if (shortestDist < 0 ||dist >= 0 && dist < shortestDist)
								{
									shortestDist = dist;
									closestObj = *innerIter;
								}
								else
								{
									continue;
								}

							}
						}
						if (closestObj)
						{
							Aabb currentIter = {};
							currentIter.min = closestObj->GetMin();
							currentIter.max = closestObj->GetMax();

							short offset = 50;
							if (isLeftShift == false)
							{
								isLeftShift = true;
								char check = CalculateOrientation(currentSelectionAabb, currentIter);
								if (check != -1)
									if (check != orientationCheck)
										orientationCheck = check;

							}
							switch (orientationCheck)
							{
							case 1:
								//Left

								if (isGridLock)
								{
									DrawPosition.y = closestObj->GetPosition().y;
									DrawPosition.x = closestObj->GetPosition().x - closestObj->GetSizeX() / 2 - (*iter)->GetSizeX() / 2;
									SnapState = kSnapBothAxis;
								}
								else
								{
									shortestDist = DistanceX(DrawPosition, closestObj->GetPosition());
									if (shortestDist < ((xDiffClose = (*iter)->GetSizeX() / 2 + closestObj->GetSizeX() / 2) + offset))
									{
										DrawPosition.x += shortestDist - xDiffClose;
										SnapState = kSnapXObject;
									}
								}
								break;
							case 2:
								//Top
								if (isGridLock)
								{
									DrawPosition.x = closestObj->GetPosition().x;
									DrawPosition.y = closestObj->GetPosition().y + (*iter)->GetSizeY() / 2 + closestObj->GetSizeY() / 2;
									SnapState = kSnapBothAxis;
								}
								else
								{
									shortestDist = DistanceY(DrawPosition, closestObj->GetPosition()); // ouch there must be a better way
									if (shortestDist < ((yDiffClose = (*iter)->GetSizeY() / 2 + closestObj->GetSizeY() / 2) + offset))
									{
										SnapState = kSnapYObject;
										DrawPosition.y -= shortestDist - yDiffClose;
									}
								}
								break;
							case 3:
								//Right

								if (isGridLock)
								{
									DrawPosition.y = closestObj->GetPosition().y;
									DrawPosition.x = closestObj->GetPosition().x + closestObj->GetSizeX() / 2 + (*iter)->GetSizeX() / 2;
									SnapState = kSnapBothAxis;
								}
								else
								{
									shortestDist = DistanceX(DrawPosition, closestObj->GetPosition()); // ouch there must be a better way
									if (shortestDist < ((xDiffClose = (*iter)->GetSizeX() / 2 + closestObj->GetSizeX() / 2) + offset))
									{
										SnapState = kSnapXObject;
										DrawPosition.x -= shortestDist - xDiffClose;
									}
								}
								break;
							case 4:
								//Bottom

								if (isGridLock)
								{
									DrawPosition.x = closestObj->GetPosition().x;
									DrawPosition.y = closestObj->GetPosition().y - (*iter)->GetSizeY() / 2 - closestObj->GetSizeY() / 2;
									SnapState = kSnapBothAxis;
								}
								else
								{
									shortestDist = DistanceY(DrawPosition, closestObj->GetPosition()); // ouch there must be a better way
									if (shortestDist < ((yDiffClose = (*iter)->GetSizeY() / 2 + closestObj->GetSizeY() / 2) + offset))
									{
										SnapState = kSnapYObject;
										DrawPosition.y += shortestDist - yDiffClose;
									}
								}
								break;
							}
						}
					}
					else
					{
						isLeftShift = false;
						SnapState = kSnapNone;
					}

					(*iter)->SetPosition(DrawPosition.x,DrawPosition.y);
					(*iter)->Draw();
					break;
				}
			}
		}

		if (GetAsyncKeyState(AEVK_MINUS) < 0)
		{
			if (DrawObject::m_f_GlobalScale >= zoomLimitMin)
			{
				DrawObject::m_f_GlobalScale -= 1 * AEFrameRateControllerGetFrameTime();
			}
			else
			{
				DrawObject::m_f_GlobalScale = zoomLimitMin;
			}
		}
		else if (GetAsyncKeyState(AEVK_EQUAL) < 0)
		{
			if (DrawObject::m_f_GlobalScale <= zoomLimitMax)
			{
				DrawObject::m_f_GlobalScale += 1 * AEFrameRateControllerGetFrameTime();
			}
			else
			{
				DrawObject::m_f_GlobalScale = zoomLimitMax;
			}
		}

		if (SelectedObject)
		{
			SelectedObject->SetColor(0.5,1.0f,0.5f,1.0f);
			if (GetAsyncKeyState(VK_LEFT) < 0)
			{
				SelectedObject->SetPositionX(SelectedObject->GetPosition().x - 1);
			}
			if (GetAsyncKeyState(VK_RIGHT) < 0)
			{
				SelectedObject->SetPositionX(SelectedObject->GetPosition().x + 1);
			}
			if (GetAsyncKeyState(VK_UP) < 0)
			{
				SelectedObject->SetPositionY(SelectedObject->GetPosition().y + 1);
			}
			if (GetAsyncKeyState(VK_DOWN) < 0)
			{
				SelectedObject->SetPositionY(SelectedObject->GetPosition().y - 1);
			}
		}

		if (GetAsyncKeyState(1) < 0)
		{

			if (!isLeftMouseClicked)
			{
				isLeftMouseClicked = true;
				bool found = false;
				if (ObjCounter == kBlankState)
				{

					Aabb mouse = {};
					mouse.min = DrawPosition;
					mouse.max = DrawPosition;
					for (auto iterator = ToSavePrefabMap.begin(); iterator != ToSavePrefabMap.end(); ++iterator)
					{
						for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
						{
							Aabb currentSelectionAabb = {};
							currentSelectionAabb.min = (*innerIter)->GetMin();
							currentSelectionAabb.max = (*innerIter)->GetMax();
							if (CheckAabbIntersect(&currentSelectionAabb,&mouse))
							{
								SelectedObject = (*innerIter);
								found = true;
							}
						}
					}
					if (!found)
					{
						if (SelectedObject)
						{
							SelectedObject->SetColor(1.0f,1.0f,1.0f,1.0f);
						}
						SelectedObject = nullptr;
					}
				}
				else
				{
					switch (ObjCounter)
					{
						case kBuildingObj:
						{
							auto newObject = new DrawObject(*buildingObj);
							auto iter = ToSavePrefabMap.find(kBuildingObj);
							if (iter != ToSavePrefabMap.end())
							{
								iter->second.push_back(newObject);
							}
							else
							{
								std::vector<DrawObject*> v_newDrawObject;
								v_newDrawObject.push_back(newObject);
								ToSavePrefabMap.insert({ kBuildingObj,v_newDrawObject });
							}
							break;
						}
						case kVerticalRoadObj:
						{
							auto newObject = new DrawObject(*verRoad);
							auto iter = ToSavePrefabMap.find(kVerticalRoadObj);
							if (iter != ToSavePrefabMap.end())
							{
								iter->second.push_back(newObject);
							}
							else
							{
								std::vector<DrawObject*> v_newDrawObject;
								v_newDrawObject.push_back(newObject);
								ToSavePrefabMap.insert({ kVerticalRoadObj,v_newDrawObject });
							}
							break;
						}
						case kHorizontalRoadObj:
						{
							auto newObject = new DrawObject(*horRoad);
							auto iter = ToSavePrefabMap.find(kHorizontalRoadObj);
							if (iter != ToSavePrefabMap.end())
							{
								iter->second.push_back(newObject);
							}
							else
							{
								std::vector<DrawObject*> v_newDrawObject;
								v_newDrawObject.push_back(newObject);
								ToSavePrefabMap.insert({ kHorizontalRoadObj,v_newDrawObject });
							}
							break;
						}
						case kJunctionObj:
						{
							auto newObject = new DrawObject(*junctionObj);
							auto iter = ToSavePrefabMap.find(kJunctionObj);
							if (iter != ToSavePrefabMap.end())
							{
								iter->second.push_back(newObject);
							}
							else
							{
								std::vector<DrawObject*> v_newDrawObject;
								v_newDrawObject.push_back(newObject);
								ToSavePrefabMap.insert({ kJunctionObj,v_newDrawObject });
							}
							break;
						}
						case kParkObj:
						{
							auto newObject = new DrawObject(*parkObj);
							auto iter = ToSavePrefabMap.find(kParkObj);
							if (iter != ToSavePrefabMap.end())
							{
								iter->second.push_back(newObject);
							}
							else
							{
								std::vector<DrawObject*> v_newDrawObject;
								v_newDrawObject.push_back(newObject);
								ToSavePrefabMap.insert({ kParkObj,v_newDrawObject });
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			isLeftMouseClicked = false; //Potential Bug
		}
		
		for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
		{
			for (auto innerIter = (*iter).second.begin();innerIter != (*iter).second.end() ;)
			{
				(*innerIter)->Draw();

				if (SelectedObject == (*innerIter))
				{
					if (GetAsyncKeyState(VK_DELETE) < 0)
					{
						delete(SelectedObject);
						innerIter = (*iter).second.erase(innerIter);
						SelectedObject = nullptr;
					}
					else
						++innerIter;
				}
				else
				{
					++innerIter;
				}
			}
		}

		if (GetAsyncKeyState(VK_LCONTROL) < 0)
		{
			if (GetAsyncKeyState(0x53) < 0)
			{
				if (!isSaved)
				{
					isSaved = true;
					std::ofstream outFile;

					OPENFILENAME ofn;

					char szFileName[MAX_PATH] = "";

					ZeroMemory(&ofn, sizeof(ofn));

					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;
					ofn.lpstrFilter = (LPCSTR)"XML(*.xml)\0All File\0.*\0";
					ofn.lpstrFile = (LPSTR)szFileName;
					ofn.nMaxFile = MAX_PATH;
					ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST ;
					ofn.lpstrDefExt = (LPCSTR)"xml";
					
					GetSaveFileName(&ofn);

					
					outFile.open(ofn.lpstrFile);

					for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
					{
						for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
						{
							outFile << "<Entity>" << std::endl;
							outFile << "\t<EntityType=" << ((strcmp((*innerIter)->GetName(), "Building1") == 0) ? "StaticObject" : "Background") << ">" << std::endl;
							outFile << "\t<Name=" << (*innerIter)->GetName() << ">" << std::endl;
							outFile << "\t\t<Pos X=" << (*innerIter)->GetPosition().x << " Y=" << (*innerIter)->GetPosition().y << ">" << std::endl;
							outFile << "\t\t<Rotation=" << (*innerIter)->GetRotation() << ">" << std::endl;
							outFile << "\t\t<Size X=" << (*innerIter)->GetSizeX() << " Y=" << (*innerIter)->GetSizeY() << ">" << std::endl;
							outFile << "\t\t<TextureName=" << (*innerIter)->GetTexName() << ">" << std::endl;
							outFile << "</Entity>" << std::endl;

						}
					}
					outFile.close();
				}
			}
			else
			{
				isSaved = false;
			}
		}
		

		s8 chars[100] = {};
		s8 chars2[100]= {};
		s8 chars4[100]= {};
		s8 chars5[100]= {};
		s8 chars6[100] = "Snapping: ";
		switch (SnapState)
		{
		case kSnapXObject:
			strcat_s<100>(chars6, "Object X-Axis");
			break;
		case kSnapYObject:
			strcat_s<100>(chars6, "Object Y-Axis");
			break;
		case kSnapNone:
			strcat_s<100>(chars6, "NIL");
			break;
		case kSnapBothAxis:
			strcat_s<100>(chars6, "XY-Axis");
			break;
		case kSnapXGrid:
			strcat_s<100>(chars6, "X-Axis");
			break;
		case kSnapYGrid:
			strcat_s<100>(chars6, "Y-Axis");
			break;
		}
		
		

		sprintf_s(chars, 100, "DrawObject Pos: %.2f,%.2f", DrawPosition.x, DrawPosition.y);
		sprintf_s(chars2, 100, "MousePos: %.2f,%.2f", static_cast<float>(currentMousePos.x),static_cast<float>(currentMousePos.y));
		sprintf_s(chars3, 100, "+");
		sprintf_s(chars5, 100, "SelectedObj: %s", SelectedObject != nullptr ? SelectedObject->GetName() : "---");
		
		count = 0;
		for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin(); iter < PrefabVector.end(); ++iter, ++count)
		{
			if (count == ObjCounter)
			{
				sprintf_s(chars4, 100, "%s", (*iter)->GetName());
				break;
			}
		}

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);

		AEGfxPrint(font, chars, currentCamPosX- ScreenSizeX, currentCamPosY+(ScreenSizeY/10)*9, 0, 0, 1);
		AEGfxPrint(font, chars2, currentCamPosX - ScreenSizeX, currentCamPosY + (ScreenSizeY/10)*9  -30, 1, 0, 0);
		AEGfxPrint(font, chars4, currentCamPosX - ScreenSizeX, currentCamPosY + (ScreenSizeY / 10) * 9-60, 0, 0, 1);
		AEGfxPrint(font, chars5, currentCamPosX - ScreenSizeX, currentCamPosY + (ScreenSizeY / 10) * 9 - 90, 0, 0, 1);
		AEGfxPrint(font, chars6, currentCamPosX - ScreenSizeX, currentCamPosY + (ScreenSizeY / 10) * 9 - 120, 0, 0, 1);
		/*
		*///DRAW ENDS////////////////////////////////////////////////////////////////////////////////////

		AESysFrameEnd();
	}
	AESysExit();

	return 0;
}