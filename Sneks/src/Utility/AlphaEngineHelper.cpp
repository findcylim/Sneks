#include "AlphaEngineHelper.h"
#include "AEEngine.h"


namespace AlphaEngineHelper {

	void AlphaInitialize(HINSTANCE hInstance, int nCmdShow)
	{
		AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 60, true, nullptr);
		AESysSetWindowTitle("S.N.E.K.S.");
		AEToogleFullScreen(false);
		AESysReset();
		AEGfxSetBackgroundColor(1, 1, 0);
	}

	void GetScreenSize(float* retWidth, float* retHeight)
	{
		//assert(!retWidth || !retHeight);
		*retWidth = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		*retHeight = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	}

}