/* Start Header ***************************************************************/
/*!
\file AlphaEngineHelper.cpp
\author Lim Chu Yan, chuyan.lim, 440002918
\par email: chuyan.lim\@digipen.edu
\par Course : GAM150
\par SNEKS ATTACK
\par High Tea Studios
\date Created: 12/02/2019
\date Modified: 26/03/2019
\brief This file contains helper functions to some AE Engine features.

\par Contribution (hours): CY - 1

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
		AEToogleFullScreen(false);
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