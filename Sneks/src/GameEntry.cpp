
#include <cstdio>
#include <cmath>
#include <cstdlib>
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

	auto snekHeadTest = static_cast<SnekHead*>(new SnekHead(-150, 0, 105, 77, snakeHeadTexture));
	snekHeadTest->SetParticles(smokeTexture, rocketTexture);

	auto snek = static_cast<Snek*>(new Snek(snekHeadTest));

	for (int iBodyParts = 0; iBodyParts < kNumBodyParts; iBodyParts++) {
		auto snekBodyTest = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snakeBodyTexture));
		snek->AddBodyPart(snekBodyTest);
	}

	int prevMouseX = 0, prevMouseY = 0;
	while (true) {
		AESysFrameStart();
		AEInputUpdate();

		/* mouse position debugger
		if (AEInputCheckCurr(AEVK_A)) {
			int mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);

			if ( (abs(mouseX - prevMouseX) > 2) && (abs(mouseY - prevMouseY) > 2)) {
				printf("mouseX: %d, mouseY: %d\n", mouseX, mouseY);
				prevMouseX = mouseX;
				prevMouseY = mouseY;
			}
		}*/

		snek->Update();
		snek->Draw();

		AESysFrameEnd();
	}

	AESysExit();
	
	return 0;
}
