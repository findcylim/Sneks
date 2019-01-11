
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include "DrawObject.h"
#include "SnekHead.h"
#include "SnekBody.h"


#include "AEEngine.h"

#include <Windows.h>



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	
	MessageBox(nullptr, "CONTROLS ARE UP DOWN LEFT RIGHT", "NOOB", MB_OK);
	AESysInit(hInstance, nCmdShow, 1500, 900, 1, 300, false, nullptr);
	AESysSetWindowTitle("TEST");
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 0);

	auto snake_head_texture =		 AEGfxTextureLoad("../Resources/snek_hed.jpg");
	auto snake_head_l_texture =		 AEGfxTextureLoad("../Resources/snek_hed_l.jpg");
	auto snake_head_r_texture =		 AEGfxTextureLoad("../Resources/snek_hed_r.jpg");
	auto snake_body_texture =		 AEGfxTextureLoad("../Resources/snake-body.png");
	auto rocket_texture =			 AEGfxTextureLoad("../Resources/rocket_booster.jpg");
	auto smoke_texture =			 AEGfxTextureLoad("../Resources/smoke.jpg");

	auto snek_head_test = static_cast<SnekHead*>(new SnekHead(-150, 0, 200, 100, snake_head_texture, snake_head_l_texture, snake_head_r_texture));
	snek_head_test->SetParticles(smoke_texture, rocket_texture);

	auto snek_body_test  = static_cast<SnekBody*>(new DrawObject( 100, 0, 61, 80, snake_body_texture));
	snek_body_test->refHead = snek_head_test;
	auto snek_body_test2 = static_cast<SnekBody*>(new DrawObject(100, 0, 61, 80, snake_body_texture));
	snek_body_test2->refHead = snek_body_test;

	while (true) {
		AESysFrameStart();

		//printf("X: %f Y: %f\n", testObj->GetPositionX(), testObj->GetPositionY());
		snek_head_test->Update();
		snek_body_test->Update();
		//snek_body_test2->Update();
		snek_head_test->Draw();
		snek_body_test->Draw();
		//snek_body_test2->Draw();
		
		AESysFrameEnd();
	}

	AESysExit();
	
	return 0;
}
