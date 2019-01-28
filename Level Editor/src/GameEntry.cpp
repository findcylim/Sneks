#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include <map>
#include <string>
#include <fstream>
#include "Graphics/DrawObject.h"
#include "AEEngine.h"
#include "Math/Aabb.h"
#include <sys/stat.h>
#include <Windows.h>
#include <vector>
#include <commdlg.h>

constexpr int kNumBodyParts = 20;

enum Objects
{
	kBuildingObj,
	kVerticalRoadObj,
	kHorizontalRoadObj,
	kNumberOfObjects
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

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<DrawObject*> background = {};
	for (int i_Background = -5; i_Background <= 5; i_Background++) {
		for (int i_BackgroundY = -5; i_BackgroundY <= 5; i_BackgroundY++) {
			auto bg = new DrawObject(i_Background * 1000, i_BackgroundY * 1000, 1000, 1000, editorScale,"EditorScale","EditorScale.png");
			background.push_back(bg);
		}
	}

	auto horRoad = new DrawObject(0, 0, 71, 9, horizontalRoadTexture,"Horizontal Road","horz-road.png");
	auto verRoad = new DrawObject(100, 100, 9, 42, verticalRoadTexture, "Vertical Road","vert-road.png");
	auto buildingObj = new DrawObject(0, 0, 71, 42, buildingTexture,"Building1","building.png");
	bool isTabPressed = false;
	char ObjCounter = kBuildingObj;

	std::vector<DrawObject*> PrefabVector;
	std::map<Objects, std::vector<DrawObject*>> ToSavePrefabMap;

	PrefabVector.push_back(buildingObj);
	PrefabVector.push_back(verRoad);
	PrefabVector.push_back(horRoad);

	horRoad->SetScale(2.0f);
	verRoad->SetScale(2.0f);

	

	auto font = AEGfxCreateFont("Arial", 20, false, false);
	auto winFont = AEGfxCreateFont("Arial", 500, 1, 0);

	float currentCamPosX, currentCamPosY,defaultCamPosX, defaultCamPosY;
	bool isMiddleMouseClicked	= false;
	bool isLeftMouseClicked		= false;
	bool isSaved				= false;
	POINT initialMousePos,currentMousePos;
	float ScreenSizeX = (AEGfxGetWinMaxX() - AEGfxGetWinMinX())/2;
	float ScreenSizeY = (AEGfxGetWinMaxY() - AEGfxGetWinMinY())/2;
	AEGfxGetCamPosition(&currentCamPosX, &currentCamPosY);
	AEGfxGetCamPosition(&defaultCamPosX, &defaultCamPosY);
	int winner = 0;

	while (!winner) {
		AESysFrameStart();
		AEInputUpdate();
		
		for (auto& i_Backgrounds : background) {
			i_Backgrounds->Draw();
		}

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

		float xMouseRealPos = static_cast<float>((currentCamPosX - ScreenSizeX) + currentMousePos.x);
		float yMouseRealPos = -static_cast<float>((currentCamPosY - ScreenSizeY) + currentMousePos.y);
		float xDiff=0.0f, yDiff= 0.0f;

		char count = 0;
		for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin(); iter < PrefabVector.end(); ++iter, ++count)
		{
			if (count == ObjCounter)
			{
				xDiff = (*iter)->GetSizeX()/2;
				yDiff = (*iter)->GetSizeY()/2;
				break;
			}
		}

