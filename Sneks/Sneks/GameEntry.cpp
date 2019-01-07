#include <glew/glew.h>
#include <glew/wglew.h>

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
	AESysInit(hInstance, nCmdShow, 1500, 900, 1, 300, NULL);
	AESysSetWindowTitle("TEST");
	AESysReset();
	AEGfxSetBackgroundColor(1, 1, 0);

	auto text_texture = AEGfxTextureLoad("../rsc/snek_hed.jpg");
	auto text_texture2 = AEGfxTextureLoad("../rsc/red_snake.jpg");

	SnekHead* testObj = (SnekHead*)new DrawObject(-150, 0, 200, 100, text_texture);
	SnekBody* testObj2 = (SnekBody*)new DrawObject(100, 0, 200, 100, text_texture2);
	testObj2->refHead = testObj;

	while (1) {
		AESysFrameStart();

		//printf("X: %f Y: %f\n", testObj->GetPositionX(), testObj->GetPositionY());
		testObj->Update();
		testObj2->Update();
		testObj->Draw();
		testObj2->Draw();
		
		AESysFrameEnd();
	}

	AESysExit();
	
	return 0;
}
