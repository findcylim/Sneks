
#include "LevelEditorSystem.h"
#include "../ECS/EntityManager.h"
#include "../ECS/EventManager.h"
#include "../ECS/System.h"


LevelEditorSystem::LevelEditorSystem(EntityManager* entityManagerPtr, Logger* logger,GraphicsSystem* graphics) :
	BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
	
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 9, "horz-road.png", "Horizontal Road"));
	m_v_PrefabVector.push_back(CreateNewPrefab(100, 100, 9, 42, "vert-road.png", "Vertical Road"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 42, "building.png", "Building1"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 1, 1, "Selection Square", "SelectionSquare.png"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 9, 9, "junction.png", "Junction"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 42, "park.png", "Park"));



	GetWindowRect(windowHandle, &windowRect);
}
LevelEditorSystem::~LevelEditorSystem()
{

}
void LevelEditorSystem::Update(float dt)
{
	float currentCamPosX, currentCamPosY,defaultCamPosX, defaultCamPosY;
	float ScreenSizeX = (AEGfxGetWinMaxX() - AEGfxGetWinMinX())/2;
	float ScreenSizeY = (AEGfxGetWinMaxY() - AEGfxGetWinMinY())/2;

	AEGfxGetCamPosition(&currentCamPosX, &currentCamPosY);
	AEGfxGetCamPosition(&defaultCamPosX, &defaultCamPosY);

	char count;


	//s8 chars3[100] = {};
	//sprintf_s(chars3, 100, "+");
	//AEGfxPrint(font, chars3, -3, -9, 0, 0, 0);


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
		for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
		{
			(*iter)->GetComponent<DrawComponent>()->SetColor(1, 1, 1, 1);
		}
		m_x_SelectedList.clear();
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

	HTVector2 DrawPosition{ (static_cast<float>((currentCamPosX - ScreenSizeX) + currentMousePos.x) - windowRect.left)
							,(static_cast<float>((currentCamPosY + ScreenSizeY) - currentMousePos.y + 14 + windowRect.top)) };
		
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

		for (std::vector<StaticObjectEntity*>::iterator iter = m_v_PrefabVector.begin();
			 iter< m_v_PrefabVector.end();
			 ++iter,++count)
		{
			auto d_Comp = (*iter)->GetComponent<DrawComponent>();
			d_Comp->m_b_IsActive = false;
			if (count == ObjCounter)
			{
				if (ObjCounter == kBlankState)
					break;

				auto t_Comp = (*iter)->GetComponent<TransformComponent>();
				
				if (GetAsyncKeyState(VK_LCONTROL) < 0)
				{
					if (GetAsyncKeyState(VK_LSHIFT) < 0)
					{
						DrawPosition.y = (DrawPosition.y - (static_cast<int>(DrawPosition.y) % 50)) + 
										(d_Comp->GetSizeY() / 2);
						SnapState = kSnapYGrid;
					}
					else
					{
						if (DrawPosition.x < 0)
							DrawPosition.x = (DrawPosition.x - (50+ (static_cast<int>(DrawPosition.x) % 50) )) + 
											(d_Comp->GetSizeX() / 2);
						else
							DrawPosition.x = (DrawPosition.x - (static_cast<int>(DrawPosition.x) % 50)) + 
											(d_Comp->GetSizeX() / 2);

						SnapState = kSnapXGrid;
					}
				}
				else if (GetAsyncKeyState(VK_LSHIFT) < 0)
				{
					Aabb currentSelectionAabb = AabbHelper::GetAabb(t_Comp->GetPosition(),
																	d_Comp->m_x_MeshSize,
																	t_Comp->m_f_Scale);

					float shortestDist = -1;
					float xDiffClose, yDiffClose;

					StaticObjectEntity* closestObj = CheckNearestSnap(currentSelectionAabb);
						
					if (closestObj)
					{
						Aabb closestIter = AabbHelper::GetAabb(t_Comp->GetPosition(),
															   d_Comp->m_x_MeshSize,
															   t_Comp->m_f_Scale);
						if (isLeftShift == false)
						{
							//First Time Pressing Shift
							isLeftShift = true;

							char check = AabbHelper::CalculateOrientation(currentSelectionAabb, closestIter);
							if (check != -1)
								if (check != orientationCheck)
									orientationCheck = check;
						}
						SnapToOrientation(orientationCheck, isGridLock, DrawPosition, SnapState, closestObj, d_Comp->GetSizeX(), d_Comp->GetSizeY());
					}
				}
				else
				{
					isLeftShift = false;
					SnapState = kSnapNone;
				}

				t_Comp->SetPosition(DrawPosition.x,DrawPosition.y);
				d_Comp->m_b_IsActive = true;
				break;
			}
		}
	}

	/**********************************************************************************/
	// Zooming in and Out
	// Still doesnt scale properly yet TODO
	/**********************************************************************************/
	/*if (GetAsyncKeyState(AEVK_MINUS) < 0)
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
	}*/
	/**********************************************************************************/

	/**********************************************************************************/
	// Move individual objects
	/**********************************************************************************/
	if (SelectedObject)
	{
		auto d_Comp = SelectedObject->GetComponent<DrawComponent>();
		auto t_Comp = SelectedObject->GetComponent<TransformComponent>();

		d_Comp->SetColor(0.5,1.0f,0.5f,1.0f);
		if (GetAsyncKeyState(VK_LEFT) < 0)
		{
			t_Comp->m_x_Position.x -= 1;
		}
		if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			t_Comp->m_x_Position.x += 1;
		}
		if (GetAsyncKeyState(VK_UP) < 0)
		{
			t_Comp->m_x_Position.y += 1;
		}
		if (GetAsyncKeyState(VK_DOWN) < 0)
		{
			t_Comp->m_x_Position.y -= 1;
		}
	}
	/**********************************************************************************/

	/**********************************************************************************/
	//	Move groups of objects
	/**********************************************************************************/
	if (m_x_SelectedList.size() != 0)
	{
		if (GetAsyncKeyState(VK_LEFT) < 0)
		{
			for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
			{
				auto t_Comp = (*iter)->GetComponent<TransformComponent>();
				t_Comp->m_x_Position.x -= 100 * AEFrameRateControllerGetFrameTime();
			}
		}
		if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
			{
				auto t_Comp = (*iter)->GetComponent<TransformComponent>();
				t_Comp->m_x_Position.x += 100 * AEFrameRateControllerGetFrameTime();
			}
		}
		if (GetAsyncKeyState(VK_UP) < 0)
		{
			for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
			{
				auto t_Comp = (*iter)->GetComponent<TransformComponent>();
				t_Comp->m_x_Position.y += 100 * AEFrameRateControllerGetFrameTime();
			}
		}
		if (GetAsyncKeyState(VK_DOWN) < 0)
		{
			for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
			{
				auto t_Comp = (*iter)->GetComponent<TransformComponent>();
				t_Comp->m_x_Position.y -= 100 * AEFrameRateControllerGetFrameTime();
			}
		}
	}

	if (GetAsyncKeyState(1) < 0)
	{
		if (!isLeftMouseClicked)
		{
			lastClickPosition.x = DrawPosition.x;
			lastClickPosition.y = DrawPosition.y;

			isLeftMouseClicked = true;
			bool found = false;

			if (ObjCounter == kBlankState)
			{
				StaticObjectEntity* collideObject = CheckPointCollideWithAnything(DrawPosition);


				if (collideObject)
				{
					if (GetAsyncKeyState(VK_LCONTROL) < 0)
					{
						if (!Listfind(collideObject))
						{
							AddToSelected(collideObject);
						}
					}
					else
					{
						ClearSelected();
						AddToSelected(collideObject);
					}
					found = true;
				}
				if (!found)
				{
					if (SelectedObject)
					{
						SelectedObject->GetComponent<DrawComponent>()->SetColor(1.0f,1.0f,1.0f,1.0f);
					}
					SelectedObject = nullptr;

					isDrawSelection = true;
					selectionSquare->SetPosition(DrawPosition.x, DrawPosition.y);
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
		else
		{
				
		}
	}
	else
	{
		isLeftMouseClicked = false; //Potential Bug
		//do stufff
		if (isDrawSelection)
		{
			Aabb selectionBounds = {};
			selectionBounds.min = selectionSquare->GetMin();
			selectionBounds.max = selectionSquare->GetMax();

			ClearSelected(m_x_SelectedList);

			for (auto iterator = ToSavePrefabMap.begin(); iterator != ToSavePrefabMap.end(); ++iterator)
			{
				for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
				{
					Aabb checkBox = { (*innerIter)->GetMin(),(*innerIter)->GetMax() };
					if (CheckAabbIntersect(&selectionBounds, &checkBox))
					{
						(*innerIter)->SetColor(0.5, 1.0f, 0.5f, 1.0f);
						m_x_SelectedList.push_back((*innerIter));

					}
				}
			}
		}
		isDrawSelection = false;
	}

	if (GetAsyncKeyState(VK_DELETE) < 0)
	{
		if (m_x_SelectedList.size() != 0)
		{
			for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
			{
				for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end();)
				{
					bool deleted = false;
					for (auto iterer = m_x_SelectedList.begin(); iterer != m_x_SelectedList.end();)
					{
						if ((*innerIter) == (*iterer))
						{
							delete *iterer;
							deleted = true;
							iterer = m_x_SelectedList.erase(iterer);
							innerIter = (*iter).second.erase(innerIter);
							break;
						}
						else
						{
							++iterer;
						}
					}
					if(!deleted)
						++innerIter;
				}

			}
		}
	}

	for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
	{
		for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
		{
			(*innerIter)->Draw();
		}
	}

	if (GetAsyncKeyState(VK_LCONTROL) < 0)
	{
		if (GetAsyncKeyState(AEVK_C) < 0)
		{
			if (!isCopying)
			{
				isCopying = true;
				copiedList = static_cast<const std::list <DrawObject*>>(m_x_SelectedList);
				ClearSelected(m_x_SelectedList);
				copiedPosition.x = DrawPosition.x;
				copiedPosition.y = DrawPosition.y;
			}
		}
		else if (GetAsyncKeyState(AEVK_V) < 0)
		{
			if (copiedList.size() != 0)
			{
				if (!isCopied)
				{
					float diffX, diffY;
					diffX = DrawPosition.x - copiedPosition.x;
					diffY = DrawPosition.y - copiedPosition.y;
					isCopied = true;
					for (auto iter = copiedList.begin(); iter != copiedList.end(); iter++)
					{
						if (*iter)
						{
							if (strcmp((*iter)->GetName(), "Horizontal Road") == 0)
							{
								auto newObject = new DrawObject(*(*iter));
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
								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
							}
							else if (strcmp((*iter)->GetName(), "Vertical Road") == 0)
							{
								auto newObject = new DrawObject(*(*iter));
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
								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
							}
							else if (strcmp((*iter)->GetName(), "Building1") == 0)
							{
								auto newObject = new DrawObject(*(*iter));
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
								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
							}
							else if (strcmp((*iter)->GetName(), "Junction") == 0)
							{
								auto newObject = new DrawObject(*(*iter));
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
								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
							}
							else if (strcmp((*iter)->GetName(), "Park") == 0)
							{
								auto newObject = new DrawObject(*(*iter));
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
								newObject->SetPosition(newObject->GetPosition().x + diffX, newObject->GetPosition().y + diffY);
							}
						}
					}
				}
			}
		}
		else if (GetAsyncKeyState(0x53) < 0)
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
			isCopying = false;
			isCopied = false;
		}
	}
	if (isDrawSelection)
	{
		selectionSquare->SetScale(-(lastClickPosition.x - DrawPosition.x), lastClickPosition.y - DrawPosition.y);
		selectionSquare->SetPositionX(lastClickPosition.x + selectionSquare->GetScaleX() / 2);
		selectionSquare->SetPositionY(lastClickPosition.y - selectionSquare->GetScaleY() / 2);
		selectionSquare->Draw();
	}

	EditorInfoPanelUI->SetPosition(currentCamPosX, currentCamPosY);
	EditorInfoPanelUI->Draw();

	if (m_x_SelectedList.size() == 1)
	{
		s8 chars[100] = {};
		sprintf_s(chars, 100, "Name: %s",(*m_x_SelectedList.begin())->GetName());
		AEGfxPrint(font, chars, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9, 1, 1, 1);
		s8 chars2[100] = {};
		sprintf_s(chars2, 100, "Pos: X: %.3f Y: %.3f ", (*m_x_SelectedList.begin())->GetPosition().x, (*m_x_SelectedList.begin())->GetPosition().y);
		AEGfxPrint(font, chars2, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9-30, 1, 1, 1);
		s8 chars3[100] = {};
		sprintf_s(chars3, 100, "Rotation: %.3f ", (*m_x_SelectedList.begin())->GetRotation());
		AEGfxPrint(font, chars3, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9 - 60, 1, 1, 1);
		s8 chars4[100] = {};
		sprintf_s(chars4, 100, "Scale: X=%.3f Y=%.3f", (*m_x_SelectedList.begin())->GetScaleX(), (*m_x_SelectedList.begin())->GetScaleY());
		AEGfxPrint(font, chars4, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9 - 90, 1, 1, 1);
	}

	s8 chars[100] = {};
	s8 chars2[100]= {};
	s8 chars4[100]= {};
	s8 chars5[100]= {};
	s8 chars6[100] = "Snapping: ";
	s8 chars7[100] = {};

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
		
		
	sprintf_s(chars, 100, "Mouse Game Pos: %.2f,%.2f", selectionSquare->GetPosition().x, selectionSquare->GetPosition().y);
	sprintf_s(chars2, 100, "Mouse Screen Pos: %.2f,%.2f", static_cast<float>(currentMousePos.x),static_cast<float>(currentMousePos.y));
	sprintf_s(chars3, 100, "+");
	sprintf_s(chars5, 100, "SelectedObj: %s", SelectedObject != nullptr ? SelectedObject->GetName() : "---");
	sprintf_s(chars7, 100, "GridLock: %s", isGridLock? "On" : "Off");

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

	AEGfxPrint(font, chars,  currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9, 1, 1, 1);
	AEGfxPrint(font, chars2, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 30, 1, 1, 1);
	AEGfxPrint(font, chars4, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 60, 1, 1, 1);
	AEGfxPrint(font, chars5, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 90, 1, 1, 1);
	AEGfxPrint(font, chars6, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 120, 1, 1, 1);
	AEGfxPrint(font, chars7, currentCamPosX - ScreenSizeX + 10, currentCamPosY + (ScreenSizeY / 10) * 9 - 150, 0, 0, 1);

}

