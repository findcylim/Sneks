
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "DrawObject.h"
#include "SnekHead.h"
#include "SnekBody.h"

#define GLEW_STATIC

#include "AEEngine.h"

#include <Windows.h>



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	
	MessageBox(NULL, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1500, 900, 1, 300, false, NULL);
	AESysSetWindowTitle("TEST");
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 0);

	auto snakehed_texture =		 AEGfxTextureLoad("../Resources/snek_hed.jpg");
	auto snakehedl_texture =	 AEGfxTextureLoad("../Resources/snek_hed_l.jpg");
	auto snakehedr_texture =	 AEGfxTextureLoad("../Resources/snek_hed_r.jpg");
	auto snekbody_texture =		 AEGfxTextureLoad("../Resources/red_snake.jpg");
	auto rocket_texture =		 AEGfxTextureLoad("../Resources/rocket_booster.jpg");
	auto smoke_texture =		 AEGfxTextureLoad("../Resources/smoke.jpg");

	SnekHead* snekHeadTest = (SnekHead*)new SnekHead(-150, 0, 200, 100, snakehed_texture, snakehedl_texture, snakehedr_texture);
	snekHeadTest->SetParticles(smoke_texture, rocket_texture);

	SnekBody* snekBodyTest = (SnekBody*)new DrawObject(100, 0, 200, 100, snekbody_texture);
	snekBodyTest->refHead = snekHeadTest;
	SnekBody* snekBodyTest2 = (SnekBody*)new DrawObject(100, 0, 200, 100, snekbody_texture);
	snekBodyTest2->refHead = snekBodyTest;

	while (1) {
		AESysFrameStart();

		//printf("X: %f Y: %f\n", testObj->GetPositionX(), testObj->GetPositionY());
		snekHeadTest->Update();
		snekBodyTest->Update();
		//snekBodyTest2->Update();
		snekHeadTest->Draw();
		snekBodyTest->Draw();
		//snekBodyTest2->Draw();
		
		AESysFrameEnd();
	}

	AESysExit();
	
	return 0;
}
