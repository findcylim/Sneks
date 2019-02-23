
#include "LevelEditorSystem.h"

#include "../../Dependencies/imgui-1.68/imgui_impl_win32.h"
LevelEditorSystem::LevelEditorSystem(EntityManager* entityManagerPtr, Logger* logger,GraphicsSystem* graphics) :
	BaseSystem(entityManagerPtr)
{
	m_o_GraphicsSystem = graphics;
	m_o_Logger = logger;
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 9, "horz-road.png", "Horizontal Road"));
	m_v_PrefabVector.push_back(CreateNewPrefab(100, 100, 9, 42, "vert-road.png", "Vertical Road"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 42, "building.png", "Building1"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 9, 9, "junction.png", "Junction"));
	m_v_PrefabVector.push_back(CreateNewPrefab(0, 0, 71, 42, "park.png", "Park"));
	for (auto prefab : m_v_PrefabVector)
	{
		prefab->GetComponent<DrawComponent>()->m_b_IsActive = false;
	}

	selectionSquare = CreateNewPrefab(0, 0, 1, 1, "SelectionSquare", "SelectionSquare.png");
	selectionSquare->GetComponent<DrawComponent>()->m_b_IsActive = false;
	GetWindowRect(windowHandle, &windowRect);

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImGui_ImplWin32_Init(windowHandle);
	//const char* glsl_version = "#version 130";
	//ImGui_ImplOpenGL3_Init(glsl_version);

	//ImGui::StyleColorsDark();
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


	s8 chars3[100] = {};
	sprintf_s(chars3, 100, "+");
	AEGfxPrint(font, chars3, -3, -9, 0, 0, 0);


	GetCursorPos(&currentMousePos);

	if (GetAsyncKeyState(4) < 0)
	{
		if (isMiddleMouseClicked == false)
		{
			isMiddleMouseClicked = true;
			GetCursorPos(&initialMousePos);
		}

		float diffX = static_cast<float>(initialMousePos.x - currentMousePos.x);
		float diffY = static_cast<float>(initialMousePos.y - currentMousePos.y);

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
			ChosenObject->GetComponent<DrawComponent>()->m_b_IsActive = false;
			if (ObjCounter >= kNumberOfObjects)
			{
				ObjCounter = 0;
			}
		}
	}
	else 
	{
		isTabPressed = false;
		count = 0;

		for (std::vector<StaticObjectEntity*>::iterator iter = m_v_PrefabVector.begin();
			 iter< m_v_PrefabVector.end();
			++iter, ++count
			 )
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
						if (isLeftShift == false)
						{
							//First Time Pressing Shift
							isLeftShift = true;
							auto pos = t_Comp->GetPosition();
							char check = AabbHelper::CalculateOrientation(pos,
								closestObj->GetComponent<TransformComponent>()->GetPosition());
							if (check != -1)
								if (check != orientationCheck)
									orientationCheck = check;


						}
						SnapToOrientation(isGridLock, DrawPosition, SnapState, closestObj, d_Comp->GetSizeX(), d_Comp->GetSizeY());
					}
					else
					{
						DrawPosition = lastMousePosition;
					}
				}
				else
				{
					isLeftShift = false;
					SnapState = kSnapNone;
				}

				t_Comp->SetPosition(DrawPosition.x,DrawPosition.y);
				d_Comp->m_b_IsActive = true;
				ChosenObject = *iter;
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
					auto d_Comp = selectionSquare->GetComponent<DrawComponent>();
					if (SelectedObject)
					{
						d_Comp->SetColor(1.0f,1.0f,1.0f,1.0f);
					}
					SelectedObject = nullptr;

					isDrawSelection = true;
					d_Comp->m_b_IsActive = true;
					selectionSquare->GetComponent<TransformComponent>()->SetPosition(DrawPosition.x, DrawPosition.y);
				}
			}
			else
			{
				if(ChosenObject)
				{ 
					auto ct_Comp = ChosenObject->GetComponent<TransformComponent>();
					auto cd_Comp = ChosenObject->GetComponent<DrawComponent>();
					switch (ObjCounter)
					{
					case kBuildingObj:
					{
						auto newObject = CopyPrefab(*ChosenObject);

						auto iter = m_x_ToSavePrefabMap.find(kBuildingObj);
						if (iter != m_x_ToSavePrefabMap.end())
						{
							iter->second.push_back(newObject);
						}
						else
						{
							std::vector<StaticObjectEntity*> v_newDrawObject;
							v_newDrawObject.push_back(newObject);
							m_x_ToSavePrefabMap.insert({ kBuildingObj,v_newDrawObject });
						}
						break;
					}
					case kVerticalRoadObj:
					{
						auto newObject = CopyPrefab(*ChosenObject);

						auto iter = m_x_ToSavePrefabMap.find(kVerticalRoadObj);
						if (iter != m_x_ToSavePrefabMap.end())
						{
							iter->second.push_back(newObject);
						}
						else
						{
							std::vector<StaticObjectEntity*> v_newDrawObject;
							v_newDrawObject.push_back(newObject);
							m_x_ToSavePrefabMap.insert({ kVerticalRoadObj,v_newDrawObject });
						}
						break;
					}
					case kHorizontalRoadObj:
					{
						auto newObject = CopyPrefab(*ChosenObject);
						auto iter = m_x_ToSavePrefabMap.find(kHorizontalRoadObj);
						if (iter != m_x_ToSavePrefabMap.end())
						{
							iter->second.push_back(newObject);
						}
						else
						{
							std::vector<StaticObjectEntity*> v_newDrawObject;
							v_newDrawObject.push_back(newObject);
							m_x_ToSavePrefabMap.insert({ kHorizontalRoadObj,v_newDrawObject });
						}
						break;
					}
					case kJunctionObj:
					{
						auto newObject = CopyPrefab(*ChosenObject);
						auto iter = m_x_ToSavePrefabMap.find(kJunctionObj);
						if (iter != m_x_ToSavePrefabMap.end())
						{
							iter->second.push_back(newObject);
						}
						else
						{
							std::vector<StaticObjectEntity*> v_newDrawObject;
							v_newDrawObject.push_back(newObject);
							m_x_ToSavePrefabMap.insert({ kJunctionObj,v_newDrawObject });
						}
						break;
					}
					case kParkObj:
					{
						auto newObject = CopyPrefab(*ChosenObject);
						auto iter = m_x_ToSavePrefabMap.find(kParkObj);
						if (iter != m_x_ToSavePrefabMap.end())
						{
							iter->second.push_back(newObject);
						}
						else
						{
							std::vector<StaticObjectEntity*> v_newDrawObject;
							v_newDrawObject.push_back(newObject);
							m_x_ToSavePrefabMap.insert({ kParkObj,v_newDrawObject });
						}
						break;
					}
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
			auto t_Comp = selectionSquare->GetComponent<TransformComponent>();
			auto d_Comp = selectionSquare->GetComponent<DrawComponent>();
			Aabb selectionBounds = AabbHelper::GetAabb(t_Comp->m_x_Position,d_Comp->m_x_MeshSize,t_Comp->m_f_Scale);

			ClearSelected();
			d_Comp->m_b_IsActive = true;

			for (auto iterator = m_x_ToSavePrefabMap.begin(); iterator != m_x_ToSavePrefabMap.end(); ++iterator)
			{
				for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
				{
					auto dt_Comp = (*innerIter)->GetComponent<TransformComponent>();
					auto dd_Comp = (*innerIter)->GetComponent<DrawComponent>();
					Aabb checkBox = AabbHelper::GetAabb(dt_Comp->m_x_Position, dd_Comp->m_x_MeshSize, dt_Comp->m_f_Scale);
					if (AabbHelper::CheckAabbIntersect(&selectionBounds, &checkBox))
					{
						dd_Comp->SetColor(0.5, 1.0f, 0.5f, 1.0f);
						m_x_SelectedList.push_back((*innerIter));

					}
				}
			}
		}
		else
		{

			selectionSquare->GetComponent<DrawComponent>()->m_b_IsActive = false;
		}
		isDrawSelection = false;
	}

	if (GetAsyncKeyState(VK_DELETE) < 0)
	{
		if (m_x_SelectedList.size() != 0)
		{
			for (auto iter = m_x_ToSavePrefabMap.begin(); iter != m_x_ToSavePrefabMap.end(); ++iter)
			{
				for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end();)
				{
					bool deleted = false;
					for (auto iterer = m_x_SelectedList.begin(); iterer != m_x_SelectedList.end();)
					{
						if ((*innerIter) == (*iterer))
						{
							m_po_EntityManager->AddToDeleteQueue(static_cast<BaseEntity*>(*iterer));
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

	if (GetAsyncKeyState(VK_LCONTROL) < 0)
	{
		if (GetAsyncKeyState(AEVK_C) < 0)
		{
			if (!isCopying)
			{
				isCopying = true;
				m_x_CopiedList = static_cast<const std::list <StaticObjectEntity*>>(m_x_SelectedList);
				ClearSelected();
				copiedPosition.x = DrawPosition.x;
				copiedPosition.y = DrawPosition.y;
			}
		}
		else if (GetAsyncKeyState(AEVK_V) < 0)
		{
			if (m_x_CopiedList.size() != 0)
			{
				if (!isCopied)
				{
					float diffX, diffY;
					diffX = DrawPosition.x - copiedPosition.x;
					diffY = DrawPosition.y - copiedPosition.y;
					isCopied = true;
					for (auto iter = m_x_CopiedList.begin(); iter != m_x_CopiedList.end(); iter++)
					{
						if (*iter)
						{
							if (strcmp((*iter)->m_pc_EntityName, "Horizontal Road") == 0)
							{
								auto newObject = CopyPrefab(*(*iter));
								auto iter = m_x_ToSavePrefabMap.find(kHorizontalRoadObj);
								if (iter != m_x_ToSavePrefabMap.end())
								{
									iter->second.push_back(newObject);
								}
								else
								{
									std::vector<StaticObjectEntity*> v_newDrawObject;
									v_newDrawObject.push_back(newObject);
									m_x_ToSavePrefabMap.insert({ kHorizontalRoadObj,v_newDrawObject });
								}
								auto& temp = newObject->GetComponent<TransformComponent>()->m_x_Position;
								temp = { temp.x + diffX, temp.y + diffY };
							}
							else if (strcmp((*iter)->m_pc_EntityName, "Vertical Road") == 0)
							{
								auto newObject = CopyPrefab(*(*iter));
								auto iter = m_x_ToSavePrefabMap.find(kVerticalRoadObj);
								if (iter != m_x_ToSavePrefabMap.end())
								{
									iter->second.push_back(newObject);
								}
								else
								{
									std::vector<StaticObjectEntity*> v_newDrawObject;
									v_newDrawObject.push_back(newObject);
									m_x_ToSavePrefabMap.insert({ kVerticalRoadObj,v_newDrawObject });
								}
								auto& temp = newObject->GetComponent<TransformComponent>()->m_x_Position;
								temp = { temp.x + diffX, temp.y + diffY };
							}
							else if (strcmp((*iter)->m_pc_EntityName, "Building1") == 0)
							{
								auto newObject = CopyPrefab(*(*iter));
								auto iter = m_x_ToSavePrefabMap.find(kBuildingObj);
								if (iter != m_x_ToSavePrefabMap.end())
								{
									iter->second.push_back(newObject);
								}
								else
								{
									std::vector<StaticObjectEntity*> v_newDrawObject;
									v_newDrawObject.push_back(newObject);
									m_x_ToSavePrefabMap.insert({ kBuildingObj,v_newDrawObject });
								}
								auto& temp = newObject->GetComponent<TransformComponent>()->m_x_Position;
								temp = { temp.x + diffX, temp.y + diffY };
							}
							else if (strcmp((*iter)->m_pc_EntityName, "Junction") == 0)
							{
								auto newObject = CopyPrefab(*(*iter));
								auto iter = m_x_ToSavePrefabMap.find(kJunctionObj);
								if (iter != m_x_ToSavePrefabMap.end())
								{
									iter->second.push_back(newObject);
								}
								else
								{
									std::vector<StaticObjectEntity*> v_newDrawObject;
									v_newDrawObject.push_back(newObject);
									m_x_ToSavePrefabMap.insert({ kJunctionObj,v_newDrawObject });
								}
								auto& temp = newObject->GetComponent<TransformComponent>()->m_x_Position;
								temp = { temp.x + diffX, temp.y + diffY };
							}
							else if (strcmp((*iter)->m_pc_EntityName, "Park") == 0)
							{
								auto newObject = CopyPrefab(*(*iter));
								auto iter = m_x_ToSavePrefabMap.find(kParkObj);
								if (iter != m_x_ToSavePrefabMap.end())
								{
									iter->second.push_back(newObject);
								}
								else
								{
									std::vector<StaticObjectEntity*> v_newDrawObject;
									v_newDrawObject.push_back(newObject);
									m_x_ToSavePrefabMap.insert({ kParkObj,v_newDrawObject });
								}
								auto& temp = newObject->GetComponent<TransformComponent>()->m_x_Position;
								temp = { temp.x + diffX, temp.y + diffY };
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

				for (auto iter = m_x_ToSavePrefabMap.begin(); iter != m_x_ToSavePrefabMap.end(); ++iter)
				{
					for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
					{
						outFile << "<Entity>" << std::endl;
						outFile << "\t<EntityType=" << ((strcmp((*innerIter)->m_pc_EntityName, "Building1") == 0) ? "StaticObject" : "Background") << ">" << std::endl;
						outFile << "\t<Name=" << (*innerIter)->m_pc_EntityName << ">" << std::endl;
						auto t_Comp = (*innerIter)->GetComponent<TransformComponent>();
						auto d_Comp = (*innerIter)->GetComponent<DrawComponent>();

						outFile << "\t\t<Pos X=" << t_Comp->m_x_Position.x << " Y=" << t_Comp->m_x_Position.y << ">" << std::endl;
						outFile << "\t\t<Rotation=" << t_Comp->GetRotation() << ">" << std::endl;
						outFile << "\t\t<Size X=" << d_Comp->GetSizeX() << " Y=" << d_Comp->GetSizeY() << ">" << std::endl;
						outFile << "\t\t<TextureName=" << d_Comp->m_px_Texture->mpName << ">" << std::endl;
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
		auto t_Comp = selectionSquare->GetComponent<TransformComponent>();
		auto d_Comp = selectionSquare->GetComponent<DrawComponent>();

		t_Comp->m_f_Scale = HTVector2{ -(lastClickPosition.x - DrawPosition.x), lastClickPosition.y - DrawPosition.y };
		t_Comp->m_x_Position.x = (lastClickPosition.x + t_Comp->m_f_Scale.x / 2);
		t_Comp->m_x_Position.y = (lastClickPosition.y - t_Comp->m_f_Scale.y / 2);
		selectionSquare->m_b_IsActive = true;
	}

	//EditorInfoPanelUI->SetPosition(currentCamPosX, currentCamPosY);
	//EditorInfoPanelUI->Draw();

	if (m_x_SelectedList.size() == 1)
	{
		auto t_Comp = (*m_x_SelectedList.begin())->GetComponent<TransformComponent>();
		auto d_Comp = (*m_x_SelectedList.begin())->GetComponent<DrawComponent>();
		s8 chars[100] = {};
		sprintf_s(chars, 100, "Name: %s",(*m_x_SelectedList.begin())->m_pc_EntityName);
		AEGfxPrint(font, chars, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9, 1, 1, 1);
		s8 chars2[100] = {};
		sprintf_s(chars2, 100, "Pos: X: %.3f Y: %.3f ", t_Comp->GetPosition().x, t_Comp->GetPosition().y);
		AEGfxPrint(font, chars2, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9-30, 1, 1, 1);
		s8 chars3[100] = {};
		sprintf_s(chars3, 100, "Rotation: %.3f ", t_Comp->GetRotation());
		AEGfxPrint(font, chars3, currentCamPosX + ScreenSizeX - 310, currentCamPosY + (ScreenSizeY / 10) * 9 - 60, 1, 1, 1);
		s8 chars4[100] = {};
		sprintf_s(chars4, 100, "Scale: X=%.3f Y=%.3f", t_Comp->m_f_Scale.x, t_Comp->m_f_Scale.y);
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
		
		
	//sprintf_s(chars, 100, "Mouse Game Pos: %.2f,%.2f", selectionSquare->GetPosition().x, selectionSquare->GetPosition().y);
	sprintf_s(chars2, 100, "Mouse Screen Pos: %.2f,%.2f", static_cast<float>(currentMousePos.x),static_cast<float>(currentMousePos.y));
	sprintf_s(chars3, 100, "+");
	sprintf_s(chars5, 100, "SelectedObj: %s", SelectedObject != nullptr ? SelectedObject->m_pc_EntityName : "---");
	sprintf_s(chars7, 100, "GridLock: %s", isGridLock? "On" : "Off");

	count = 0;
	for (std::vector<StaticObjectEntity*>::iterator iter = m_v_PrefabVector.begin();
		iter < m_v_PrefabVector.end();
		++iter, ++count)
	{
		if (count == ObjCounter)
		{
			sprintf_s(chars4, 100, "%s", (*iter)->m_pc_EntityName);
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
	

	lastMousePosition = DrawPosition;
	/*
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	//show Main Window
	ImGui::ShowDemoWindow();
	// 1. Show a simple window.
			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	// Rendering
	ImGui::Render();
	//glViewport(0, 0, AEGetWindowWidth(), AEGetWindowHeight());                 
	//Display Size got from Resize Command
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//SwapBuffers(g_HDCDeviceContext);
	*/
}

StaticObjectEntity* LevelEditorSystem::CreateNewPrefab(float posX, float posY, float sizeX, 
	float sizeY, const char* textureName, const char * objectName)
{
	StaticObjectEntity* obj = static_cast<StaticObjectEntity*>(
		m_po_EntityManager->NewEntity(kEntityStaticObject, objectName));

	auto t_Comp = obj->GetComponent<TransformComponent>();
	t_Comp->SetPosition(posX, posY);
	t_Comp->SetRotation(0);
	obj->GetComponent<DrawComponent>()->Initialize(m_o_GraphicsSystem->FetchTexture(textureName),
		sizeX, sizeY, HTColor{ 1,1,1,1 });

	return obj;
}

StaticObjectEntity* LevelEditorSystem::CopyPrefab(StaticObjectEntity& rhs)
{
	StaticObjectEntity* obj = static_cast<StaticObjectEntity*>(
		m_po_EntityManager->NewEntity(kEntityStaticObject, rhs.m_pc_EntityName));

	auto t_Comp = obj->GetComponent<TransformComponent>();
	auto rhst_Comp = rhs.GetComponent<TransformComponent>();
	auto d_Comp = obj->GetComponent<DrawComponent>();
	auto rhsd_Comp = rhs.GetComponent<DrawComponent>();

	t_Comp->SetPosition(rhst_Comp->m_x_Position.x, rhst_Comp->m_x_Position.y);
	t_Comp->SetRotation(rhst_Comp->GetRotation());
	d_Comp->Initialize(rhsd_Comp->m_px_Texture, rhsd_Comp->m_x_MeshSize.x, rhsd_Comp->m_x_MeshSize.y, rhsd_Comp->m_f_RgbaColor);
	
	return obj;
}

StaticObjectEntity* LevelEditorSystem::CheckNearestSnap(Aabb& boundsCheck)
{
	if (m_x_ToSavePrefabMap.size() == 0)
		return nullptr;
	
	StaticObjectEntity* closestObjX = nullptr;
	StaticObjectEntity* closestObjY = nullptr;
	float shortestDistX = -1;
	float shortestDistY = -1;
	float shortestDist = -1;
	HTVector2 boundOrigin = { boundsCheck.min.x + (boundsCheck.max.x - boundsCheck.min.x) / 2,
							  boundsCheck.min.y + (boundsCheck.max.y - boundsCheck.min.y) / 2 };
	for (auto iterator = m_x_ToSavePrefabMap.begin(); iterator != m_x_ToSavePrefabMap.end(); ++iterator)
	{
		for (auto innerIter = (*iterator).second.begin(); innerIter != (*iterator).second.end(); ++innerIter)
		{
			auto t_Comp = (*innerIter)->GetComponent<TransformComponent>();
			auto d_Comp = (*innerIter)->GetComponent<DrawComponent>();
			Aabb inBounds = AabbHelper::GetAabb(t_Comp->GetPosition(), d_Comp->m_x_MeshSize, t_Comp->m_f_Scale);
			char orientation = AabbHelper::CalculateOrientation(HTVector2{boundsCheck.max.x- boundsCheck.min.x ,
				boundsCheck.max.y - boundsCheck.min.y }, t_Comp->GetPosition());
			

			switch (orientation)
			{
				case 3:
				case 1:
				{
					float distX = MathHT::CalculateDistanceX(boundOrigin, t_Comp->GetPosition())
					- ((boundsCheck.max.x - boundsCheck.min.x) / 2)
					- (d_Comp->m_x_MeshSize.x / 2);
				if (distX < shortestDistX || shortestDistX < 0)
				{
					shortestDistX = distX;
					closestObjX = *innerIter;
				}
				else if (distX == shortestDistX)
				{
					float distY = MathHT::CalculateDistanceY(boundOrigin, t_Comp->GetPosition())
						- ((boundsCheck.max.y - boundsCheck.min.y) / 2)
						- (d_Comp->m_x_MeshSize.y / 2);
					if (distY <= shortestDistY || shortestDistY < 0)
					{
						shortestDistY = distY;
						closestObjY = *innerIter;
					}
				}
				break;
				}
				case 2:
				case 4:
				{
					float distY = MathHT::CalculateDistanceY(boundOrigin, t_Comp->GetPosition())
						- ((boundsCheck.max.y - boundsCheck.min.y) / 2)
						- (d_Comp->m_x_MeshSize.y / 2);
					if (distY <= shortestDistY || shortestDistY < 0)
					{
						shortestDistY = distY;
						closestObjY = *innerIter;
					}
					else if (distY == shortestDistY)
					{
						float distX = MathHT::CalculateDistanceX(boundOrigin, t_Comp->GetPosition())
							- ((boundsCheck.max.x - boundsCheck.min.x) / 2)
							- (d_Comp->m_x_MeshSize.x / 2);
						if (distX < shortestDistX || shortestDistX < 0)
						{

							shortestDistX = distX;
							closestObjX = *innerIter;
						}
					}
					break;
				}
			}

		}
	}

	if (closestObjX == closestObjY )
		return closestObjX;
	if (!closestObjX && closestObjY)
		return closestObjY;
	if (!closestObjY && closestObjX)
		return closestObjX;

		
	if(shortestDistX < shortestDistY)
		return closestObjX;
	else
		return closestObjY;
}



void LevelEditorSystem::SnapToOrientation( bool hardSnap, HTVector2& positionToSnap, SnappingState& snapState,
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
			shortestDist = MathHT::CalculateDistanceX(positionToSnap, t_Comp->GetPosition());
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
			shortestDist = MathHT::CalculateDistanceY(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
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
			shortestDist = MathHT::CalculateDistanceX(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
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
			shortestDist = MathHT::CalculateDistanceY(positionToSnap, t_Comp->GetPosition()); // ouch there must be a better way
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