		if (GetAsyncKeyState(VK_LSHIFT) < 0)
		{
			if (static_cast<int>(xMouseRealPos) % 100 < 50)
			{
				xMouseRealPos -= static_cast<int>(xMouseRealPos) % 100;
				xMouseRealPos += xDiff;
			}
			else
			{
				xMouseRealPos -= static_cast<int>(xMouseRealPos) % 100;
				xMouseRealPos += 100;
				xMouseRealPos += xDiff;
			}
		}

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
			count = 0;
			for (std::vector<DrawObject*>::iterator iter = PrefabVector.begin();iter<PrefabVector.end();++iter,++count)
			{
				if (count == ObjCounter)
				{
					(*iter)->SetPositionX(xMouseRealPos);
					(*iter)->SetPositionY(yMouseRealPos);
					(*iter)->Draw();
					break;
				}
			}
		}
		
		if (GetAsyncKeyState(1) < 0)
		{
			if (!isLeftMouseClicked)
			{
				isLeftMouseClicked = true;
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
				}
			}
		}
		else
		{
			isLeftMouseClicked = false;
		}
		
		for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
		{
			for (auto innerIter = (*iter).second.begin();innerIter != (*iter).second.end() ;++innerIter)
			{
				(*innerIter)->Draw();
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

					outFile << "<" << ofn.lpstrFile << ">" << std::endl;
					for (auto iter = ToSavePrefabMap.begin(); iter != ToSavePrefabMap.end(); ++iter)
					{
						for (auto innerIter = (*iter).second.begin(); innerIter != (*iter).second.end(); ++innerIter)
						{
							outFile << "<Entity>" << std::endl;
							outFile << "\t<Name=" << (*innerIter)->GetName() << ">" << std::endl;
							outFile << "\t\t<Pos X=" << (*innerIter)->GetPosition().x << " Y=" << (*innerIter)->GetPosition().y << ">" << std::endl;
							outFile << "\t\t<Size X=" << (*innerIter)->GetSizeX() << " Y=" << (*innerIter)->GetSizeY() << ">" << std::endl;
							outFile << "\t\t<Rotation=" << (*innerIter)->GetRotation() << ">" << std::endl;
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




		////Collision check with AABBs (Hardcoded)////////////////////////////////////////////////////////////////
		//Aabb snekHeadAabb ={};
		//snekHeadAabb.min = snek->m_po_Head->GetMin();
		//snekHeadAabb.max = snek->m_po_Head->GetMax();
		//Aabb snekHeadAabb2 ={};
		//snekHeadAabb2.min = snek2->m_po_Head->GetMin();
		//snekHeadAabb2.max = snek2->m_po_Head->GetMax();
		////Head on head action
		//if (CheckAabbIntersect(&snekHeadAabb, &snekHeadAabb2))
		//{
		//	//check iframes
		//	if (snek->m_po_Head->GetInvulnerable() > 0 || snek2->m_po_Head->GetInvulnerable() > 0){}
		//	else {
		//		cameraShake->AddShake(20.0f);
		//		snek->m_po_Head->SetInvulnerable(1.0f);
		//		snek2->m_po_Head->SetInvulnerable(1.0f);
		//		if (snek->m_v_BodyParts.empty()) {
		//			auto chars = new s8[100];
		//			sprintf_s(chars, 100, "PLAYER 1 WINS");

		//			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//			AEGfxPrint(winFont, chars, 0, 0, 0, 0, 1);
		//			MessageBox(nullptr, "PLAYER 1 WINS", "ENDGAME", MB_OK);
		//			return 1;
		//			winner = 1;
		//		}
		//		else if (snek2->m_v_BodyParts.empty())
		//		{
		//			auto chars = new s8[100];
		//			sprintf_s(chars, 100, "PLAYER 2 WINS");

		//			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		//			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		//			AEGfxPrint(winFont, chars, 0, 0, 1, 0, 0);
		//			MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);
		//			return 1;
		//			winner = 2;

		//		}
		//		snek2->m_v_BodyParts.pop_back();
		//		snek->m_v_BodyParts.pop_back();
		//		snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
		//		snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
		//	}
		//}
		//else {// collision check each head with the other snakes' body
		//	if (snek2->m_po_Head->GetInvulnerable() > 0){}
		//	else {
		//		auto i_BodyParts = snek2->m_v_BodyParts.begin();
		//		for (; i_BodyParts != snek2->m_v_BodyParts.end(); ++i_BodyParts)
		//		{
		//			if (snek2->m_po_Head->GetInvulnerable() > 0)
		//				break;
		//			Aabb bodyPartAabb ={};
		//			bodyPartAabb.min = (*i_BodyParts)->GetMin();
		//			bodyPartAabb.max = (*i_BodyParts)->GetMax();
		//			if (CheckAabbIntersect(&bodyPartAabb, &snekHeadAabb))
		//			{
		//				cameraShake->AddShake(20.0f);
		//				//snek->m_po_Head->SetColor(rand() % 10000);
		//				snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
		//				snek2->m_v_BodyParts.erase(i_BodyParts, snek2->m_v_BodyParts.end());
		//				snek2->m_po_Head->SetInvulnerable(2.0f);
		//				break;
		//			}
		//		}
		//	}
		//	// collision check head 2 with snake 1 body
		//	if (snek->m_po_Head->GetInvulnerable() > 0){}
		//	else {
		//		auto i_BodyParts2 = snek->m_v_BodyParts.begin();
		//		for (; i_BodyParts2 != snek->m_v_BodyParts.end(); ++i_BodyParts2)
		//		{
		//			if (snek->m_po_Head->GetInvulnerable() > 0)
		//				break;
		//			Aabb bodyPartAabb2 ={};
		//			bodyPartAabb2.min = (*i_BodyParts2)->GetMin();
		//			bodyPartAabb2.max = (*i_BodyParts2)->GetMax();
		//			if (CheckAabbIntersect(&bodyPartAabb2, &snekHeadAabb2))
		//			{
		//				cameraShake->AddShake(20.0f);
		//				//snek2->m_po_Head->SetColor(rand() % 10000);
		//				snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
		//				snek->m_v_BodyParts.erase(i_BodyParts2, snek->m_v_BodyParts.end());
		//				snek->m_po_Head->SetInvulnerable(2.0f);
		//				break;
		//			}
		//		}
		//	}

		//	// collision check heads with buildings
		//	auto i_Buildings = buildingsVec.begin();
		//	for (; i_Buildings != buildingsVec.end(); ++i_Buildings)
		//	{
		//		if ((*i_Buildings)->GetColor() == 9009)
		//		{
		//			continue;
		//		}
		//		Aabb buildingAabb ={};
		//		buildingAabb.min = (*i_Buildings)->GetMin();
		//		buildingAabb.max = (*i_Buildings)->GetMax();
		//		if (CheckAabbIntersect(&buildingAabb, &snekHeadAabb))
		//		{
		//			//cameraShake->AddShake(1.0f);

		//			(*i_Buildings)->SetColor(9009);
		//			float bodySpawnX;
		//			float bodySpawnY;
		//			if (!snek->m_v_BodyParts.empty()) {
		//				bodySpawnX = snek->m_v_BodyParts.back()->GetPosition().x;
		//				bodySpawnY = snek->m_v_BodyParts.back()->GetPosition().y;
		//			}else
		//			{
		//				bodySpawnX = snek->m_po_Head->GetPosition().x;
		//				bodySpawnY = snek->m_po_Head->GetPosition().y;
		//			}
		//			auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(bodySpawnX, bodySpawnY,
		//																				  61, 80, snakeBodyTexture));
		//			snek->AddBodyPart(snekBodyTest);
		//			break;
		//		}
		//		if (CheckAabbIntersect(&buildingAabb, &snekHeadAabb2))
		//		{
		//			//cameraShake->AddShake(1.0f);
		//			(*i_Buildings)->SetColor(9009);
		//			float bodySpawnX;
		//			float bodySpawnY;
		//			if (!snek2->m_v_BodyParts.empty()) {
		//				bodySpawnX = snek2->m_v_BodyParts.back()->GetPosition().x;
		//				bodySpawnY = snek2->m_v_BodyParts.back()->GetPosition().y;
		//			}
		//			else
		//			{
		//				bodySpawnX = snek2->m_po_Head->GetPosition().x;
		//				bodySpawnY = snek2->m_po_Head->GetPosition().y;
		//			}
		//			auto snekBodyTest2 = static_cast<SnekBody*>(new DrawObject(bodySpawnX, bodySpawnY,
		//																				  61, 80, snake2BodyTexture));
		//			snek2->AddBodyPart(snekBodyTest2);
		//			break;
		//		}
		//	}
		//}
		//Collision check end////////////////////////////////////////////////////////////////////////////////////

		//DRAW STARTS////////////////////////////////////////////////////////////////////////////////////
		/*		
		for (auto& i_Building : buildingsVec) {
			i_Building->Draw();
		}
		*/

		s8 chars[100] = {};
		s8 chars2[100]= {};
		s8 chars3[100]= {};
		s8 chars4[100]= {};

		

		sprintf_s(chars, 100, "CamPos: %.2f,%.2f", currentCamPosX,currentCamPosY);
		sprintf_s(chars2, 100, "MousePos: %.2f,%.2f", static_cast<float>(currentMousePos.x),static_cast<float>(currentMousePos.y));
		sprintf_s(chars3, 100, "+");
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
		AEGfxPrint(font, chars3, -3, -9, 0, 0, 0);
		AEGfxPrint(font, chars4, currentCamPosX - ScreenSizeX, currentCamPosY + (ScreenSizeY / 10) * 9-60, 0, 0, 1);
		/*
		*///DRAW ENDS////////////////////////////////////////////////////////////////////////////////////

		AESysFrameEnd();
	}
	AESysExit();

	return 0;
}