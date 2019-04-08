/* Start Header****************************************************************/
/*!
\file AlphaEngineHelper.cpp
\author Primary Author : Lim Chu Yan, chuyan.lim
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\brief This file contains helper functions to some AE Engine features.

\par Contribution : CY - 100.00%

Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *****************************************************************/

#include "AlphaEngineHelper.h"
#include "AEEngine.h"


namespace AlphaEngineHelper {

	void AlphaInitialize(HINSTANCE hInstance, int nCmdShow)
	{
		AESysInit(hInstance, nCmdShow, 1920, 1080, 1, 60, true, nullptr);
		AESysSetWindowTitle("S.N.E.K.S.");
		AEToogleFullScreen(true);
		AEGfxSetBackgroundColor(1, 1, 0);
		AESysReset();
	}

	void GetScreenSize(float* retWidth, float* retHeight)
	{
		//assert(!retWidth || !retHeight);
		*retWidth = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		*retHeight = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	}

}