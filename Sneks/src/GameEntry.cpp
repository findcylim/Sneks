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

#include <Windows.h>
#include <vector>

constexpr int kNumBodyParts = 20;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1500, 900, 1, 300, false, nullptr);
	AESysSetWindowTitle("TEST");
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 0);

	auto snakeHeadTexture  = AEGfxTextureLoad("../Resources/head.png");
	auto snakeHeadTexture2 = AEGfxTextureLoad("../Resources/head2.png");
	auto snakeHeadLTexture = AEGfxTextureLoad("../Resources/snek_hed_l.jpg");
	auto snakeHeadRTexture = AEGfxTextureLoad("../Resources/snek_hed_r.jpg");
	auto snakeBodyTexture  = AEGfxTextureLoad("../Resources/bodytest.png");
	auto snakeBodyTexture2 = AEGfxTextureLoad("../Resources/bodytest2.png");
	auto rocketTexture     = AEGfxTextureLoad("../Resources/rocket_booster.jpg");
	auto smokeTexture      = AEGfxTextureLoad("../Resources/smoke.jpg");

	auto snekHeadTest = static_cast<SnekHead*>(new SnekHead(500, 0, 108, 78, snakeHeadTexture));
	snekHeadTest->SetParticles(smokeTexture, rocketTexture);
	snekHeadTest->SetColor(9999);
	snekHeadTest->SetRotation(PI);

	auto snekHeadTest2 = static_cast<SnekHead*>(new SnekHead(-150, 0, 108, 78, snakeHeadTexture2));
	snekHeadTest2->SetParticles(smokeTexture, rocketTexture);
	snekHeadTest2->SetColor(9999);
	snekHeadTest2->SetRotation(PI);

	auto snek = static_cast<Snek*>(new Snek(snekHeadTest));
	auto snek2 = static_cast<Snek*>(new Snek(snekHeadTest2));
	snek2->SetPlayer(1);
	for (int iBodyParts = 0; iBodyParts < kNumBodyParts; iBodyParts++) {
		auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 84, snakeBodyTexture));
		snek->AddBodyPart(snekBodyTest);
		auto snekBodyTest2 = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 84, snakeBodyTexture2));
		snek2->AddBodyPart(snekBodyTest2);
	}

	int prevMouseX = 0, prevMouseY = 0;
	while (true) {
		AESysFrameStart();
		AEInputUpdate();

		snek->Update();
		snek2->Update();

		//Collision check with AABBs (Hardcoded)
		Aabb snekHeadAabb = {};
		snekHeadAabb.min = snek->m_po_Head->GetMin();
		snekHeadAabb.max = snek->m_po_Head->GetMax();
		Aabb snekHeadAabb2 ={};
		snekHeadAabb2.min = snek2->m_po_Head->GetMin();
		snekHeadAabb2.max = snek2->m_po_Head->GetMax();

		//auto font = AEGfxCreateFont("Arial", 10, false, false);
		//s8* chars = "";
		//AEGfxPrint(font, "", -300, -500, 1, 1, 1);

		if (CheckAabbIntersect(&snekHeadAabb, &snekHeadAabb2))
		{
			if (snek->m_v_BodyParts.empty())  {
				MessageBox(nullptr, "PLAYER WINS", "ENDGAME", MB_OK);
				return 0;
			}
			else if (snek2->m_v_BodyParts.empty())
			{
				MessageBox(nullptr, "PLAYER 2 WINS", "ENDGAME", MB_OK);
				return 0;
			}
			snek2->m_v_BodyParts.pop_back();
			snek->m_v_BodyParts.pop_back();
			snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
			snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
		}
		else {// collision check each head with the other snakes' body
			auto i_BodyParts = snek2->m_v_BodyParts.begin();
			for (; i_BodyParts != snek2->m_v_BodyParts.end(); ++i_BodyParts)
			{
				Aabb bodyPartAabb ={};
				bodyPartAabb.min = (*i_BodyParts)->GetMin();
				bodyPartAabb.max = (*i_BodyParts)->GetMax();
				if (CheckAabbIntersect(&bodyPartAabb, &snekHeadAabb))
				{
					//snek->m_po_Head->SetColor(rand() % 10000);
					snek->m_po_Head->SetRotation(snek->m_po_Head->GetRotation() + PI);
					snek2->m_v_BodyParts.erase(i_BodyParts, snek2->m_v_BodyParts.end());
					break;
				}
			}


			auto i_BodyParts2 = snek->m_v_BodyParts.begin();
			for (; i_BodyParts2 != snek->m_v_BodyParts.end(); ++i_BodyParts2)
			{
				Aabb bodyPartAabb2 ={};
				bodyPartAabb2.min = (*i_BodyParts2)->GetMin();
				bodyPartAabb2.max = (*i_BodyParts2)->GetMax();
				if (CheckAabbIntersect(&bodyPartAabb2, &snekHeadAabb2))
				{
					//snek2->m_po_Head->SetColor(rand() % 10000);
					snek2->m_po_Head->SetRotation(snek2->m_po_Head->GetRotation() + PI);
					snek->m_v_BodyParts.erase(i_BodyParts2, snek->m_v_BodyParts.end());
					break;
				}
			}
		}

		//Collision check end

		snek->Draw();
		snek2->Draw();
		AESysFrameEnd();
	}
	delete snek;
	AESysExit();

	return 0;
}