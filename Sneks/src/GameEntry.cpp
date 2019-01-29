#include "ECS/EventManager.h"
#include "Utility/Logger.h"
#include "ECS/ECSystem.h"
#include <iostream>

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "Graphics/DrawObject.h"
#include "Snek/SnekHead.h"
#include "Snek/SnekBody.h"
#include "Snek/Snek.h"
#include "AEEngine.h"
#include "Math/Aabb.h"
#include "Graphics/CameraShake.h"
#include "Math/PerlinNoise.h"

#include <Windows.h>
#include <vector>
#include <iostream>
#include "Graphics/Background.h"
#include "Graphics/Camera.h"
#include "Graphics/Buildings.h"
#include "Utility/DebugPrintToScreen.h"
#include "Math/Collisions.h"


constexpr int kNumBodyParts = 20;
float DrawObject::m_f_GlobalScale = 1.0f;
float DrawObject::m_f_GlobalCameraOffsetX = 0.0f;
float DrawObject::m_f_GlobalCameraOffsetY = 0.0f;

AEVec2 m_ScreenSize;
float GetScreenSizeX()
{
	return m_ScreenSize.x;
}
float GetScreenSizeY()
{
	return m_ScreenSize.y;
}

void SetCameraRelative(float x, float y)
{
	DrawObject::m_f_GlobalCameraOffsetX = x;
	DrawObject::m_f_GlobalCameraOffsetY = y;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	//MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 300, true, nullptr);
	m_ScreenSize.x = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	m_ScreenSize.y = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	AESysSetWindowTitle("TEST");
	AEToogleFullScreen(true);
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 1);

	auto camera		  = new Camera(&m_ScreenSize);
	auto cameraShake = new CameraShake();
	auto perlinNoise = new PerlinNoise();
	auto collisionManager = new Collisions();
	
	auto debugPrint = new DebugPrintToScreen("Segoe UI", 20);

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

	auto bgInstances = new Background(2, 2, cityTexture);
	auto buildingInstances = new Buildings(0, 0, buildingTexture);
	srand(static_cast<unsigned int>(time(nullptr)));

	auto snek  = new Snek(kNumBodyParts, 500, 0, snakeHeadTexture, snakeBodyTexture);
	auto snek2 = new Snek(kNumBodyParts, -150, 0, snake2HeadTexture, snake2BodyTexture);
	camera->AddToTrack(snek->m_po_Head);
	camera->AddToTrack(snek2->m_po_Head);
	snek2->SetPlayer(1);

	int winner = 0;

	s8 player1Boost[100] ={};
	s8 player2Boost[100]={};
	s8 currentShakeMag[100]={};
	debugPrint->AddToPrintList(player1Boost);
	debugPrint->AddToPrintList(player2Boost);
	debugPrint->AddToPrintList(currentShakeMag);

	bool movement = true;
	while (!winner) {
		AESysFrameStart();
		AEInputUpdate();

		camera->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
		cameraShake->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
		perlinNoise->Update();

		if (AEInputCheckReleased(AEVK_F5))
		{
			movement = !movement;
		}

		if (movement) {
			snek->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
			snek2->Update(static_cast<float>(AEFrameRateControllerGetFrameTime()));
		}

		//}
		//CAMERA ZOOM CHECKS FOR ZOOM END///////////////////////////////////////////////////////////////////////

		// Debug Controls////////////////////////////////////////////////////////////////////////////////////
		if (AEInputCheckReleased(AEVK_Z))
		{
			//cameraShake->AddShake(5.0f);
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
			buildingInstances->GenerateNewBuildings(20);
		}
		if (GetAsyncKeyState(AEVK_F4))
		{
			buildingInstances->RemoveBuildings();
		}
		//END DEBUG CONTROLS////////////////////////////////////////////////////////////////////////////////////
		
		/*
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

					//AEGfxPrint(winFont, chars, 0, 0, 0, 0, 1);
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

					//AEGfxPrint(winFont, chars, 0, 0, 1, 0, 0);
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
				if ((*i_Buildings)->GetColor().red == 1.0f && (*i_Buildings)->GetColor().blue == 0)
				{
					continue;
				}
				Aabb buildingAabb ={};
				buildingAabb.min = (*i_Buildings)->GetMin();
				buildingAabb.max = (*i_Buildings)->GetMax();
				if (CheckAabbIntersect(&buildingAabb, &snekHeadAabb))
				{
					//cameraShake->AddShake(1.0f);

					(*i_Buildings)->SetColor(1.0f,0,0,0.5f);
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
					(*i_Buildings)->SetColor(1.0f, 0, 0, 0.5f);
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


		}*/

		//Collision check end////////////////////////////////////////////////////////////////////////////////////

		//DRAW STARTS////////////////////////////////////////////////////////////////////////////////////
		bgInstances->Draw();
		buildingInstances->Draw();

		snek->Draw();
		snek2->Draw();



		auto boostLevel  = static_cast<float>(snek->m_po_Head->GetBoost());
		auto boostLevel2 = static_cast<float>(snek2->m_po_Head->GetBoost());
		auto shakeMagnitude  = static_cast<float>(cameraShake->AddShake(0));

		sprintf_s(player1Boost, 100, "Player 1 Boost: %.1f", boostLevel);
		sprintf_s(player2Boost, 100, "Player 2 Boost: %.1f", boostLevel2);
		sprintf_s(currentShakeMag, 100, "Shake Magnitude: %.3f", shakeMagnitude);

		
		debugPrint->Draw();
		//DRAW ENDS////////////////////////////////////////////////////////////////////////////////////

		AESysFrameEnd();
	}
	delete snek;
	AESysExit();
	if (winner == 1)
		MessageBox(nullptr, "PLAYER 1 WINS", "ENDGAME", MB_OK);
	else 
		MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);


/*

int main() {
	//game goes here
	int lol;
	ECSystem* Engine = new ECSystem;
	Engine->InitializeEngine();

	std::cout << "What" << std::endl;
	while (Engine->IsEngineOn())
	{
		Engine->Update();
	}

	delete(Engine);

	std::cin >> lol;

*/
	return 0;

}