StaticObjectEntity* LevelEditorSystem::CreateNewPrefab(float posX, float posY, float sizeX, 
	float sizeY, const char* textureName, const char * objectName)
{
	StaticObjectEntity* obj = static_cast<StaticObjectEntity*>(
		m_po_EntityManager->NewEntity(kEntityStaticObject, objectName));

	obj->GetComponent<TransformComponent>()->SetPosition(posX, posY);
	obj->GetComponent<TransformComponent>()->SetRotation(0);
	obj->GetComponent<DrawComponent>()->Initialize(m_o_GraphicsSystem->FetchTexture(textureName),
		sizeX, sizeY, HTColor{ 1,1,1,1 });

	return obj;
}

StaticObjectEntity* LevelEditorSystem::CheckNearestSnap(Aabb& boundsCheck)
{
	StaticObjectEntity* closestObjX = nullptr;
	StaticObjectEntity* closestObjY = nullptr;
	StaticObjectEntity* closestObj = nullptr;
	float shortestDistX = -1;
	float shortestDistY = -1;
	float shortestDist = -1;
	HTVector2 boundOrigin = { boundsCheck.min.x + (boundsCheck.max.x - boundsCheck.min.x) / 2,
							  boundsCheck.min.y + (boundsCheck.max.y - boundsCheck.min.y) / 2 };
	for (auto iterator = m_x_ToSavePrefabMap.begin(); iterator != m_x_ToSavePrefabMap.end(); ++iterator)
	{
		for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
		{
			float distX = CalculateDistanceX(boundOrigin, (*innerIter)->GetComponent<TransformComponent>()->GetPosition());
			float distY = CalculateDistanceY(boundOrigin, (*innerIter)->GetComponent<TransformComponent>()->GetPosition());
			float Dist = CalculateVector2Distance(boundOrigin, (*innerIter)->GetComponent<TransformComponent>()->GetPosition());
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

	if ((CalculateDistanceX(boundOrigin, closestObjY->GetComponent<TransformComponent>()->GetPosition()) - 
		(closestObjY->GetComponent<DrawComponent>()->GetSizeY() / 2 + (boundOrigin.y - boundsCheck.min.y))) >
		(CalculateDistanceY(boundOrigin, closestObjX->GetComponent<TransformComponent>()->GetPosition()) - 
		(closestObjX->GetComponent<DrawComponent>()->GetSizeX() / 2 + (boundOrigin.x - boundsCheck.min.x))))
		return closestObjX;
	else
		return closestObjY;
}



void SnapToOrientation(char orientationCheck, bool hardSnap, HTVector2& positionToSnap, SnappingState& snapState,
	StaticObjectEntity* closestObj, float sizeX, float sizeY)
{
	float xDiffClose, yDiffClose;
	float shortestDist = -1;
	float offset = 50;

	auto t_Comp = closestObj->GetComponent<TransformComponent>();
	auto d_Comp = closestObj->GetComponent<DrawComponent>();
	switch (orientationCheck)
	{
	case 1:
		//Left
		if (hardSnap)
		{
			positionToSnap.y = t_Comp->GetPosition().y;
			positionToSnap.x = t_Comp->GetPosition().x - d_Comp->GetSizeX() / 2 - sizeX / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = CalculateDistanceX(positionToSnap, t_Comp->GetPosition());
			if (shortestDist < ((xDiffClose = sizeX / 2 + d_Comp->GetSizeX() / 2) + offset))
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
			positionToSnap.x = t_Comp->GetPosition().x;
			positionToSnap.y = t_Comp->GetPosition().y + sizeY / 2 + d_Comp->GetSizeY() / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = CalculateDistanceY(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((yDiffClose = sizeY / 2 + d_Comp->GetSizeY() / 2) + offset))
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
			positionToSnap.y = t_Comp->GetPosition().y;
			positionToSnap.x = t_Comp->GetPosition().x + d_Comp->GetSizeX() / 2 + sizeX / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = CalculateDistanceX(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((xDiffClose = sizeX / 2 + d_Comp->GetSizeX() / 2) + offset))
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
			positionToSnap.x = t_Comp->GetPosition().x;
			positionToSnap.y = t_Comp->GetPosition().y - sizeY / 2 - d_Comp->GetSizeY() / 2;
			snapState = kSnapBothAxis;
		}
		else
		{
			shortestDist = CalculateDistanceY(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
			if (shortestDist < ((yDiffClose = sizeY / 2 + d_Comp->GetSizeY() / 2) + offset))
			{
				snapState = kSnapYObject;
				positionToSnap.y += shortestDist - yDiffClose;
			}
		}
		break;
	}
}


StaticObjectEntity* LevelEditorSystem::CheckPointCollideWithAnything(HTVector2 pos)
{
	for (auto iterator = m_x_ToSavePrefabMap.begin(); iterator != m_x_ToSavePrefabMap.end(); ++iterator)
	{
		for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
		{
			Aabb currentSelectionAabb = {};
			auto t_Comp = (*innerIter)->GetComponent<TransformComponent>();
			auto d_Comp = (*innerIter)->GetComponent<DrawComponent>();

			currentSelectionAabb.min = AabbHelper::GetMin(t_Comp->GetPosition(), 
									   d_Comp->m_x_MeshSize,
									   t_Comp->m_f_Scale);
			currentSelectionAabb.max = AabbHelper::GetMax(t_Comp->GetPosition(),
									   d_Comp->m_x_MeshSize,
									   t_Comp->m_f_Scale);

			if (AabbHelper::CheckPointAabbIntersect(&currentSelectionAabb, pos))
			{
				return *innerIter;
			}
		}
	}
	return nullptr;
}

void LevelEditorSystem::ClearSelected()
{
	for (auto iter = m_x_SelectedList.begin(); iter != m_x_SelectedList.end(); iter++)
	{
		(*iter)->GetComponent<DrawComponent>()->SetColor(1, 1, 1, 1);
	}
	m_x_SelectedList.clear();
}

void LevelEditorSystem::AddToSelected(StaticObjectEntity* object)
{
	object->GetComponent<DrawComponent>()->SetColor(0.5f, 1, 0.5f, 1);
	m_x_SelectedList.push_back(object);
}

bool LevelEditorSystem::Listfind(StaticObjectEntity* object)
{
	for (auto Object : m_x_SelectedList)
	{
		if (Object == object)
		{
			return true;
		}
	}
	return false;
}


