#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "DrawObject.h"
#include "SnekHead.h"
#include "SnekBody.h"
#include "Snek.h"
#include "AEEngine.h"
#include "Aabb.h"
#include "CameraShake.h"

#include <Windows.h>
#include <vector>

constexpr int kNumBodyParts = 20;
float DrawObject::m_f_GlobalScale = 1.0f;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	//MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 300, true, nullptr);
	AESysSetWindowTitle("TEST");
	AEToogleFullScreen(true);
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 1);

	auto cameraShake = new CameraShake();
	
	auto snakeHeadTexture           = AEGfxTextureLoad("../Resources/snake-head.png");
	auto snakeHeadLTexture          = AEGfxTextureLoad("../Resources/snek_hed_l.jpg");
	auto snakeHeadRTexture          = AEGfxTextureLoad("../Resources/snek_hed_r.jpg");
	auto snake2HeadTexture          = AEGfxTextureLoad("../Resources/head2.png");
	auto snakeBodyTexture           = AEGfxTextureLoad("../Resources/snake-body.png");
	auto snake2BodyTexture          = AEGfxTextureLoad("../Resources/snake-body2.png");
	auto rocketTexture              = AEGfxTextureLoad("../Resources/rocket_booster.jpg");
	auto smokeTexture               = AEGfxTextureLoad("../Resources/smoke.jpg");
	auto cityTexture		           = AEGfxTextureLoad("../Resources/map.png");
	auto verticalRoadTexture		  = AEGfxTextureLoad("../Resources/vert-road.png");
	auto horizontalRoadTexture		  = AEGfxTextureLoad("../Resources/horz-road.png");
	auto buildingTexture    		  = AEGfxTextureLoad("../Resources/building.png");

	srand(static_cast<unsigned int>(time(nullptr)));

	std::vector<DrawObject*> background = {};
	float ScreenSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	float ScreenSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	for (int i_Background = -5; i_Background <= 5; i_Background++) {
		for (int i_BackgroundY = -5; i_BackgroundY <= 5; i_BackgroundY++) {
			auto bg = new DrawObject(i_Background * ScreenSizeX, i_BackgroundY * ScreenSizeY, ScreenSizeX, ScreenSizeY, cityTexture);
			background.push_back(bg);
		}
	}

	auto horRoad = new DrawObject(0, 0, 71, 9, horizontalRoadTexture);
	auto verRoad = new DrawObject(100, 100, 9, 42, verticalRoadTexture);

	std::vector<DrawObject*> buildingsVec ={};
	std::vector<AEVec2> built ={};
	constexpr int buildingsDistX = 80;
	constexpr int buildingsDistY = 51;
	int maxBuildingsX = 1920 / 80;
	int maxBuildingsY = 1080 / 51;
	float firstBuildingX = -35.5f - buildingsDistX * 11;
	float firstBuildingY = -buildingsDistY * 10;

	for (int iBuildings = 0; iBuildings < 50; iBuildings++) {

		AEVec2 randIndex = AEVec2();
		bool uniqueIndex = false;
		while (!uniqueIndex) {
			uniqueIndex = true;
			randIndex.x = static_cast<f32>(rand() % maxBuildingsX);
			randIndex.y = static_cast<f32>(rand() % maxBuildingsY);
			for (auto& i_Built : built) {
				if (i_Built.x == randIndex.x && i_Built.y == randIndex.y)
				{
					uniqueIndex = false;
				}
			}
		}
		built.push_back(randIndex);
		DrawObject* building = new DrawObject(firstBuildingX + randIndex.x * buildingsDistX, firstBuildingY + randIndex.y * buildingsDistY, 71, 42, buildingTexture);
		buildingsVec.push_back(building);
	}

	horRoad->SetScale(2.0f);
	verRoad->SetScale(2.0f);

	auto snekHeadTest = static_cast<SnekHead*>(new SnekHead(500, 0, 105, 77, snakeHeadTexture));
	snekHeadTest->SetParticles(smokeTexture, rocketTexture);
	//snekHeadTest->SetColor(9999);
	snekHeadTest->SetRotation(PI);

	auto snekHeadTest2 = static_cast<SnekHead*>(new SnekHead(-150, 0, 105, 77, snake2HeadTexture));
	snekHeadTest2->SetParticles(smokeTexture, rocketTexture);

	//snekHeadTest2->SetColor(rand() % 1000 * 10 + 9);

	auto snek = static_cast<Snek*>(new Snek(snekHeadTest));
	auto snek2 = static_cast<Snek*>(new Snek(snekHeadTest2));

	auto font = AEGfxCreateFont("Arial", 20, false, false);
	auto winFont = AEGfxCreateFont("Arial", 500, 1, 0);

	float f, ff;
	AEGfxGetCamPosition(&f, &ff);
	snek2->SetPlayer(1);
	for (int iBodyParts = 0; iBodyParts < kNumBodyParts; iBodyParts++) {
		auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snakeBodyTexture));
		snek->AddBodyPart(snekBodyTest);
		auto snekBodyTest2 = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snake2BodyTexture));
		//snekBodyTest2->SetColor(rand() % 10000);
		snek2->AddBodyPart(snekBodyTest2);
	}
	int winner = 0;

	while (!winner) {
		AESysFrameStart();
		AEInputUpdate();
		float currentViewDistanceX = ScreenSizeX / DrawObject::m_f_GlobalScale;
		float currentViewDistanceY = ScreenSizeY / DrawObject::m_f_GlobalScale;

		cameraShake->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
		snek->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
		snek2->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));

		float distHedFromScreenEdgeX = fabsf(snek->m_po_Head->GetPosition().x) - currentViewDistanceX / 2;
		float distHedFromScreenEdgeY = fabsf(snek->m_po_Head->GetPosition().y) - currentViewDistanceY / 2;

		//CAMERA ZOOM CHECKS FOR ZOOM OUT ///////////////////////////////////////////////////////////////////////
		if (distHedFromScreenEdgeX + 50  > 0)
		{
			DrawObject::m_f_GlobalScale -= (distHedFromScreenEdgeX + 50) / ScreenSizeX;
		}
		else if (distHedFromScreenEdgeY + 50  > 0)
		{
			DrawObject::m_f_GlobalScale -= (distHedFromScreenEdgeY + 50) / ScreenSizeY;
		}

		float distHed2FromScreenEdgeX = fabsf(snek2->m_po_Head->GetPosition().x) - currentViewDistanceX / 2;
		float distHed2FromScreenEdgeY = fabsf(snek2->m_po_Head->GetPosition().y) - currentViewDistanceY / 2;

		//CAMERA ZOOM CHECKS FOR HED 2
		if (distHed2FromScreenEdgeX + 50 > 0)
		{
			DrawObject::m_f_GlobalScale -= (distHed2FromScreenEdgeX + 50) / ScreenSizeX;
		}
		else if (distHed2FromScreenEdgeY + 50 > 0)
		{
			DrawObject::m_f_GlobalScale -= (distHed2FromScreenEdgeY + 50) / ScreenSizeY;
		}
		//CHECK FOR ZOOM IN
		else if (DrawObject::m_f_GlobalScale < 1.0f) {

			if (-distHedFromScreenEdgeX > 100 && -distHed2FromScreenEdgeX > 100 &&
				-distHedFromScreenEdgeY > 100 && -distHed2FromScreenEdgeY > 100 &&
				DrawObject::m_f_GlobalScale > 0.75f)
			{
				DrawObject::m_f_GlobalScale += 0.0005f;
			}
			else if (-distHedFromScreenEdgeX > 400 && -distHed2FromScreenEdgeX > 400 &&
				-distHedFromScreenEdgeY > 400 && -distHed2FromScreenEdgeY > 400)
			{
				DrawObject::m_f_GlobalScale += 0.001f;
				//ScreenSizeX / ((currentViewDistanceX - distHedFromScreenEdgeX) / 2) - DrawObject::m_f_GlobalScale;// (-distHedFromScreenEdgeY - 300 / DrawObject::m_f_GlobalScale) / ScreenSizeX;
			}
		}
			//if (-distHedFromScreenEdgeY > 1000)
			//{
			//	DrawObject::m_f_GlobalScale += 0.001f;// (-distHedFromScreenEdgeY - 300 / DrawObject::m_f_GlobalScale) / ScreenSizeX;
			//}
		//}
		//CAMERA ZOOM CHECKS FOR ZOOM END///////////////////////////////////////////////////////////////////////

		// Debug Controls////////////////////////////////////////////////////////////////////////////////////
		if (GetAsyncKeyState(AEVK_Z))
		{
			cameraShake->AddShake(5.0f);
		}
		if (GetAsyncKeyState(AEVK_3))
		{
			DrawObject::m_f_GlobalScale += 0.001f;
		}
		if (GetAsyncKeyState(AEVK_4))
		{
			DrawObject::m_f_GlobalScale -= 0.001f;
		}
		if (GetAsyncKeyState(AEVK_F1))
		{
			snek->m_po_Head->SetInvulnerable(10.0f);
		}
		if (GetAsyncKeyState(AEVK_F2))
		{
			snek2->m_po_Head->SetInvulnerable(10.0f);
		}
		if (GetAsyncKeyState(AEVK_F3))
		{
			if (built.size() >= maxBuildingsX * maxBuildingsY)
			{
				
			}
			else {
				AEVec2 randIndex = AEVec2();
				bool uniqueIndex = false;
				while (!uniqueIndex) {
					uniqueIndex = true;
					randIndex.x = static_cast<f32>(rand() % maxBuildingsX);
					randIndex.y = static_cast<f32>(rand() % maxBuildingsY);
					for (auto& i_Built : built) {
						if (i_Built.x == randIndex.x && i_Built.y == randIndex.y)
						{
							uniqueIndex = false;
						}
					}
				}
				built.push_back(randIndex);
				DrawObject* building = new DrawObject(firstBuildingX + randIndex.x * buildingsDistX,
					firstBuildingY + randIndex.y * buildingsDistY, 71, 42, buildingTexture);
				buildingsVec.push_back(building);
			}
		}
		if (GetAsyncKeyState(AEVK_F4))
		{
			buildingsVec.clear();
			built.clear();
		}
		//END DEBUG CONTROLS////////////////////////////////////////////////////////////////////////////////////

		//Collision check with AABBs (Hardcoded)////////////////////////////////////////////////////////////////
		Aabb snekHeadAabb ={};
		snekHeadAabb.min = snek->m_po_Head->GetMin();
		snekHeadAabb.max = snek->m_po_Head->GetMax();
		Aabb snekHeadAabb2 ={};
		snekHeadAabb2.min = snek2->m_po_Head->GetMin();
		snekHeadAabb2.max = snek2->m_po_Head->GetMax();
		//Head on head action
		if (CheckAabbIntersect(&snekHeadAabb, &snekHeadAabb2))
		{
			//check iframes
			if (snek->m_po_Head->GetInvulnerable() > 0 || snek2->m_po_Head->GetInvulnerable() > 0){}
			else {
				cameraShake->AddShake(20.0f);
				snek->m_po_Head->SetInvulnerable(1.0f);
				snek2->m_po_Head->SetInvulnerable(1.0f);
				if (snek->m_v_BodyParts.empty()) {
					auto chars = new s8[100];
					sprintf_s(chars, 100, "PLAYER 1 WINS");

					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					AEGfxSetBlendMode(AE_GFX_BM_BLEND);

					AEGfxPrint(winFont, chars, 0, 0, 0, 0, 1);
					MessageBox(nullptr, "PLAYER 1 WINS", "ENDGAME", MB_OK);
					return 1;
					winner = 1;
				}
				else if (snek2->m_v_BodyParts.empty())
				{
					auto chars = new s8[100];
					sprintf_s(chars, 100, "PLAYER 2 WINS");

					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					AEGfxSetBlendMode(AE_GFX_BM_BLEND);

					AEGfxPrint(winFont, chars, 0, 0, 1, 0, 0);
					MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);
					return 1;
					winner = 2;

				}
				snek2->m_v_BodyParts.pop_back();
				snek->m_v_BodyParts.pop_back();
				snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
				snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
			}
		}
		else {// collision check each head with the other snakes' body
			if (snek2->m_po_Head->GetInvulnerable() > 0){}
			else {
				auto i_BodyParts = snek2->m_v_BodyParts.begin();
				for (; i_BodyParts != snek2->m_v_BodyParts.end(); ++i_BodyParts)
				{
					if (snek2->m_po_Head->GetInvulnerable() > 0)
						break;
					Aabb bodyPartAabb ={};
					bodyPartAabb.min = (*i_BodyParts)->GetMin();
					bodyPartAabb.max = (*i_BodyParts)->GetMax();
					if (CheckAabbIntersect(&bodyPartAabb, &snekHeadAabb))
					{
						cameraShake->AddShake(20.0f);
						//snek->m_po_Head->SetColor(rand() % 10000);
						snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
						snek2->m_v_BodyParts.erase(i_BodyParts, snek2->m_v_BodyParts.end());
						snek2->m_po_Head->SetInvulnerable(2.0f);
						break;
					}
				}
			}
			// collision check head 2 with snake 1 body
			if (snek->m_po_Head->GetInvulnerable() > 0){}
			else {
				auto i_BodyParts2 = snek->m_v_BodyParts.begin();
				for (; i_BodyParts2 != snek->m_v_BodyParts.end(); ++i_BodyParts2)
				{
					if (snek->m_po_Head->GetInvulnerable() > 0)
						break;
					Aabb bodyPartAabb2 ={};
					bodyPartAabb2.min = (*i_BodyParts2)->GetMin();
					bodyPartAabb2.max = (*i_BodyParts2)->GetMax();
					if (CheckAabbIntersect(&bodyPartAabb2, &snekHeadAabb2))
					{
						cameraShake->AddShake(20.0f);
						//snek2->m_po_Head->SetColor(rand() % 10000);
						snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
						snek->m_v_BodyParts.erase(i_BodyParts2, snek->m_v_BodyParts.end());
						snek->m_po_Head->SetInvulnerable(2.0f);
						break;
					}
				}
			}

			// collision check heads with buildings
			auto i_Buildings = buildingsVec.begin();
			for (; i_Buildings != buildingsVec.end(); ++i_Buildings)
			{
				if ((*i_Buildings)->GetColor() == 9009)
				{
					continue;
				}
				Aabb buildingAabb ={};
				buildingAabb.min = (*i_Buildings)->GetMin();
				buildingAabb.max = (*i_Buildings)->GetMax();
				if (CheckAabbIntersect(&buildingAabb, &snekHeadAabb))
				{
					//cameraShake->AddShake(1.0f);

					(*i_Buildings)->SetColor(9009);
					float bodySpawnX;
					float bodySpawnY;
					if (!snek->m_v_BodyParts.empty()) {
						bodySpawnX = snek->m_v_BodyParts.back()->GetPosition().x;
						bodySpawnY = snek->m_v_BodyParts.back()->GetPosition().y;
					}else
					{
						bodySpawnX = snek->m_po_Head->GetPosition().x;
						bodySpawnY = snek->m_po_Head->GetPosition().y;
					}
					auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(bodySpawnX, bodySpawnY,
																						  61, 80, snakeBodyTexture));
					snek->AddBodyPart(snekBodyTest);
					break;
				}
				if (CheckAabbIntersect(&buildingAabb, &snekHeadAabb2))
				{
					//cameraShake->AddShake(1.0f);
					(*i_Buildings)->SetColor(9009);
					float bodySpawnX;
					float bodySpawnY;
					if (!snek2->m_v_BodyParts.empty()) {
						bodySpawnX = snek2->m_v_BodyParts.back()->GetPosition().x;
						bodySpawnY = snek2->m_v_BodyParts.back()->GetPosition().y;
					}
					else
					{
						bodySpawnX = snek2->m_po_Head->GetPosition().x;
						bodySpawnY = snek2->m_po_Head->GetPosition().y;
					}
					auto snekBodyTest2 = static_cast<SnekBody*>(new DrawObject(bodySpawnX, bodySpawnY,
																						  61, 80, snake2BodyTexture));
					snek2->AddBodyPart(snekBodyTest2);
					break;
				}
			}


		}
		//Collision check end////////////////////////////////////////////////////////////////////////////////////

		//DRAW STARTS////////////////////////////////////////////////////////////////////////////////////
		for (auto& i_Backgrounds : background) {
			i_Backgrounds->Draw();
		}		
		for (auto& i_Building : buildingsVec) {
			i_Building->Draw();
		}
		//horRoad->Draw();
		//verRoad->Draw();
		snek->Draw();
		snek2->Draw();

		s8 chars[100] = {};
		s8 chars2[100]= {};
		s8 chars3[100]={};

		auto boostLevel  = static_cast<float>(snek->m_po_Head->GetBoost());
		auto boostLevel2 = static_cast<float>(snek2->m_po_Head->GetBoost());
		auto shakeMagnitude  = static_cast<float>(cameraShake->AddShake(0));


		sprintf_s(chars, 100, "Player 1 Boost: %.1f", boostLevel);
		sprintf_s(chars2, 100, "Player 2 Boost: %.1f", boostLevel2);
		sprintf_s(chars3, 100, "Shake Magnitude: %.3f", shakeMagnitude);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_NONE);

		AEGfxPrint(font, chars, -900, 450, 0, 0, 1);
		AEGfxPrint(font, chars2, -900, 420, 1, 0, 0);
		AEGfxPrint(font, chars3, -900, 390, 0, 1, 0);
		//DRAW ENDS////////////////////////////////////////////////////////////////////////////////////

		AESysFrameEnd();
	}
	delete snek;
	AESysExit();
	if (winner == 1)
		MessageBox(nullptr, "PLAYER 1 WINS", "ENDGAME", MB_OK);
	else 
		MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);
	return 0;
}