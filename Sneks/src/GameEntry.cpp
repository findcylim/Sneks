#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <time.h>
#include "DrawObject.h"
#include "SnekHead.h"
#include "SnekBody.h"
#include "Snek.h"
#include "AEEngine.h"

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

	auto snakeHeadTexture  = AEGfxTextureLoad("../Resources/snake-head.png");
	auto snakeHeadLTexture = AEGfxTextureLoad("../Resources/snek_hed_l.jpg");
	auto snakeHeadRTexture = AEGfxTextureLoad("../Resources/snek_hed_r.jpg");
	auto snakeBodyTexture  = AEGfxTextureLoad("../Resources/snake-body.png");
	auto rocketTexture     = AEGfxTextureLoad("../Resources/rocket_booster.jpg");
	auto smokeTexture      = AEGfxTextureLoad("../Resources/smoke.jpg");

	auto snekHeadTest = static_cast<SnekHead*>(new SnekHead(500, 0, 105, 77, snakeHeadTexture));
	snekHeadTest->SetParticles(smokeTexture, rocketTexture);
	snekHeadTest->SetColor(9999);

	auto snekHeadTest2 = static_cast<SnekHead*>(new SnekHead(-150, 0, 105, 77, snakeHeadTexture));
	snekHeadTest2->SetParticles(smokeTexture, rocketTexture);
	srand(static_cast<unsigned int>(time(nullptr)));

	snekHeadTest2->SetColor(rand() % 1000 * 10 + 9);

	auto snek = static_cast<Snek*>(new Snek(snekHeadTest));
	auto snek2 = static_cast<Snek*>(new Snek(snekHeadTest2));
	snek2->SetPlayer(1);
	for (int iBodyParts = 0; iBodyParts < kNumBodyParts; iBodyParts++) {
		auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snakeBodyTexture));
		snek->AddBodyPart(snekBodyTest);
		auto snekBodyTest2 = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snakeBodyTexture));
		snekBodyTest2->SetColor(rand() % 10000);
		snek2->AddBodyPart(snekBodyTest2);
	}

	int prevMouseX = 0, prevMouseY = 0;
	while (true) {
		AESysFrameStart();
		AEInputUpdate();

		snek->Update();
		snek->Draw();
		snek2->Update();
		snek2->Draw();
		AESysFrameEnd();
	}
	delete snek;
	AESysExit();

	return 0;
}